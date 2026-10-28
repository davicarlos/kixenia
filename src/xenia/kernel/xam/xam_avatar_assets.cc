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
#include <filesystem>
#include <fstream>
#include <mutex>
#include <optional>
#include <random>
#include <span>
#include <vector>

#include "xenia/base/byte_order.h"
#include "xenia/base/cvar.h"
#include "xenia/base/logging.h"
#include "xenia/base/math.h"
#include "xenia/cpu/lzx.h"
#include "xenia/gpu/texture_address.h"
#include "xenia/kernel/kernel_state.h"
#include "xenia/kernel/util/shim_utils.h"
#include "xenia/vfs/devices/host_path_entry.h"
#include "xenia/vfs/devices/xcontent_container_device.h"

DEFINE_path(avatar_asset_package, "",
            "The Xbox 360 system's avatar asset package "
            "($SystemUpdate/FFFE07DF00000002 of a system update), used for "
            "avatars instead of the AvatarAssetPack titles ship. It's what a "
            "console uses and has more parts (props, animations).",
            "Kernel");

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
// 15.0f, what 0x81967540 puts at +0x5968 of the animation (0x81613834); its
// use isn't known yet.
constexpr uint32_t kAnimationDefault = 0x41700000;

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
// The stock bodies (0x81632E68 and 0x81632E78).
constexpr uint8_t kStockMaleBody[16] = {0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
                                        0x00, 0x01, 0xC1, 0xC8, 0xF1, 0x09,
                                        0xA1, 0x9C, 0xB2, 0xE0};
constexpr uint8_t kStockFemaleBody[16] = {0x00, 0x00, 0x00, 0x02, 0x00, 0x01,
                                          0x00, 0x02, 0xC1, 0xC8, 0xF1, 0x09,
                                          0xA1, 0x9C, 0xB2, 0xE0};
// Categories a manifest always has a part for: shoes, pants, shirt and one
// more (0x816331A0).
constexpr uint16_t kRequiredCategories[] = {0x20, 0x10, 0x8, 0x4};

constexpr uint8_t kStrbFormat[16] = {0x59, 0xBD, 0xE0, 0x8B, 0x49, 0x09,
                                     0x4E, 0xE4, 0xB9, 0x91, 0xAD, 0xA8,
                                     0x7B, 0x7C, 0x0B, 0x6B};
enum StrbBlock : uint8_t {
  kStrbBlockFaceTexture = 2,
  kStrbBlockModel = 3,
  kStrbBlockShape = 4,
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
// The title's threads and the Avatar Viewer both load the pack.
std::mutex pack_mutex;
uint32_t skinning_version = 2;
uint32_t coordinate_mode = 1;

struct AssetPack {
  bool loaded = false;
  std::vector<uint8_t> toc;
  uint32_t entry_count = 0;
  // The table's layout by its version: header size, entry size and where
  // in an entry its data's start and size are.
  uint32_t header_size = 0x28;
  uint32_t entry_size = 0xF8;
  uint32_t data_field = 0xF0;
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

// Versions of AvatarAssetPack.toc: 2 in the packs titles ship, 3 in the
// system's (0x8197E238 reads both).
constexpr uint8_t kTocVersion2[16] = {0x8A, 0x76, 0x2D, 0xF4, 0xC0, 0x67,
                                      0x4B, 0x66, 0xB4, 0xEE, 0x56, 0x46,
                                      0x6B, 0x1B, 0x82, 0x80};
constexpr uint8_t kTocVersion3[16] = {0x58, 0x0A, 0x07, 0xD6, 0x4B, 0xCD,
                                      0x40, 0xBE, 0xBF, 0x37, 0x25, 0x4B,
                                      0xE8, 0x26, 0xFB, 0x0B};

const uint8_t* PackEntry(uint32_t index) {
  return &asset_pack.toc[asset_pack.header_size +
                         size_t(index) * asset_pack.entry_size];
}
uint32_t PackDataStart(const uint8_t* entry) {
  return load_be32(entry + asset_pack.data_field);
}
uint32_t PackDataSize(const uint8_t* entry) {
  return load_be32(entry + asset_pack.data_field + 4);
}

// A host file to open the package from: the configured system package, or
// the title's AvatarAssetPack, copied out first when it's inside a disc image.
std::filesystem::path FindPackageFile() {
  if (!cvars::avatar_asset_package.empty()) {
    return cvars::avatar_asset_package;
  }
  auto entry =
      kernel_state()->file_system()->ResolvePath("game:\\AvatarAssetPack");
  if (!entry) {
    return {};
  }
  if (auto host_entry = dynamic_cast<vfs::HostPathEntry*>(entry)) {
    return host_entry->host_path();
  }
  vfs::File* file = nullptr;
  if (entry->Open(vfs::FileAccess::kFileReadData, &file) != X_STATUS_SUCCESS) {
    return {};
  }
  std::vector<uint8_t> data(entry->size());
  size_t read = 0;
  X_STATUS status = file->ReadSync(data, 0, &read);
  file->Destroy();
  if (status != X_STATUS_SUCCESS || read != data.size()) {
    return {};
  }
  std::filesystem::path copy =
      std::filesystem::temp_directory_path() /
      fmt::format("xenia-avatar-asset-pack-{:08X}", kernel_state()->title_id());
  std::ofstream out(copy, std::ios::binary | std::ios::trunc);
  out.write(reinterpret_cast<const char*>(data.data()), data.size());
  return out.good() ? copy : std::filesystem::path();
}

bool LoadAssetPack() {
  std::lock_guard<std::mutex> lock(pack_mutex);
  if (asset_pack.loaded) {
    return !asset_pack.toc.empty();
  }
  asset_pack.loaded = true;
  std::filesystem::path package = FindPackageFile();
  if (package.empty()) {
    XELOGW(
        "Avatar: no avatar asset package (set avatar_asset_package or use a "
        "title that ships AvatarAssetPack); avatars only get the title's own "
        "assets");
    return false;
  }
  std::unique_ptr<vfs::Device> device =
      vfs::XContentContainerDevice::CreateContentDevice("", package);
  if (!device || !device->Initialize()) {
    XELOGE("Avatar: can't open the avatar asset package");
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
  if (status != X_STATUS_SUCCESS || read != toc.size() || toc.size() < 0x30) {
    XELOGE("Avatar: can't read AvatarAssetPack.toc");
    return false;
  }
  if (!std::memcmp(toc.data(), kTocVersion3, 16)) {
    asset_pack.header_size = 0x30;
    asset_pack.entry_size = 0x10C;
    asset_pack.data_field = 0x104;
  } else if (std::memcmp(toc.data(), kTocVersion2, 16)) {
    XELOGE("Avatar: AvatarAssetPack.toc of an unknown version");
    return false;
  }
  uint32_t count = load_be32(&toc[0x1C]);
  if (asset_pack.header_size + size_t(count) * asset_pack.entry_size >
      toc.size()) {
    XELOGE("Avatar: AvatarAssetPack.toc has a bad entry count {}", count);
    return false;
  }
  asset_pack.toc = std::move(toc);
  asset_pack.entry_count = count;
  XELOGI("Avatar: AvatarAssetPack.toc version {} has {} assets",
         asset_pack.header_size == 0x30 ? 3 : 2, count);
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
    const uint8_t* entry = PackEntry(index);
    if (load_be32(entry) != load_be32(id)) {
      return std::nullopt;
    }
    uint32_t start = PackDataStart(entry);
    uint32_t size = PackDataSize(entry);
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
  // The manifest slot of a face texture (kind 2) or shape (kind 3).
  uint32_t slot = 0;
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

// 0x81978680: a texture header and its rows of blocks, written over a texture
// record that already says where its data goes (+0x24) and how much room it
// has (+0xC). Tiled here when the asset asks for it.
bool FillTexture(Stream& stream, uint8_t* record) {
  std::span<const uint8_t> bytes;
  uint32_t data = load_be32(record + 0x24);
  uint32_t room = load_be32(record + 0x0C);
  if (!data || !stream.Take(0x21, &bytes)) {
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
  if (size > room) {
    XELOGE("Avatar: texture of {:X} bytes for room of {:X}", size, room);
    return false;
  }

  std::memset(record, 0, kTextureRecordSize);
  StoreU32(record + 0x00, format);
  StoreU32(record + 0x04, width);
  StoreU32(record + 0x08, height);
  StoreU32(record + 0x0C, size);
  StoreU32(record + 0x14, slice_size);
  StoreU32(record + 0x1C, 1);
  StoreU32(record + 0x20, slices);
  StoreU32(record + 0x24, data);
  uint8_t* out = TranslateVirtual(data);
  if (empty) {
    std::memset(out, 0, size);
    return true;
  }
  // Only the DXT formats (8 or 16 bytes per 4x4 block) are in the pack.
  uint32_t block_bytes = (format & 0x3F) == 0x12 ? 8 : 16;
  uint32_t blocks_wide = row_bytes / block_bytes;
  uint32_t pitch_blocks = xe::align(blocks_wide, 32u);
  uint32_t pitch = pitch_blocks * block_bytes;
  if (!slices || size_t(pitch) * rows > slice_size ||
      size_t(slices - 1) * slice_size + size_t(pitch) * rows > size) {
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

// A model's texture: where its data goes in the GPU buffer and how much room
// it has (0x81974A90 and 0x81977690), then the texture itself. Textures the
// model only reserves are marked empty; face textures fill them later.
bool DecodeTexture(Stream& stream, uint8_t* record, uint32_t gpu_base) {
  std::span<const uint8_t> bytes;
  if (!stream.Take(8, &bytes)) {
    return false;
  }
  BitReader place(bytes);
  std::memset(record, 0, kTextureRecordSize);
  StoreU32(record + 0x00, 0x1A200154);
  StoreU32(record + 0x0C, place.Read(0x20, 32));
  StoreU32(record + 0x1C, 1);
  StoreU32(record + 0x24, gpu_base + place.Read(0, 32));
  return FillTexture(stream, record);
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

// The material subsets each face texture slot fills (0x81632F08).
constexpr uint32_t kFaceSlotSubsets[6][2] = {
    {0xC, 0}, {0x9, 0xA}, {0x7, 0x8}, {0x6, 0}, {0xB, 0}, {0x5, 0}};

// 0x819671A0: a face texture fills, in the components already built, the
// textures that the subsets of its slot point to (the u16 at +8 of the
// subset). The head reserves them as empty textures of the right size.
bool ApplyFaceTexture(const QueuedPart& part,
                      const std::vector<StrbBlockView>& blocks,
                      uint8_t* components, uint32_t built) {
  if (part.slot >= xe::countof(kFaceSlotSubsets)) {
    return false;
  }
  const StrbBlockView* texture = nullptr;
  for (const auto& block : blocks) {
    if (block.type == kStrbBlockFaceTexture) {
      texture = &block;
      break;
    }
  }
  std::vector<uint8_t> data;
  if (!texture || !Decompress(texture->body, &data)) {
    return false;
  }
  Stream stream(data);
  for (int32_t c = int32_t(built) - 1; c >= 0; --c) {
    const uint8_t* component = components + c * kComponentSize;
    uint32_t meshes = load_be32(component + 0x14);
    uint32_t textures = load_be32(component + 0x18);
    uint8_t* mesh_records = TranslateVirtual(load_be32(component + 0x2C));
    uint8_t* texture_records = TranslateVirtual(load_be32(component + 0x30));
    uint64_t filled = 0;
    for (uint32_t id : kFaceSlotSubsets[part.slot]) {
      if (!id) {
        break;
      }
      for (int32_t m = int32_t(meshes) - 1; m >= 0; --m) {
        for (int32_t s = kMeshMaxSubsets - 1; s >= 0; --s) {
          const uint8_t* subset =
              mesh_records + m * kMeshRecordSize + 4 + s * 0x18;
          if (load_be32(subset + 4) != id) {
            continue;
          }
          uint32_t index = xe::load_and_swap<uint16_t>(subset + 8);
          if (index >= textures || index >= 64 || (filled >> index) & 1) {
            continue;
          }
          filled |= uint64_t(1) << index;
          if (!FillTexture(stream,
                           texture_records + index * kTextureRecordSize)) {
            return false;
          }
        }
      }
    }
  }
  return true;
}

// The ID a block 4 is for, as 0x81979F30 reads it: the mask (32 bits), the
// index and the body type (16 bits each) little endian, then the GUID bytes.
void ReadShapeTarget(const uint8_t* bytes, uint8_t* id) {
  xe::store_and_swap<uint32_t>(id, xe::load<uint32_t>(bytes));
  xe::store_and_swap<uint16_t>(id + 4, xe::load<uint16_t>(bytes + 4));
  xe::store_and_swap<uint16_t>(id + 6, xe::load<uint16_t>(bytes + 6));
  std::memcpy(id + 8, bytes + 8, 8);
}

// 0x81966138 and 0x81975D80: a block 4 changes a component already built.
// Its first part (0x8197A078) hides triangles by collapsing them onto their
// first vertex, its second (0x8197A710) puts vertices elsewhere: each entry
// is a byte offset into the vertex buffer and a vertex quantized like the
// model's, with its own scale and origin. Faces get their nose, chin and ears
// this way, and some wearables reshape what they cover.
bool ApplyShape(const StrbBlockView& block, uint8_t* components,
                uint32_t built, const uint8_t* records) {
  std::span<const uint8_t> data = block.body;
  if (data.size() < 0x20) {
    return false;
  }
  uint8_t target[16];
  ReadShapeTarget(data.data() + 8, target);
  BitReader hide(data.subspan(0, 0x20));
  uint32_t hide_count = hide.Read(0, 32);
  uint32_t index_size = hide.Read(0x20, 32);
  uint32_t hide_bias = hide.Read(0xC0, 32);
  uint32_t hide_bits = hide.Read(0xE0, 32);
  size_t hide_bytes = (uint64_t(hide_count) * hide_bits + 7) / 8;
  size_t moves = 0x20 + hide_bytes;
  if (hide_bits > 32 || moves + 0x53 > data.size()) {
    return false;
  }
  std::span<const uint8_t> move_data = data.subspan(moves);
  BitReader move(move_data);
  uint32_t move_count = move.Read(0, 32);
  uint32_t vertex_size = move.Read(0x20, 32);
  uint32_t offset_bias = move.Read(0xC0, 32);
  uint32_t offset_bits = move.Read(0xE0, 32);
  float scale, origin[3];
  uint32_t scale_bits = move.Read(0x100, 32);
  std::memcpy(&scale, &scale_bits, 4);
  for (uint32_t i = 0; i < 3; ++i) {
    uint32_t v = move.Read(0x120 + 0x20 * i, 32);
    std::memcpy(&origin[i], &v, 4);
  }
  uint32_t width[3] = {move.Read(0x180, 6), move.Read(0x186, 6),
                       move.Read(0x18C, 6)};
  struct Field {
    uint32_t bits, bias;
  } fields[4] = {{move.Read(0x1B2, 32), move.Read(0x192, 32)},
                 {move.Read(0x1F2, 32), move.Read(0x1D2, 32)},
                 {move.Read(0x232, 32), move.Read(0x212, 32)},
                 {move.Read(0x272, 32), move.Read(0x252, 32)}};
  uint64_t entry_bits = uint64_t(offset_bits) + width[0] + width[1] + width[2];
  for (const auto& field : fields) {
    if (field.bits > 32) {
      return false;
    }
    entry_bits += field.bits;
  }
  if (offset_bits > 32 ||
      0x292 + entry_bits * move_count > move_data.size() * 8) {
    return false;
  }
  float step[3];
  for (uint32_t i = 0; i < 3; ++i) {
    step[i] = scale * kLatticeStep[i];
  }

  bool applied = false;
  for (int32_t c = int32_t(built) - 1; c >= 0; --c) {
    const uint8_t* component = components + c * kComponentSize;
    if (std::memcmp(records + c * kPartRecordSize, target, 16) ||
        load_be32(component + 0x10) != index_size ||
        load_be32(component + 0x0C) != vertex_size) {
      continue;
    }
    uint8_t* indices = TranslateVirtual(load_be32(component + 0x28));
    BitReader hidden(data.subspan(0x20, hide_bytes));
    for (uint32_t i = 0; i < hide_count; ++i) {
      uint32_t triangle = hidden.Read(uint64_t(i) * hide_bits, hide_bits) +
                          hide_bias;
      if (size_t(triangle) * 6 + 6 > index_size) {
        return false;
      }
      uint8_t* t = indices + size_t(triangle) * 6;
      std::memcpy(t + 2, t, 2);
      std::memcpy(t + 4, t, 2);
    }
    uint8_t* vertices = TranslateVirtual(load_be32(component + 0x24));
    uint64_t p = 0x292;
    for (uint32_t i = 0; i < move_count; ++i) {
      uint32_t offset = move.Read(p, offset_bits) + offset_bias;
      p += offset_bits;
      if (size_t(offset) + 0x1C > vertex_size) {
        return false;
      }
      uint32_t q[3];
      for (uint32_t a = 0; a < 3; ++a) {
        q[a] = move.Read(p, width[a]);
        p += width[a];
      }
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
      for (uint32_t f = 0; f < 4; ++f) {
        packed[f] = move.Read(p, fields[f].bits) + fields[f].bias;
        p += fields[f].bits;
      }
      if (coordinate_mode != 1) {
        z = -z;
        packed[0] = (uint32_t(-int32_t(packed[0] >> 22)) << 22) |
                    (packed[0] & 0x3FFFFF);
      }
      uint8_t* out = vertices + offset;
      StoreFloat(out + 0, x);
      StoreFloat(out + 4, y);
      StoreFloat(out + 8, z);
      for (uint32_t f = 0; f < 4; ++f) {
        StoreU32(out + 0xC + f * 4, packed[f]);
      }
    }
    applied = true;
  }
  return applied;
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
        part.slot = i;
        parts.push_back(part);
      }
    }
    for (uint32_t i = 0; i < 3; ++i) {
      const uint8_t* id = manifest + kManifestShapes + i * 0x10;
      if (!IsNullId(id)) {
        QueuedPart part{};
        std::memcpy(part.record.data(), id, 0x10);
        part.kind = 3;
        part.slot = i;
        parts.push_back(part);
      }
    }
  }
  return parts;
}

// What xam's random avatar generator (0x8197FF28) draws from.

// Chance in percent, for a male and a female avatar, that a random avatar
// has a part of each kind (0x816343C0). The hat comes first, as it decides
// which hair variants fit.
struct PartChance {
  uint32_t mask;
  uint8_t male, female;
};
constexpr PartChance kPartChances[] = {
    {0x40, 20, 5},        {0x4, 100, 100},      {0x8, 100, 100},
    {0x10, 100, 100},     {0x20, 100, 100},     {0x80, 5, 5},
    {0x100, 10, 10},      {0x200, 5, 20},       {0x400, 1, 75},
    {0x800, 5, 75},       {0x2000, 100, 100},   {0x4000, 100, 100},
    {0x8000, 100, 100},   {0x10000, 10, 0},     {0x20000, 20, 40},
    {0x40000, 0, 0},      {0x80000, 100, 100},  {0x100000, 100, 100},
    {0x200000, 100, 100},
};
// The face texture slot of each mask (0x8197F708) and the shape slot of each
// mask (0x8197F520).
constexpr uint32_t kFaceTextureSlotMasks[] = {0x8000,  0x2000,  0x4000,
                                              0x10000, 0x40000, 0x20000};
constexpr uint32_t kShapeSlotMasks[] = {0x100000, 0x80000, 0x200000};
// The weight and the height, both the same five steps (0x816345D8,
// 0x816345EC).
constexpr float kBodySteps[] = {-1.0f, -0.5f, 0.0f, 0.5f, 1.0f};

// Skin, all (0x81634610, 18 colors).
constexpr uint32_t kSkinColors[] = {
    0xFF6E4124, 0xFF64411B, 0xFF40280E, 0xFFD7AA71,
    0xFFBE8749, 0xFF98652F, 0xFFFCEEDD, 0xFFF8DBB9,
    0xFFE6BC8E, 0xFFDBCAB7, 0xFFF0B393, 0xFFE3946B,
    0xFFE3A976, 0xFFDE914D, 0xFFD3793D, 0xFFD7B971,
    0xFFC7984C, 0xFFA87D3F,
};
// Skin the generator picks (0x81634658, 12 colors).
constexpr uint32_t kNaturalSkinColors[] = {
    0xFF6E4124, 0xFFD7AA71, 0xFF64411B, 0xFFBE8749,
    0xFF98652F, 0xFFE6BC8E, 0xFFE3A976, 0xFFD7B971,
    0xFFF0B393, 0xFFDE914D, 0xFFC7984C, 0xFFA87D3F,
};
// Hair, eyebrows and facial hair (0x81634688, 27 colors).
constexpr uint32_t kHairColors[] = {
    0xFFFFFCB5, 0xFFFEEF7C, 0xFFF7D748, 0xFFE4D381,
    0xFFDCB74F, 0xFFBD934B, 0xFFDECFBA, 0xFFBFA783,
    0xFF988054, 0xFF906636, 0xFF745131, 0xFF493421,
    0xFF85541A, 0xFF553921, 0xFF1F120B, 0xFF6E5326,
    0xFF372116, 0xFF0D0D0D, 0xFFE3802F, 0xFFB85E22,
    0xFF753E1F, 0xFF92340E, 0xFF682618, 0xFF523436,
    0xFFE2DEDD, 0xFF8E8274, 0xFF4F463D,
};
// Lips, all (0x816349F0, 27 colors).
constexpr uint32_t kLipColors[] = {
    0xFFFD6A68, 0xFFD00E0E, 0xFFAD2D2D, 0xFFE58995,
    0xFFC65265, 0xFF9F5D69, 0xFFD59282, 0xFFB56157,
    0xFF83463F, 0xFFCF8562, 0xFF875741, 0xFF5C3B2C,
    0xFFEB7D80, 0xFFF5686B, 0xFFCB6758, 0xFFEFC28B,
    0xFFECA853, 0xFFBC7535, 0xFFD46552, 0xFFB14A37,
    0xFF8A1A0E, 0xFFDA61D4, 0xFFB93996, 0xFF8F2D5E,
    0xFF786DD5, 0xFF6E51A5, 0xFF000000,
};
// Lips the generator picks (0x81634A60, 21 colors).
constexpr uint32_t kNaturalLipColors[] = {
    0xFFFD6A68, 0xFFE58995, 0xFFD59282, 0xFFD00E0E,
    0xFFC65265, 0xFFB56157, 0xFFAD2D2D, 0xFF9F5D69,
    0xFF83463F, 0xFFCF8562, 0xFFEB7D80, 0xFFEFC28B,
    0xFF875741, 0xFFF5686B, 0xFFECA853, 0xFF5C3B2C,
    0xFFCB6758, 0xFFBC7535, 0xFFD46552, 0xFFB14A37,
    0xFF8A1A0E,
};
// Eyes, all (0x81634940, 18 colors).
constexpr uint32_t kEyeColors[] = {
    0xFF92B2CA, 0xFF6381A7, 0xFF2A72A4, 0xFF88B049,
    0xFF727F35, 0xFF37512A, 0xFFAE9843, 0xFF82501D,
    0xFF5B351E, 0xFF917A51, 0xFF312114, 0xFF212121,
    0xFFCECECC, 0xFF595D5C, 0xFF243F53, 0xFF7B7B95,
    0xFF8F7497, 0xFFAB0207,
};
// Eyes the generator picks (0x81634894, 15 colors).
constexpr uint32_t kNaturalEyeColors[] = {
    0xFF92B2CA, 0xFF88B049, 0xFFAE9843, 0xFF6381A7,
    0xFF727F35, 0xFF82501D, 0xFF2A72A4, 0xFF37512A,
    0xFF5B351E, 0xFF917A51, 0xFFCECECC, 0xFF312114,
    0xFF595D5C, 0xFF212121, 0xFF243F53,
};
// Eye shadow (0x81634988, 26 colors).
constexpr uint32_t kEyeShadowColors[] = {
    0xFFBA72B6, 0xFFF75A87, 0xFF9A2F7D, 0xFFC5496D,
    0xFF631A46, 0xFFAAA6C9, 0xFF776DD3, 0xFF6E50A4,
    0xFFA9CEF0, 0xFF5395CA, 0xFF195381, 0xFFB2B05D,
    0xFFC1C335, 0xFF61701F, 0xFFFFE2A3, 0xFFFEC854,
    0xFFE06E32, 0xFFF3BC7B, 0xFFC08950, 0xFF904F33,
    0xFFC1A590, 0xFFB37D59, 0xFF6A3919, 0xFFFFFFFF,
    0xFFA2A89B, 0xFF000000,
};
// Colors 7 and 8 (0x816348D0, 27 colors).
constexpr uint32_t kColor7Colors[] = {
    0xFFE8C84F, 0xFFE18417, 0xFFD13927, 0xFFE673B8,
    0xFF7F3979, 0xFF5F4294, 0xFF66BCDC, 0xFF2AB0AB,
    0xFF4265BC, 0xFF8ABF55, 0xFF399251, 0xFF4D5523,
    0xFFBCAE89, 0xFF8C603A, 0xFF654428, 0xFFFFFFFF,
    0xFF8B8B8B, 0xFF616161, 0xFFAA7C65, 0xFFAD5F47,
    0xFF59332F, 0xFFD75F47, 0xFFAA1D26, 0xFF81271F,
    0xFFECB6BE, 0xFFCF5969, 0xFFB24F7D,
};
// Hair the generator picks for a group of skin colors (0x816346F4, 11 colors).
constexpr uint32_t kHairForSkin1[] = {
    0xFF6E5326, 0xFF745131, 0xFF553921, 0xFF372116,
    0xFF493421, 0xFF1F120B, 0xFF0D0D0D, 0xFF92340E,
    0xFF682618, 0xFF523436, 0xFF4F463D,
};
// Hair the generator picks for a group of skin colors (0x81634720, 22 colors).
constexpr uint32_t kHairForSkin2[] = {
    0xFFDECFBA, 0xFFDCB74F, 0xFFBFA783, 0xFFBD934B,
    0xFF988054, 0xFF906636, 0xFF85541A, 0xFF6E5326,
    0xFF745131, 0xFF553921, 0xFF372116, 0xFF493421,
    0xFF1F120B, 0xFF0D0D0D, 0xFFE3802F, 0xFF92340E,
    0xFFB85E22, 0xFF682618, 0xFF8E8274, 0xFF753E1F,
    0xFF523436, 0xFF4F463D,
};
// Hair the generator picks for a group of skin colors (0x816347BC, 9 colors).
constexpr uint32_t kHairForSkin3[] = {
    0xFF553921, 0xFF372116, 0xFF493421, 0xFF1F120B,
    0xFF0D0D0D, 0xFF92340E, 0xFF682618, 0xFF523436,
    0xFF4F463D,
};
// Hair the generator picks for a group of skin colors (0x81634778, 17 colors).
constexpr uint32_t kHairForSkin4[] = {
    0xFFDCB74F, 0xFFBD934B, 0xFF906636, 0xFF85541A,
    0xFF6E5326, 0xFF745131, 0xFF553921, 0xFF372116,
    0xFF493421, 0xFF1F120B, 0xFF0D0D0D, 0xFF92340E,
    0xFFB85E22, 0xFF682618, 0xFF753E1F, 0xFF523436,
    0xFF4F463D,
};
// Hair the generator picks for a group of skin colors (0x816347E0, 24 colors).
constexpr uint32_t kHairForSkin5[] = {
    0xFFE4D381, 0xFFFEEF7C, 0xFFDCB74F, 0xFFBFA783,
    0xFFF7D748, 0xFFBD934B, 0xFF988054, 0xFF906636,
    0xFF85541A, 0xFF6E5326, 0xFF745131, 0xFF553921,
    0xFF372116, 0xFF493421, 0xFF1F120B, 0xFF0D0D0D,
    0xFFE3802F, 0xFF92340E, 0xFFB85E22, 0xFF682618,
    0xFF8E8274, 0xFF753E1F, 0xFF523436, 0xFF4F463D,
};
// Hair the generator picks for a group of skin colors (0x81634840, 21 colors).
constexpr uint32_t kHairForSkin6[] = {
    0xFFE4D381, 0xFFDCB74F, 0xFFBD934B, 0xFF988054,
    0xFF906636, 0xFF85541A, 0xFF6E5326, 0xFF745131,
    0xFF553921, 0xFF372116, 0xFF493421, 0xFF1F120B,
    0xFF0D0D0D, 0xFFE3802F, 0xFF92340E, 0xFFB85E22,
    0xFF682618, 0xFF8E8274, 0xFF753E1F, 0xFF523436,
    0xFF4F463D,
};

std::mutex random_mutex;
std::mt19937 random_engine{std::random_device{}()};

uint32_t RandomBelow(uint32_t n) {
  return n ? std::uniform_int_distribution<uint32_t>(0, n - 1)(random_engine)
           : 0;
}

template <size_t N>
uint32_t RandomColor(const uint32_t (&palette)[N]) {
  return palette[RandomBelow(N)];
}

// The hair palette goes with the skin color (0x81981448), by the skin's
// position among all the skin colors.
uint32_t RandomHairColor(uint32_t skin) {
  uint32_t group = 0;
  for (uint32_t i = 0; i < xe::countof(kSkinColors); ++i) {
    if (kSkinColors[i] == skin) {
      group = i + 1;
      break;
    }
  }
  switch (group) {
    case 1:
    case 8:
    case 17:
    case 18:
      return RandomColor(kHairForSkin1);
    case 4:
    case 7:
      return RandomColor(kHairForSkin3);
    case 5:
    case 14:
    case 15:
      return RandomColor(kHairForSkin4);
    case 6:
    case 13:
      return RandomColor(kHairForSkin5);
    case 9:
    case 16:
      return RandomColor(kHairForSkin6);
    default:
      return RandomColor(kHairForSkin2);
  }
}

// The colors of a stock wearable: a trio from the palette its asset carries
// (STRB block 7: a count, then trios of colors each in 8 bytes), or black.
std::array<uint32_t, 3> RandomPartColors(uint32_t index) {
  std::array<uint32_t, 3> colors = {0xFF000000, 0xFF000000, 0xFF000000};
  const uint8_t* entry = PackEntry(index);
  uint32_t start = PackDataStart(entry), size = PackDataSize(entry);
  std::vector<StrbBlockView> blocks;
  if (!ParseStrb(std::span<const uint8_t>(&asset_pack.toc[start], size),
                 &blocks)) {
    return colors;
  }
  for (const auto& block : blocks) {
    if (block.type != kStrbBlockPalette || block.body.size() < 4) {
      continue;
    }
    uint32_t count = xe::load<uint32_t>(block.body.data());
    if (!count || 4 + size_t(count) * 24 > block.body.size()) {
      break;
    }
    const uint8_t* trio = block.body.data() + 4 + RandomBelow(count) * 24;
    for (uint32_t c = 0; c < 3; ++c) {
      colors[c] = xe::load<uint32_t>(trio + c * 8);
    }
    break;
  }
  return colors;
}

// One random avatar in the manifest layout, following xam's generator: the
// weight and height (0x8197EBB0), the shapes (0x8197F520), the face
// textures (0x8197F708), the colors (0x8197ECE8) and the wearables
// (0x8197FA20), each kind by its chance for the body type.
// TODO(davicarlos): xam draws the parts of up to 100 avatars at once so that
// they come out evenly spread (0x8197F1D0, 0x8197FA98); this picks each part
// on its own.
void RandomManifest(uint8_t* manifest, uint32_t body_type) {
  std::memset(manifest, 0, kManifestSize);
  bool male = body_type == 1;
  xe::store_and_swap<float>(manifest + 4,
                            kBodySteps[RandomBelow(xe::countof(kBodySteps))]);
  xe::store_and_swap<float>(manifest + 8,
                            kBodySteps[RandomBelow(xe::countof(kBodySteps))]);

  const auto& toc = asset_pack.toc;
  auto entry = [&](uint32_t i) { return PackEntry(i); };
  auto entry_mask = [&](uint32_t i) { return load_be32(entry(i)); };
  auto entry_body = [&](uint32_t i) { return uint32_t(entry(i)[4]); };
  // Bit 0 of byte 6 is clear on the hair variants made to go under a hat.
  auto entry_hat_hair = [&](uint32_t i) { return !(entry(i)[6] & 1); };
  auto pick = [&](auto&& wanted) -> int32_t {
    std::vector<uint32_t> candidates;
    for (uint32_t i = 0; i < asset_pack.entry_count; ++i) {
      if (PackDataSize(entry(i)) && (entry_body(i) & body_type) &&
          wanted(i)) {
        candidates.push_back(i);
      }
    }
    return candidates.empty()
               ? -1
               : int32_t(candidates[RandomBelow(uint32_t(candidates.size()))]);
  };
  auto write_id = [&](uint8_t* out, uint32_t i) {
    MakeStockId(out, entry_mask(i), uint16_t(i), uint16_t(entry_body(i)));
  };

  // The stock body and the head.
  MakeStockId(manifest + kManifestBodyA, 2, male ? 0 : 1,
              uint16_t(body_type));
  xe::store_and_swap<uint16_t>(manifest + kManifestBodyA + 0x10, 2);
  int32_t head = pick([&](uint32_t i) { return entry_mask(i) == 1; });
  if (head >= 0) {
    write_id(manifest + kManifestBodyB, uint32_t(head));
    xe::store_and_swap<uint16_t>(manifest + kManifestBodyB + 0x10, 1);
  }

  uint32_t kinds = 0;
  for (const auto& chance : kPartChances) {
    if (RandomBelow(100) < (male ? chance.male : chance.female)) {
      kinds |= chance.mask;
    }
  }
  bool hat = kinds & 0x40;
  uint32_t used = 0, wearables = 0;
  for (const auto& chance : kPartChances) {
    uint32_t mask = chance.mask;
    if (!(kinds & mask) || mask > 0x800) {
      continue;
    }
    int32_t part = pick([&](uint32_t i) {
      uint32_t wearable = entry_mask(i) & 0xFFC;
      return wearable && (wearable & (0u - wearable)) == mask &&
             !(entry_mask(i) & 0xFFFF & used) &&
             (mask != 0x4 || entry_hat_hair(i) == hat);
    });
    if (part < 0 || wearables >= kMaxParts) {
      continue;
    }
    uint8_t* record = manifest + kManifestParts + wearables++ * 0x20;
    write_id(record, uint32_t(part));
    uint16_t covers = uint16_t(entry_mask(uint32_t(part)));
    xe::store_and_swap<uint16_t>(record + 0x10, covers);
    auto colors = RandomPartColors(uint32_t(part));
    for (uint32_t c = 0; c < 3; ++c) {
      xe::store_and_swap<uint32_t>(record + 0x14 + c * 4, colors[c]);
    }
    used |= covers;
  }
  for (uint32_t slot = 0; slot < xe::countof(kFaceTextureSlotMasks); ++slot) {
    uint32_t mask = kFaceTextureSlotMasks[slot];
    int32_t part =
        (kinds & mask) ? pick([&](uint32_t i) { return entry_mask(i) == mask; })
                       : -1;
    if (part >= 0) {
      // A face texture record ends with 1.0f (0x8197F590 puts it there).
      uint8_t* record = manifest + kManifestTextures + slot * 0x20;
      write_id(record, uint32_t(part));
      xe::store_and_swap<float>(record + 0x10, 1.0f);
    }
  }
  for (uint32_t slot = 0; slot < xe::countof(kShapeSlotMasks); ++slot) {
    uint32_t mask = kShapeSlotMasks[slot];
    int32_t part =
        (kinds & mask) ? pick([&](uint32_t i) { return entry_mask(i) == mask; })
                       : -1;
    if (part >= 0) {
      write_id(manifest + kManifestShapes + slot * 0x10, uint32_t(part));
    }
  }

  // 0x8197ECE8: skin, hair to go with it, eyes, eye shadow, lips and color
  // 7; the eyebrows and the facial hair take the hair color, color 8 the 7th.
  uint32_t colors[9];
  colors[0] = RandomColor(kNaturalSkinColors);
  colors[1] = RandomHairColor(colors[0]);
  colors[3] = RandomColor(kNaturalEyeColors);
  colors[5] = RandomColor(kEyeShadowColors);
  colors[2] = RandomColor(kNaturalLipColors);
  colors[7] = RandomColor(kColor7Colors);
  colors[8] = colors[7];
  colors[4] = colors[1];
  colors[6] = colors[1];
  for (uint32_t i = 0; i < 9; ++i) {
    xe::store_and_swap<uint32_t>(manifest + kManifestColors + i * 4,
                                 colors[i]);
  }
}

}  // namespace

bool RandomManifests(uint8_t* out, uint32_t count, uint32_t body_types) {
  if (!LoadAssetPack() || !(body_types & 3)) {
    return false;
  }
  std::lock_guard<std::mutex> lock(random_mutex);
  for (uint32_t i = 0; i < count; ++i) {
    uint32_t body_type = body_types & 3;
    if (body_type == 3) {
      body_type = 1 + RandomBelow(2);
    }
    RandomManifest(out + size_t(i) * kManifestSize, body_type);
  }
  return true;
}

std::vector<CatalogEntry> AssetCatalog(uint32_t language) {
  std::vector<CatalogEntry> catalog;
  if (!LoadAssetPack()) {
    return catalog;
  }
  // Every entry names the part in 13 languages, UTF-16BE strings in the pool
  // after the table, by XC_LANGUAGE.
  if (language >= 13) {
    language = 1;
  }
  const auto& toc = asset_pack.toc;
  for (uint32_t i = 0; i < asset_pack.entry_count; ++i) {
    const uint8_t* entry = PackEntry(i);
    if (!PackDataSize(entry)) {
      continue;
    }
    CatalogEntry part;
    part.index = uint16_t(i);
    part.mask = load_be32(entry);
    part.body_type = entry[4] & 0xF;
    for (size_t o = load_be32(entry + 0xBC + language * 4);
         o + 1 < toc.size() && (toc[o] || toc[o + 1]); o += 2) {
      // To UTF-8; the viewer's font only has Latin letters, so the rest of
      // the BMP becomes '?'.
      char16_t ch = char16_t((toc[o] << 8) | toc[o + 1]);
      if (ch < 0x80) {
        part.name.push_back(char(ch));
      } else if (ch < 0x800) {
        part.name.push_back(char(0xC0 | (ch >> 6)));
        part.name.push_back(char(0x80 | (ch & 0x3F)));
      } else {
        part.name.push_back('?');
      }
    }
    catalog.push_back(std::move(part));
  }
  return catalog;
}

void MakeStockId(uint8_t* id, uint32_t mask, uint16_t index,
                 uint16_t body_type) {
  std::memcpy(id, kStockMaleBody, 16);
  xe::store_and_swap<uint32_t>(id, mask);
  xe::store_and_swap<uint16_t>(id + 4, index);
  xe::store_and_swap<uint16_t>(id + 6, body_type);
}

// 0x8196CA80, which XamAvatarManifestGetBodyType calls.
uint32_t ManifestBodyType(const uint8_t* manifest) {
  if (!std::memcmp(manifest + kManifestBodyA, kStockMaleBody, 16)) {
    return 1;
  }
  if (!std::memcmp(manifest + kManifestBodyA, kStockFemaleBody, 16)) {
    return 2;
  }
  return 0;
}

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
  // The asset has to be made for the manifest's body.
  if (!(ManifestBodyType(manifest) & descriptor.body_type)) {
    return X_E_FAIL;
  }

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
  if (component_mask & kBodyComponent) {
    // Room for a component with a skeleton of its own and its animation,
    // laid out as 0x81967628 and 0x81967540 do. In the system's asset pack
    // only props have one (0x1000: a sheep, a teddy bear); they're built by
    // 0x81966C98, which isn't done here, so the room stays empty.
    uint32_t body = buffers.Allocate(false, 0x5C, 4);
    uint32_t body_skeleton = buffers.Allocate(false, 0x1B10, 0x10);
    uint32_t animation = buffers.Allocate(false, 0x5990, 4);
    uint32_t animation_data = buffers.Allocate(false, 0x28000, 4);
    if (!body || !body_skeleton || !animation || !animation_data) {
      return X_E_FAIL;
    }
    std::memset(TranslateVirtual(body), 0, 0x5C);
    std::memset(TranslateVirtual(body_skeleton), 0, 0x1B10);
    std::memset(TranslateVirtual(animation), 0, 0x5990);
    StoreU32(TranslateVirtual(body_skeleton) + 4, body_skeleton + 0x10);
    StoreU32(TranslateVirtual(body), body_skeleton);
    StoreU32(TranslateVirtual(body) + 0x58, animation);
    StoreU32(TranslateVirtual(animation) + 0x5968, kAnimationDefault);
    StoreU32(TranslateVirtual(animation) + 0x5988, 0x28000);
    StoreU32(TranslateVirtual(animation) + 0x598C, animation_data);
    StoreU32(header + 0x4, body);
  }

  // Components first, then what goes into them: face textures fill the
  // textures the head reserved (0x81969490 defers them, 0x819672F8).
  std::vector<QueuedPart> parts = QueueParts(manifest, component_mask);
  uint8_t* component_tables = TranslateVirtual(components);
  uint32_t built = 0;
  for (uint32_t pass = 1; pass <= 2; ++pass) {
    for (const auto& part : parts) {
      // The second pass also looks at components again for their block 4.
      if (part.kind != pass && pass != 2) {
        continue;
      }
      auto file = FindAsset(part.record.data());
      if (!file) {
        XELOGW("Avatar: asset {:016X}{:016X} not available",
               xe::load_and_swap<uint64_t>(&part.record[0]),
               xe::load_and_swap<uint64_t>(&part.record[8]));
        continue;
      }
      std::vector<StrbBlockView> blocks;
      AssetDescriptor descriptor;
      if (!ParseStrb(*file, &blocks) ||
          !ParseDescriptor(blocks, &descriptor)) {
        XELOGW("Avatar: unreadable asset {:016X}",
               xe::load_and_swap<uint64_t>(&part.record[0]));
        continue;
      }
      if (part.kind == 2) {
        if (ApplyFaceTexture(part, blocks, component_tables, built)) {
          XELOGI("Avatar: face texture {} from {:016X}", part.slot,
                 xe::load_and_swap<uint64_t>(&part.record[0]));
        } else {
          XELOGW("Avatar: face texture {} from {:016X} failed", part.slot,
                 xe::load_and_swap<uint64_t>(&part.record[0]));
        }
        continue;
      }
      if (part.kind == 3 || pass == 2) {
        // Shapes, and the block 4 a component may carry for others.
        bool any = false, ok = true;
        for (const auto& block : blocks) {
          if (block.type == kStrbBlockShape) {
            any = true;
            ok &= ApplyShape(block, component_tables, built,
                             TranslateVirtual(records));
          }
        }
        if (any) {
          XELOGI("Avatar: shape from {:016X} {}",
                 xe::load_and_swap<uint64_t>(&part.record[0]),
                 ok ? "applied" : "not applied");
        }
        continue;
      }
      if (built >= kMaxParts) {
        continue;
      }
      if (xe::load_and_swap<uint16_t>(&part.record[0x10]) == kBodyComponent) {
        XELOGW("Avatar: props with a skeleton (0x81966C98) aren't built");
        continue;
      }
      uint8_t* component = component_tables + built * kComponentSize;
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
  }
  return built ? X_E_SUCCESS : X_E_FAIL;
}

}  // namespace avatar
}  // namespace xam
}  // namespace kernel
}  // namespace xe
