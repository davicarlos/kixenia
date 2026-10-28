/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2025 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

// The avatar assets xam's avatar app (avatarcore in xam 17559) builds for
// XamAvatarGetAssets. Parts come from the system AvatarAssetPack that titles
// ship, or from titles' own assets. Each part is an STRB file whose model
// block holds bit packed meshes, textures and seams, LZX compressed.

#include "xenia/kernel/xam/xam_avatar_assets.h"

#include <array>
#include <cmath>
#include <cstring>
#include <mutex>
#include <optional>
#include <span>
#include <vector>

#include "xenia/base/byte_order.h"
#include "xenia/base/logging.h"
#include "xenia/base/math.h"
#include "xenia/cpu/lzx.h"
#include "xenia/gpu/texture_address.h"
#include "xenia/kernel/kernel_state.h"
#include "xenia/kernel/util/shim_utils.h"
#include "xenia/vfs/devices/host_path_entry.h"
#include "xenia/vfs/devices/xcontent_container_device.h"

namespace xe {
namespace kernel {
namespace xam {
namespace avatar {

namespace {

// Per component sizes, by bit of the component mask (0x816332B8).
struct ComponentSizes {
  uint32_t header;
  uint32_t gpu_resources;
};
constexpr ComponentSizes kComponentSizes[] = {
    {0x800, 0x53800}, {0x400, 0x12C00}, {0x800, 0xE000}, {0x800, 0x14400},
    {0x800, 0xF000},  {0x800, 0xD400},  {0x800, 0xA400}, {0x800, 0x6800},
    {0x800, 0x8400},  {0x800, 0x5C00},  {0x800, 0x5800}, {0x800, 0x5C00},
    {0x800, 0x19000},
};
constexpr uint16_t kBodyComponent = 0x1000;

// The fixed part holds the asset header, the skeleton and the per component
// tables; asking for the body adds its skeleton and animation room.
constexpr uint32_t kResultFixedSize = 0x1F80;
constexpr uint32_t kResultFixedSizeWithBody = 0x3147C;

constexpr uint32_t kMaxParts = 13;
constexpr uint32_t kPartRecordSize = 0x20;
constexpr uint32_t kComponentSize = 0x34;
constexpr uint32_t kMeshRecordSize = 0x200;
constexpr uint32_t kMeshMaxSubsets = 20;
constexpr uint32_t kTextureRecordSize = 0x2C;

// Manifest layout.
constexpr uint32_t kManifestBodyA = 0x120;
constexpr uint32_t kManifestBodyB = 0x140;
constexpr uint32_t kManifestParts = 0x160;
constexpr uint32_t kManifestPartsEnd = 0x300;
constexpr uint32_t kManifestDefaultParts = 0x300;
constexpr uint32_t kManifestTextures = 0x3C;
constexpr uint32_t kManifestShapes = 0xC;
constexpr uint32_t kManifestColors = 0xFC;
// Categories a manifest always has a part for: shoes, pants, shirt and one
// more (0x816331A0).
constexpr uint16_t kRequiredCategories[] = {0x20, 0x10, 0x8, 0x4};

constexpr uint8_t kStrbFormat[16] = {0x59, 0xBD, 0xE0, 0x8B, 0x49, 0x09,
                                     0x4E, 0xE4, 0xB9, 0x91, 0xAD, 0xA8,
                                     0x7B, 0x7C, 0x0B, 0x6B};
enum StrbBlock : uint8_t {
  kStrbBlockModel = 3,
  kStrbBlockDescriptorV1 = 6,
  kStrbBlockPalette = 7,
  kStrbBlockDescriptor = 8,
};

// Steps of the hexagonal close packed lattice vertex positions are quantized
// to (0x81A975C0, set up at 0x81A70F50), and the shifts of odd rows/layers.
const float kLatticeStep[3] = {2.0f, float(std::sqrt(6.0) * (2.0 / 3.0)),
                               float(std::sqrt(3.0))};
constexpr float kLatticeOddLayerShift = 1.0f / 3.0f;
constexpr float kLatticeOddRowShift = 0.5f;

std::mutex state_mutex;
uint32_t skinning_version = 2;
uint32_t coordinate_mode = 1;

struct AssetPack {
  bool loaded = false;
  std::vector<uint8_t> toc;
  uint32_t entry_count = 0;
};
AssetPack asset_pack;

uint32_t load_be32(const uint8_t* p) { return xe::load_and_swap<uint32_t>(p); }

uint8_t* TranslateVirtual(uint32_t address) {
  return kernel_state()->memory()->TranslateVirtual<uint8_t*>(address);
}

bool IsNullId(const uint8_t* id) {
  for (uint32_t i = 0; i < 16; ++i) {
    if (id[i]) {
      return false;
    }
  }
  return true;
}

// Bits of a little endian bit stream, least significant first, the way
// 0x8197A388 reads the fields.
class BitReader {
 public:
  explicit BitReader(std::span<const uint8_t> data) : data_(data) {}
  uint32_t Read(uint64_t position, uint32_t count) const {
    if (!count) {
      return 0;
    }
    uint64_t value = 0;
    size_t first = size_t(position >> 3);
    uint32_t shift = uint32_t(position & 7);
    size_t bytes = (shift + count + 7) / 8;
    for (size_t i = 0; i < bytes && first + i < data_.size(); ++i) {
      value |= uint64_t(data_[first + i]) << (8 * i);
    }
    value >>= shift;
    return count >= 32 ? uint32_t(value)
                       : uint32_t(value & ((1u << count) - 1));
  }

 private:
  std::span<const uint8_t> data_;
};

class Stream {
 public:
  explicit Stream(std::span<const uint8_t> data) : data_(data) {}
  bool Take(size_t size, std::span<const uint8_t>* out) {
    if (position_ + size > data_.size()) {
      return false;
    }
    *out = data_.subspan(position_, size);
    position_ += size;
    return true;
  }
  bool Skip(size_t size) {
    if (position_ + size > data_.size()) {
      return false;
    }
    position_ += size;
    return true;
  }
  std::span<const uint8_t> rest() const { return data_.subspan(position_); }
  size_t position() const { return position_; }

 private:
  std::span<const uint8_t> data_;
  size_t position_ = 0;
};

struct StrbBlockView {
  uint8_t type;
  std::span<const uint8_t> body;
};

// Blocks of an STRB file: a 1 byte type, then the size and another field as
// big endian 32 bit values, the body 4 byte aligned after that.
bool ParseStrb(std::span<const uint8_t> file,
               std::vector<StrbBlockView>* blocks) {
  if (file.size() < 0x20 || std::memcmp(file.data(), "STRB", 4) ||
      file[4] != 1 || std::memcmp(file.data() + 6, kStrbFormat, 16) ||
      file[0x16] != 1 || file[0x17] != 4) {
    return false;
  }
  uint32_t alignment = file[0x1A] ? file[0x1A] : 4;
  size_t offset = xe::align<size_t>(0x1D, alignment);
  while (offset < file.size()) {
    if (offset + 9 > file.size()) {
      return false;
    }
    uint8_t type = file[offset];
    uint32_t size = load_be32(&file[offset + 1]);
    size_t body = offset + xe::align<size_t>(9, alignment);
    if (body + size > file.size()) {
      return false;
    }
    blocks->push_back({type, file.subspan(body, size)});
    offset = body + xe::align<size_t>(size, alignment);
  }
  return true;
}

struct AssetDescriptor {
  uint8_t body_type = 0;
  uint32_t asset_type = 0;
  uint32_t category = 0;
  uint32_t unknown = 0;
  uint32_t skinning_version = 0;
};

// 0x8196EE50: the descriptor block, version 1 (13 bytes) or 2 (14 bytes) after
// its version byte.
bool ParseDescriptor(const std::vector<StrbBlockView>& blocks,
                     AssetDescriptor* descriptor) {
  for (const auto& block : blocks) {
    std::span<const uint8_t> fields;
    bool has_version = false;
    if (block.type == kStrbBlockDescriptor && !block.body.empty()) {
      if (block.body[0] != 1 && block.body[0] != 2) {
        return false;
      }
      has_version = block.body[0] == 2;
      fields = block.body.subspan(1);
    } else if (block.type == kStrbBlockDescriptorV1) {
      fields = block.body;
    } else {
      continue;
    }
    if (fields.size() < size_t(has_version ? 0xE : 0xD)) {
      return false;
    }
    BitReader bits(fields);
    descriptor->body_type = uint8_t(bits.Read(0, 8));
    descriptor->asset_type = bits.Read(0x8, 32);
    descriptor->category = bits.Read(0x28, 32);
    descriptor->unknown = bits.Read(0x48, 32);
    descriptor->skinning_version = has_version ? bits.Read(0x68, 8) : 1;
    return true;
  }
  return false;
}

// The compressed blocks are frames of {compressed size, output offset, output
// size} (little endian) and LZX data, with the decoder reset at every frame
// (0x81976888).
bool Decompress(std::span<const uint8_t> block, std::vector<uint8_t>* out) {
  LzxFrameDecoder decoder(0x8000);
  size_t offset = 0;
  while (offset + 12 <= block.size()) {
    uint32_t compressed = xe::load<uint32_t>(&block[offset]);
    uint32_t output_offset = xe::load<uint32_t>(&block[offset + 4]);
    uint32_t output_size = xe::load<uint32_t>(&block[offset + 8]);
    offset += 12;
    if (!compressed || compressed > 0x9800 || !output_size ||
        output_size > 0x8000 || offset + compressed > block.size()) {
      return false;
    }
    if (out->size() < size_t(output_offset) + output_size) {
      out->resize(size_t(output_offset) + output_size);
    }
    if (!decoder.Reset() ||
        !decoder.Decompress(&block[offset], compressed,
                            out->data() + output_offset, output_size)) {
      return false;
    }
    offset += compressed;
  }
  return !out->empty();
}

bool LoadAssetPack() {
  if (asset_pack.loaded) {
    return !asset_pack.toc.empty();
  }
  asset_pack.loaded = true;
  auto entry =
      kernel_state()->file_system()->ResolvePath("game:\\AvatarAssetPack");
  auto host_entry = dynamic_cast<vfs::HostPathEntry*>(entry);
  if (!host_entry) {
    XELOGW(
        "Avatar: game:\\AvatarAssetPack {}; avatars only get the title's own "
        "assets",
        entry ? "is inside a disc image" : "not found");
    return false;
  }
  std::unique_ptr<vfs::Device> device =
      vfs::XContentContainerDevice::CreateContentDevice(
          "", host_entry->host_path());
  if (!device || !device->Initialize()) {
    XELOGE("Avatar: can't open the package {}",
           host_entry->host_path().string());
    return false;
  }
  auto toc_entry = device->ResolvePath("AvatarAssetPack.toc");
  vfs::File* file = nullptr;
  if (!toc_entry ||
      toc_entry->Open(vfs::FileAccess::kFileReadData, &file) !=
          X_STATUS_SUCCESS) {
    XELOGE("Avatar: AvatarAssetPack.toc missing from the package");
    return false;
  }
  std::vector<uint8_t> toc(toc_entry->size());
  size_t read = 0;
  X_STATUS status = file->ReadSync(toc, 0, &read);
  file->Destroy();
  if (status != X_STATUS_SUCCESS || read != toc.size() || toc.size() < 0x28) {
    XELOGE("Avatar: can't read AvatarAssetPack.toc");
    return false;
  }
  // Version 2 of the table: 0x28 byte header, entries of 0xF8 bytes.
  uint32_t count = load_be32(&toc[0x1C]);
  if (0x28 + size_t(count) * 0xF8 > toc.size()) {
    XELOGE("Avatar: AvatarAssetPack.toc has a bad entry count {}", count);
    return false;
  }
  asset_pack.toc = std::move(toc);
  asset_pack.entry_count = count;
  XELOGI("Avatar: AvatarAssetPack has {} assets", count);
  return true;
}

// The STRB file of an asset: stock assets from the pack, titles' own assets
// from guest memory (their IDs carry the address, see SetCustomAsset).
std::optional<std::span<const uint8_t>> FindAsset(const uint8_t* id) {
  uint32_t source = id[6] & 0xF;
  if (source == 0) {
    if (!LoadAssetPack()) {
      return std::nullopt;
    }
    // Stock IDs are {component mask, entry index, body type} and the stock
    // GUID; the entry's own mask has to agree.
    const auto& toc = asset_pack.toc;
    uint32_t index = xe::load_and_swap<uint16_t>(id + 4);
    if (index >= asset_pack.entry_count) {
      return std::nullopt;
    }
    const uint8_t* entry = &toc[0x28 + size_t(index) * 0xF8];
    if (load_be32(entry) != load_be32(id)) {
      return std::nullopt;
    }
    uint32_t start = load_be32(entry + 0xF0);
    uint32_t size = load_be32(entry + 0xF4);
    if (!size || size_t(start) + size > toc.size()) {
      return std::nullopt;
    }
    return std::span<const uint8_t>(&toc[start], size);
  }
  if (source == 0xF) {
    uint32_t size = load_be32(id + 8) & 0x00FFFFFF;
    uint32_t address = load_be32(id + 12);
    if (!address || !size) {
      return std::nullopt;
    }
    return std::span<const uint8_t>(TranslateVirtual(address), size);
  }
  // Marketplace and downloaded assets aren't available.
  return std::nullopt;
}

struct Color {
  float rgba[4];
  uint32_t id;
};

// D3DCOLOR to the float4 vupkd3d and the 1/255 scale give.
Color UnpackColor(uint32_t argb, uint32_t id) {
  return {{float((argb >> 16) & 0xFF) / 255.0f,
           float((argb >> 8) & 0xFF) / 255.0f, float(argb & 0xFF) / 255.0f,
           float(argb >> 24) / 255.0f},
          id};
}

struct QueuedPart {
  std::array<uint8_t, kPartRecordSize> record;
  uint32_t kind;
  std::vector<Color> colors;
};

// The result and GPU buffers, filled front to back (0x81966298, 0x819662F0).
struct Buffers {
  uint32_t result_base, result_size, result_next;
  uint32_t gpu_base, gpu_size, gpu_next;

  uint32_t Allocate(bool gpu, uint32_t size, uint32_t alignment) {
    uint32_t& next = gpu ? gpu_next : result_next;
    uint32_t base = gpu ? gpu_base : result_base;
    uint32_t total = gpu ? gpu_size : result_size;
    uint32_t offset = xe::align(next - base, alignment);
    if (offset + size > total) {
      return 0;
    }
    next = base + offset + size;
    return base + offset;
  }
};

void StoreFloat(uint8_t* p, float value) {
  xe::store_and_swap<float>(p, value);
}
void StoreU32(uint8_t* p, uint32_t value) {
  xe::store_and_swap<uint32_t>(p, value);
}
void StoreU16(uint8_t* p, uint16_t value) {
  xe::store_and_swap<uint16_t>(p, value);
}

// 0x819747D8 and the vertex and index codecs below it.
bool DecodeMesh(Stream& stream, uint8_t* record, uint32_t gpu_base,
                const std::array<uint32_t, 12>& header) {
  std::span<const uint8_t> bytes;
  if (!stream.Take(0x21, &bytes)) {
    return false;
  }
  BitReader bits(bytes);
  uint32_t subsets = bits.Read(0x20, 5);
  uint32_t triangles = bits.Read(0x25, 32);
  uint32_t vertex_count = bits.Read(0x45, 32);
  uint32_t vertex_type = bits.Read(0x65, 32);
  uint32_t stride = bits.Read(0x85, 32);
  uint32_t vertex_offset = bits.Read(0xC5, 32);
  uint32_t index_offset = bits.Read(0xE5, 32);
  if (subsets > kMeshMaxSubsets || vertex_type < 1 || vertex_type > 7 ||
      stride != 0x1C + 4 * vertex_type ||
      size_t(vertex_offset) + size_t(vertex_count) * stride > header[1] ||
      size_t(index_offset) + size_t(triangles) * 6 > header[1]) {
    XELOGE("Avatar: bad mesh header (type {}, stride {:X})", vertex_type,
           stride);
    return false;
  }
  std::memset(record, 0, kMeshRecordSize);
  StoreU32(record + 0x0, bits.Read(0, 32));
  StoreU32(record + 0x1E4, triangles);
  StoreU32(record + 0x1E8, vertex_count);
  StoreU32(record + 0x1EC, vertex_type);
  StoreU32(record + 0x1F0, stride);
  StoreU32(record + 0x1F4, bits.Read(0xA5, 32));
  StoreU32(record + 0x1F8, gpu_base + vertex_offset);
  StoreU32(record + 0x1FC, gpu_base + index_offset);

  // 0x81971408: material subsets, index ranges (type 1) or colors.
  for (uint32_t i = 0; i < subsets; ++i) {
    if (!stream.Take(0x18, &bytes)) {
      return false;
    }
    BitReader subset(bytes);
    uint8_t* out = record + 4 + i * 0x18;
    uint32_t type = subset.Read(0, 32);
    StoreU32(out, type);
    StoreU32(out + 4, subset.Read(0x20, 32));
    if (type == 1) {
      StoreU16(out + 8, uint16_t(subset.Read(0x40, 16)));
      StoreU16(out + 0xA, uint16_t(subset.Read(0x50, 16)));
      StoreU32(out + 0xC, subset.Read(0x60, 32));
    } else {
      for (uint32_t j = 0; j < 4; ++j) {
        StoreU32(out + 8 + j * 4, subset.Read(0x40 + j * 0x20, 32));
      }
    }
  }

  // Vertices: a 0x1B2 bit header (count, scale, origin, the widths of the
  // quantized position and the {bits, bias} of four packed fields), then
  // every vertex: position, the four fields, and 2 * type 16 bit values.
  std::span<const uint8_t> rest = stream.rest();
  if (rest.size() < 0x37) {
    return false;
  }
  BitReader vh(rest);
  uint32_t count = vh.Read(0, 32);
  float scale;
  uint32_t scale_bits = vh.Read(0x20, 32);
  std::memcpy(&scale, &scale_bits, 4);
  float origin[3];
  for (uint32_t i = 0; i < 3; ++i) {
    uint32_t v = vh.Read(0x40 + 0x20 * i, 32);
    std::memcpy(&origin[i], &v, 4);
  }
  uint32_t width[3] = {vh.Read(0xA0, 6), vh.Read(0xA6, 6), vh.Read(0xAC, 6)};
  struct Field {
    uint32_t bits, bias;
  } fields[4] = {{vh.Read(0xD2, 32), vh.Read(0xB2, 32)},
                 {vh.Read(0x112, 32), vh.Read(0xF2, 32)},
                 {vh.Read(0x152, 32), vh.Read(0x132, 32)},
                 {vh.Read(0x192, 32), vh.Read(0x172, 32)}};
  uint64_t vertex_bits = uint64_t(width[0]) + width[1] + width[2] +
                         0x20 * vertex_type;
  for (const auto& field : fields) {
    if (field.bits > 32) {
      return false;
    }
    vertex_bits += field.bits;
  }
  size_t vertex_bytes = size_t((count * vertex_bits + 0x1B9) >> 3);
  if (count != vertex_count || vertex_bytes > rest.size()) {
    XELOGE("Avatar: vertex stream doesn't match its mesh");
    return false;
  }
  float step[3];
  for (uint32_t i = 0; i < 3; ++i) {
    step[i] = scale * kLatticeStep[i];
  }
  BitReader vb(rest);
  uint8_t* vertices = TranslateVirtual(gpu_base + vertex_offset);
  uint64_t position = 0x1B2;
  for (uint32_t v = 0; v < count; ++v) {
    uint8_t* out = vertices + size_t(v) * stride;
    uint64_t p = position;
    uint32_t q[3];
    for (uint32_t i = 0; i < 3; ++i) {
      q[i] = vb.Read(p, width[i]);
      p += width[i];
    }
    // 0x81970FA0.
    float y = float(q[1]) * step[1] + origin[1];
    float z = float(q[2]) * step[2] + origin[2];
    if (q[1] & 1) {
      z += step[2] * kLatticeOddLayerShift;
    }
    float x = float(q[0]) * step[0] + origin[0];
    if ((q[1] ^ q[2]) & 1) {
      x += step[0] * kLatticeOddRowShift;
    }
    uint32_t packed[4];
    for (uint32_t i = 0; i < 4; ++i) {
      packed[i] = vb.Read(p, fields[i].bits) + fields[i].bias;
      p += fields[i].bits;
    }
    if (coordinate_mode != 1) {
      // Flip the handedness: z and the top 10 bit field of the normal.
      z = -z;
      packed[0] = (uint32_t(-int32_t(packed[0] >> 22)) << 22) |
                  (packed[0] & 0x3FFFFF);
    }
    StoreFloat(out + 0, x);
    StoreFloat(out + 4, y);
    StoreFloat(out + 8, z);
    for (uint32_t i = 0; i < 4; ++i) {
      StoreU32(out + 0xC + i * 4, packed[i]);
    }
    for (uint32_t i = 0; i < 2 * vertex_type; ++i) {
      StoreU16(out + 0x1C + i * 2, uint16_t(vb.Read(p, 16)));
      p += 16;
    }
    position += vertex_bits;
  }
  stream.Skip(vertex_bytes);

  // Indices: {count, bias16, bits16} and count values of bits + bias.
  rest = stream.rest();
  if (rest.size() < 8) {
    return false;
  }
  BitReader ib(rest);
  uint32_t index_count = ib.Read(0, 32);
  uint32_t index_bias = ib.Read(0x20, 16);
  uint32_t index_width = ib.Read(0x30, 16);
  size_t index_bytes =
      size_t((uint64_t(index_count) * index_width + 0x47) >> 3);
  if (index_count != triangles * 3 || index_width > 16 ||
      index_bytes > rest.size()) {
    XELOGE("Avatar: index stream doesn't match its mesh");
    return false;
  }
  uint8_t* indices = TranslateVirtual(gpu_base + index_offset);
  for (uint32_t i = 0; i < index_count; ++i) {
    StoreU16(indices + i * 2,
             uint16_t(ib.Read(0x40 + uint64_t(i) * index_width, index_width) +
                      index_bias));
  }
  stream.Skip(index_bytes);
  return true;
}

// 0x81978680: rows of texture blocks, tiled here when the asset asks for it.
bool DecodeTexture(Stream& stream, uint8_t* record, uint32_t gpu_base) {
  std::span<const uint8_t> bytes;
  if (!stream.Take(8, &bytes)) {
    return false;
  }
  BitReader place(bytes);
  uint32_t data_offset = place.Read(0, 32);
  uint32_t data_size = place.Read(0x20, 32);
  if (!stream.Take(0x21, &bytes)) {
    return false;
  }
  BitReader bits(bytes);
  uint32_t format = bits.Read(0, 32);
  uint32_t width = bits.Read(0x20, 32);
  uint32_t height = bits.Read(0x40, 32);
  uint32_t size = bits.Read(0x60, 32);
  uint32_t slice_size = bits.Read(0x80, 32);
  uint32_t slices = bits.Read(0xA0, 32);
  bool empty = bits.Read(0xC0, 1);
  bool tile = bits.Read(0xC1, 1);
  uint32_t row_bytes = bits.Read(0xC2, 32);
  uint32_t rows = bits.Read(0xE2, 32);

  uint32_t data = gpu_base + data_offset;
  std::memset(record, 0, kTextureRecordSize);
  StoreU32(record + 0x00, format);
  StoreU32(record + 0x04, width);
  StoreU32(record + 0x08, height);
  StoreU32(record + 0x0C, size);
  StoreU32(record + 0x14, slice_size);
  StoreU32(record + 0x1C, 1);
  StoreU32(record + 0x20, slices);
  StoreU32(record + 0x24, data);
  if (size > data_size) {
    return false;
  }
  uint8_t* out = TranslateVirtual(data);
  if (empty) {
    std::memset(out, 0, size_t(slice_size) * std::max(slices, 1u));
    return true;
  }
  // Only the DXT formats (8 or 16 bytes per 4x4 block) are in the pack.
  uint32_t block_bytes = (format & 0x3F) == 0x12 ? 8 : 16;
  uint32_t blocks_wide = row_bytes / block_bytes;
  uint32_t pitch_blocks = xe::align(blocks_wide, 32u);
  uint32_t pitch = pitch_blocks * block_bytes;
  if (size_t(pitch) * rows > slice_size ||
      size_t(slices - 1) * slice_size + size_t(pitch) * rows > data_size) {
    XELOGE("Avatar: texture {}x{} doesn't fit its slices", width, height);
    return false;
  }
  for (uint32_t s = 0; s < slices; ++s) {
    uint8_t* slice = out + size_t(s) * slice_size;
    for (uint32_t r = 0; r < rows; ++r) {
      if (!stream.Take(row_bytes, &bytes)) {
        return false;
      }
      if (!tile) {
        std::memcpy(slice + size_t(r) * pitch, bytes.data(), row_bytes);
        continue;
      }
      uint32_t log2_bytes = block_bytes == 8 ? 3 : 4;
      for (uint32_t b = 0; b < blocks_wide; ++b) {
        int32_t offset = gpu::texture_address::Tiled2D(
            int32_t(b), int32_t(r), pitch_blocks, log2_bytes);
        if (offset < 0 || uint32_t(offset) + block_bytes > slice_size) {
          return false;
        }
        std::memcpy(slice + offset, bytes.data() + size_t(b) * block_bytes,
                    block_bytes);
      }
    }
  }
  return true;
}

// 0x81971CA8: vertices shared by seams get the position and the 8 bytes at
// +0x10 of another vertex.
bool ApplySeams(Stream& stream, uint32_t vertex_base) {
  uint8_t* base = TranslateVirtual(vertex_base);
  while (true) {
    std::span<const uint8_t> bytes;
    if (!stream.Take(0xC, &bytes)) {
      return false;
    }
    BitReader header(bytes);
    uint32_t count = header.Read(0, 32);
    uint32_t bias = header.Read(0x20, 32);
    uint32_t width = header.Read(0x40, 32);
    if (width > 32) {
      return false;
    }
    size_t body = size_t((uint64_t(count) * width + 0x67) >> 3) - 0xC;
    std::span<const uint8_t> rest = stream.rest();
    if (body > rest.size()) {
      return false;
    }
    if (count & 1) {
      stream.Skip(body);
      return true;
    }
    BitReader values(rest);
    for (uint32_t i = 0; i + 1 < count; i += 2) {
      uint32_t from = values.Read(uint64_t(i) * width, width) + bias;
      uint32_t to = values.Read(uint64_t(i + 1) * width, width) + bias;
      std::memmove(base + to, base + from, 12);
      std::memmove(base + to + 0x10, base + from + 0x10, 8);
    }
    stream.Skip(body);
  }
}

// 0x819709E0: the subsets of every mesh with the color's id take its value.
void PaintColor(uint8_t* component, const Color& color) {
  uint32_t meshes = load_be32(component + 0x14);
  uint8_t* records = TranslateVirtual(load_be32(component + 0x2C));
  for (uint32_t m = 0; m < meshes; ++m) {
    for (uint32_t s = 0; s < kMeshMaxSubsets; ++s) {
      uint8_t* subset = records + m * kMeshRecordSize + 4 + s * 0x18;
      if (load_be32(subset + 4) == color.id) {
        for (uint32_t i = 0; i < 4; ++i) {
          StoreFloat(subset + 8 + i * 4, color.rgba[i]);
        }
      }
    }
  }
}

// 0x81966450 and 0x81974A90: one component's meshes and textures.
bool BuildComponent(const QueuedPart& part,
                    const std::vector<StrbBlockView>& blocks,
                    uint8_t* component, Buffers& buffers) {
  uint16_t mask = xe::load_and_swap<uint16_t>(&part.record[0x10]);
  uint32_t result_room = 0, gpu_room = 0;
  for (uint32_t i = 0; i < xe::countof(kComponentSizes); ++i) {
    if (mask & (1u << i)) {
      result_room += kComponentSizes[i].header;
      gpu_room += kComponentSizes[i].gpu_resources;
    }
  }
  uint32_t result = buffers.Allocate(false, result_room, 0x10);
  uint32_t gpu = buffers.Allocate(true, gpu_room, 0x1000);
  if (!result || !gpu) {
    XELOGE("Avatar: no room for the component {:04X}", mask);
    return false;
  }

  const StrbBlockView* model = nullptr;
  for (const auto& block : blocks) {
    if (block.type == kStrbBlockModel) {
      model = &block;
      break;
    }
  }
  std::vector<uint8_t> data;
  if (!model || !Decompress(model->body, &data)) {
    XELOGE("Avatar: the component {:04X} has no readable model", mask);
    return false;
  }
  Stream stream(data);
  std::span<const uint8_t> bytes;
  if (!stream.Take(0x30, &bytes)) {
    return false;
  }
  std::array<uint32_t, 12> header;
  for (uint32_t i = 0; i < 12; ++i) {
    header[i] = xe::load<uint32_t>(&bytes[i * 4]);
  }
  if (header[0] > result_room || header[1] > gpu_room ||
      header[9] + header[5] * kMeshRecordSize > header[0] ||
      header[10] + header[6] * kTextureRecordSize > header[0]) {
    XELOGE("Avatar: the component {:04X} doesn't fit ({:X}/{:X})", mask,
           header[0], header[1]);
    return false;
  }
  for (uint32_t i = 0; i < 7; ++i) {
    StoreU32(component + i * 4, header[i]);
  }
  StoreU32(component + 0x1C, result);
  StoreU32(component + 0x20, gpu);
  StoreU32(component + 0x24, gpu + header[7]);
  StoreU32(component + 0x28, gpu + header[8]);
  StoreU32(component + 0x2C, result + header[9]);
  StoreU32(component + 0x30, result + header[10]);

  uint8_t* meshes = TranslateVirtual(result + header[9]);
  for (uint32_t i = 0; i < header[5]; ++i) {
    if (!DecodeMesh(stream, meshes + i * kMeshRecordSize, gpu, header)) {
      return false;
    }
  }
  uint8_t* textures = TranslateVirtual(result + header[10]);
  for (uint32_t i = 0; i < header[6]; ++i) {
    if (!DecodeTexture(stream, textures + i * kTextureRecordSize, gpu)) {
      XELOGE("Avatar: bad texture {} of the component {:04X}", i, mask);
      return false;
    }
  }
  if (!ApplySeams(stream, gpu + header[7])) {
    XELOGE("Avatar: bad seams in the component {:04X}", mask);
    return false;
  }
  for (const auto& color : part.colors) {
    PaintColor(component, color);
  }
  return true;
}

uint32_t ManifestColor(const uint8_t* manifest, uint32_t index) {
  return load_be32(manifest + kManifestColors + index * 4);
}

uint32_t CountManifestParts(const uint8_t* manifest) {
  uint32_t count = 0;
  for (uint32_t o = kManifestParts; o < kManifestPartsEnd; o += 0x20) {
    if (IsNullId(manifest + o)) {
      break;
    }
    ++count;
  }
  return count;
}

// 0x81968D88, 0x81968AD8 and 0x81968920: the parts asked for, in the order
// xam builds them, with the colors each one gets.
std::vector<QueuedPart> QueueParts(const uint8_t* manifest, uint16_t mask) {
  std::vector<QueuedPart> parts;
  auto record_mask = [&](uint32_t offset) {
    return xe::load_and_swap<uint16_t>(manifest + offset + 0x10);
  };
  Color skin = UnpackColor(ManifestColor(manifest, 0), 0x1B);
  if (record_mask(kManifestBodyA) & mask) {
    QueuedPart part{};
    std::memcpy(part.record.data(), manifest + kManifestBodyA, 0x20);
    part.kind = 1;
    part.colors = {UnpackColor(ManifestColor(manifest, 0), 0x16), skin};
    parts.push_back(part);
  }
  bool body_b = record_mask(kManifestBodyB) & mask;
  if (body_b) {
    QueuedPart part{};
    std::memcpy(part.record.data(), manifest + kManifestBodyB, 0x20);
    part.kind = 1;
    for (uint32_t i = 0; i < 9; ++i) {
      part.colors.push_back(UnpackColor(ManifestColor(manifest, i), 0xD + i));
    }
    part.colors.push_back(skin);
    parts.push_back(part);
  }
  uint32_t count = CountManifestParts(manifest);
  for (uint32_t i = 0; i < count; ++i) {
    uint32_t offset = kManifestParts + i * 0x20;
    if (!(record_mask(offset) & mask)) {
      continue;
    }
    QueuedPart part{};
    std::memcpy(part.record.data(), manifest + offset, 0x20);
    part.kind = 1;
    for (uint32_t c = 0; c < 3; ++c) {
      part.colors.push_back(
          UnpackColor(load_be32(manifest + offset + 0x14 + c * 4), 0x16 + c));
    }
    part.colors.push_back(skin);
    parts.push_back(part);
  }
  if (body_b) {
    for (uint32_t i = 0; i < 6; ++i) {
      const uint8_t* record = manifest + kManifestTextures + i * 0x20;
      if (!IsNullId(record)) {
        QueuedPart part{};
        std::memcpy(part.record.data(), record, 0x20);
        part.kind = 2;
        parts.push_back(part);
      }
    }
    for (uint32_t i = 0; i < 3; ++i) {
      const uint8_t* id = manifest + kManifestShapes + i * 0x10;
      if (!IsNullId(id)) {
        QueuedPart part{};
        std::memcpy(part.record.data(), id, 0x10);
        part.kind = 3;
        parts.push_back(part);
      }
    }
  }
  return parts;
}

}  // namespace

void SetInitializeParameters(uint32_t version, uint32_t mode) {
  std::lock_guard<std::mutex> lock(state_mutex);
  skinning_version = 2 - (version < 3 ? 1 : 0);
  coordinate_mode = mode;
}

void GetAssetsResultSize(uint16_t component_mask, uint32_t* result_size,
                         uint32_t* gpu_resource_size) {
  *result_size = (component_mask & kBodyComponent) ? kResultFixedSizeWithBody
                                                   : kResultFixedSize;
  *gpu_resource_size = 0;
  for (uint32_t i = 0; i < xe::countof(kComponentSizes); ++i) {
    if (component_mask & (1u << i)) {
      *result_size += xe::align(kComponentSizes[i].header, 0x10u);
      *gpu_resource_size +=
          xe::align(kComponentSizes[i].gpu_resources, 0x1000u);
    }
  }
}

X_HRESULT SetCustomAsset(uint32_t buffer_size, uint32_t asset_data_ptr,
                         uint32_t custom_color_count,
                         uint32_t custom_colors_ptr, uint32_t slot_enabled,
                         uint32_t manifest_ptr) {
  if (!asset_data_ptr || !buffer_size || !manifest_ptr) {
    return X_E_FAIL;
  }
  std::span<const uint8_t> file(TranslateVirtual(asset_data_ptr), buffer_size);
  std::vector<StrbBlockView> blocks;
  AssetDescriptor descriptor;
  if (!ParseStrb(file, &blocks) || !ParseDescriptor(blocks, &descriptor)) {
    XELOGW("Avatar: SetCustomAsset got an asset it can't read");
    return X_E_FAIL;
  }
  {
    std::lock_guard<std::mutex> lock(state_mutex);
    if (descriptor.skinning_version != skinning_version) {
      return X_E_FAIL;
    }
  }
  uint8_t* manifest = TranslateVirtual(manifest_ptr);

  // The ID of a title asset (0x81965B98): the category, source 0xF, the body
  // type, and the size and address of the data.
  uint8_t id[16] = {};
  xe::store_and_swap<uint32_t>(id, descriptor.category);
  id[6] = 0x0F;
  id[7] = descriptor.body_type & 0xF;
  xe::store_and_swap<uint32_t>(id + 8, buffer_size);
  id[8] = 0xC0;
  xe::store_and_swap<uint32_t>(id + 12, asset_data_ptr);

  switch (descriptor.asset_type) {
    case 1: {
      uint8_t record[kPartRecordSize] = {};
      std::memcpy(record, id, 16);
      uint16_t mask = uint16_t(descriptor.category);
      xe::store_and_swap<uint16_t>(record + 0x10, mask);
      uint32_t colors[3] = {0xFFFF0000, 0xFF00FF00, 0xFF0000FF};
      for (uint32_t i = 0; i < std::min(custom_color_count, 3u); ++i) {
        colors[i] = load_be32(TranslateVirtual(custom_colors_ptr) + i * 4);
      }
      for (uint32_t i = 0; i < 3; ++i) {
        xe::store_and_swap<uint32_t>(record + 0x14 + i * 4, colors[i]);
      }
      // 0x8196D1B0: keep it as the default of a required category, drop the
      // parts it covers, add it, and bring back the defaults left uncovered.
      for (uint32_t k = 0; k < xe::countof(kRequiredCategories); ++k) {
        if (kRequiredCategories[k] == mask) {
          std::memcpy(manifest + kManifestDefaultParts + k * 0x20, record,
                      kPartRecordSize);
        }
      }
      uint32_t count = CountManifestParts(manifest);
      for (int32_t i = int32_t(count) - 1; i >= 0; --i) {
        uint8_t* part = manifest + kManifestParts + i * 0x20;
        if (xe::load_and_swap<uint16_t>(part + 0x10) & mask) {
          std::memmove(part, part + 0x20, (count - 1 - i) * 0x20);
          std::memset(manifest + kManifestParts + (count - 1) * 0x20, 0, 0x20);
          --count;
        }
      }
      auto add = [&](const uint8_t* part) {
        uint32_t n = CountManifestParts(manifest);
        if (n < kMaxParts) {
          std::memcpy(manifest + kManifestParts + n * 0x20, part,
                      kPartRecordSize);
        }
      };
      add(record);
      uint16_t covered = 0;
      count = CountManifestParts(manifest);
      for (uint32_t i = 0; i < count; ++i) {
        covered |= xe::load_and_swap<uint16_t>(manifest + kManifestParts +
                                               i * 0x20 + 0x10);
      }
      for (uint32_t k = 0; k < xe::countof(kRequiredCategories); ++k) {
        const uint8_t* fallback = manifest + kManifestDefaultParts + k * 0x20;
        if (!(covered & kRequiredCategories[k]) && !IsNullId(fallback)) {
          add(fallback);
        }
      }
      return X_E_SUCCESS;
    }
    case 2: {
      static const uint32_t kTextureMasks[] = {0x8000,  0x2000,  0x4000,
                                               0x10000, 0x40000, 0x20000};
      if (!slot_enabled || descriptor.category >= 6) {
        return X_E_FAIL;
      }
      xe::store_and_swap<uint32_t>(id, kTextureMasks[descriptor.category]);
      uint8_t* record =
          manifest + kManifestTextures + descriptor.category * 0x20;
      std::memcpy(record, id, 16);
      std::memset(record + 16, 0, 16);
      return X_E_SUCCESS;
    }
    case 3: {
      static const uint32_t kShapeMasks[] = {0x100000, 0x80000, 0x200000};
      if (!slot_enabled || descriptor.category >= 3) {
        return X_E_FAIL;
      }
      xe::store_and_swap<uint32_t>(id, kShapeMasks[descriptor.category]);
      std::memcpy(manifest + kManifestShapes + descriptor.category * 0x10, id,
                  16);
      return X_E_SUCCESS;
    }
  }
  return X_E_FAIL;
}

X_HRESULT GetAssets(const uint8_t* manifest, uint16_t component_mask,
                    uint32_t flags, uint32_t result_buffer_ptr,
                    uint32_t gpu_resource_buffer_ptr) {
  if (!result_buffer_ptr || !gpu_resource_buffer_ptr) {
    return X_E_INVALIDARG;
  }
  if (component_mask & kBodyComponent) {
    // The body brings the skeleton and animations (0x81966C98), not done yet.
    XELOGW("Avatar: GetAssets for the body isn't implemented");
    return X_E_FAIL;
  }
  Buffers buffers{};
  GetAssetsResultSize(component_mask, &buffers.result_size,
                      &buffers.gpu_size);
  buffers.result_base = result_buffer_ptr;
  buffers.gpu_base = gpu_resource_buffer_ptr;
  // 0x81967628: the header, then the skeleton block, the part records and
  // the component tables.
  buffers.result_next = result_buffer_ptr + 0x14;
  buffers.gpu_next = gpu_resource_buffer_ptr;
  uint8_t* header = TranslateVirtual(result_buffer_ptr);
  std::memset(header, 0, 0x14);
  uint32_t skeleton = buffers.Allocate(false, 0x1B10, 0x10);
  uint32_t records = buffers.Allocate(false, kMaxParts * kPartRecordSize, 4);
  uint32_t components = buffers.Allocate(false, kMaxParts * kComponentSize, 4);
  if (!skeleton || !records || !components) {
    return X_E_FAIL;
  }
  std::memset(TranslateVirtual(skeleton), 0, 0x1B10);
  StoreU32(TranslateVirtual(skeleton) + 4, skeleton + 0x10);
  std::memset(TranslateVirtual(records), 0, kMaxParts * kPartRecordSize);
  std::memset(TranslateVirtual(components), 0, kMaxParts * kComponentSize);
  StoreU32(header + 0x0, skeleton);
  StoreU32(header + 0xC, records);
  StoreU32(header + 0x10, components);

  uint32_t built = 0;
  for (const auto& part : QueueParts(manifest, component_mask)) {
    auto file = FindAsset(part.record.data());
    if (!file) {
      XELOGW("Avatar: asset {:016X}{:016X} not available",
             xe::load_and_swap<uint64_t>(&part.record[0]),
             xe::load_and_swap<uint64_t>(&part.record[8]));
      continue;
    }
    std::vector<StrbBlockView> blocks;
    AssetDescriptor descriptor;
    if (!ParseStrb(*file, &blocks) || !ParseDescriptor(blocks, &descriptor)) {
      XELOGW("Avatar: unreadable asset {:016X}",
             xe::load_and_swap<uint64_t>(&part.record[0]));
      continue;
    }
    if (part.kind != 1) {
      // Face textures and shapes (0x819672F8) aren't built yet.
      XELOGD("Avatar: skipping a kind {} asset", part.kind);
      continue;
    }
    if (built >= kMaxParts) {
      break;
    }
    uint8_t* component = TranslateVirtual(components) + built * kComponentSize;
    if (!BuildComponent(part, blocks, component, buffers)) {
      std::memset(component, 0, kComponentSize);
      continue;
    }
    std::memcpy(TranslateVirtual(records) + built * kPartRecordSize,
                part.record.data(), kPartRecordSize);
    ++built;
    StoreU32(header + 0x8, built);
    XELOGI("Avatar: built component {:04X} from {:016X}",
           xe::load_and_swap<uint16_t>(&part.record[0x10]),
           xe::load_and_swap<uint64_t>(&part.record[0]));
  }
  return built ? X_E_SUCCESS : X_E_FAIL;
}

}  // namespace avatar
}  // namespace xam
}  // namespace kernel
}  // namespace xe
