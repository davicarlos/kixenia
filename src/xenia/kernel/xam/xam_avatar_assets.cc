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

#include <algorithm>
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
  kStrbBlockAnimation = 1,
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
uint32_t initialize_version = 0;

// The enumeration a title has open (the TOC reader's +0x19C..+0x1AC).
struct AssetEnumeration {
  uint32_t mask = 0;
  uint8_t body_type = 0;
  uint32_t flags = 0;
  uint32_t language = 1;
  uint32_t next = 0;
  bool active = false;
};
AssetEnumeration asset_enumeration;

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

namespace {

// The quantization 0x81976D78 reads for an animation track: scale, origin
// and three widths, into 0x34 bytes; the steps are the scale times the same
// lattice the vertices use (0x81A975B4, set up at 0x81A70E98).
void ReadAnimationQuantization(const BitReader& bits, uint64_t at,
                               uint8_t* out) {
  std::memset(out, 0, 0x34);
  uint32_t scale_bits = bits.Read(at, 32);
  float scale;
  std::memcpy(&scale, &scale_bits, 4);
  StoreU32(out, scale_bits);
  for (uint32_t i = 0; i < 3; ++i) {
    StoreFloat(out + 4 + i * 4, scale * kLatticeStep[i]);
    StoreU32(out + 0x10 + i * 4, bits.Read(at + 0x20 + i * 0x20, 32));
    StoreU32(out + 0x28 + i * 4, bits.Read(at + 0x80 + i * 6, 6));
  }
}

// A list of tracks (0x819770F0 with three quantizations each, 0x81976F18
// with two): its first 32 bits, then the count and, at +8, the entries, the
// last entry read first; +4 is the bits a key frame takes, the sum of the
// widths.
bool ReadAnimationTracks(std::span<const uint8_t> data, size_t header_size,
                         uint32_t max_count, uint32_t per_entry,
                         uint8_t* out) {
  if (data.size() < header_size) {
    return false;
  }
  BitReader bits(data.subspan(0, header_size));
  StoreU32(out, bits.Read(0, 32));
  uint32_t count = bits.Read(0x20, 32);
  if (count > max_count) {
    return false;
  }
  uint8_t* list = out + 4;
  StoreU32(list, count);
  uint32_t entry_size = per_entry * 0x34;
  uint32_t total = 0;
  for (uint32_t k = 0; k < count; ++k) {
    uint8_t* entry = list + 8 + (count - 1 - k) * entry_size;
    for (uint32_t q = 0; q < per_entry; ++q) {
      ReadAnimationQuantization(bits, 0x40 + uint64_t(k) * per_entry * 0x92 +
                                          q * 0x92,
                                entry + q * 0x34);
      for (uint32_t w = 0; w < 3; ++w) {
        total += load_be32(entry + q * 0x34 + 0x28 + w * 4);
      }
    }
  }
  StoreU32(list + 4, total);
  return true;
}

// 0x81976CB0: pairs of 32-bit values, the last pair read first, the second
// value of each stored first; +4 sums those.
bool ReadAnimationPairs(std::span<const uint8_t> data, uint8_t* out) {
  if (data.size() < 0x30) {
    return false;
  }
  BitReader bits(data.subspan(0, 0x30));
  StoreU32(out, bits.Read(0, 32));
  uint32_t count = bits.Read(0x20, 32);
  if (count > 5) {
    return false;
  }
  uint8_t* list = out + 4;
  StoreU32(list, count);
  uint32_t total = 0;
  for (uint32_t k = 0; k < count; ++k) {
    uint8_t* entry = list + 8 + (count - 1 - k) * 0xC;
    StoreU32(entry + 4, bits.Read(0x40 + uint64_t(k) * 0x40, 32));
    StoreU32(entry, bits.Read(0x60 + uint64_t(k) * 0x40, 32));
    total += load_be32(entry);
  }
  StoreU32(list + 4, total);
  return true;
}


// The stock skeleton of the human body, which xam keeps for each skinning
// version (0x81A975E8: 0x81633B78 for version 1, 0x81633D98 for 2, 0x21A
// bytes each, 0x81633FB4) and reads into the result's skeleton block with
// the block 5 reader (0x81979D68, 0x81970830) once the components are built
// (0x81969490, 0x819660A0). The bit stream: the bone count (0x81970070),
// a quantization of the positions and one of the rotation vectors (0x81976D78
// each, the same lattice as the vertices), then a record per bone with its
// parent in 8 bits and the two quantized vectors (0x819704D0, 0x81970460).
// The rotation vectors have no bits: the rest pose is unrotated.
constexpr uint8_t kStockSkeletonV1[0x21A] = {
    0x47, 0x00, 0x00, 0x00, 0x02, 0x44, 0xED, 0x36, 0x78, 0xD6, 0x24, 0xBF,
    0x19, 0x66, 0x97, 0x3C, 0x94, 0xC3, 0x97, 0xBD, 0x51, 0xE4, 0x08, 0x10,
    0xB5, 0xDB, 0x00, 0x00, 0x80, 0x93, 0x02, 0x00, 0x60, 0x95, 0x02, 0x00,
    0xE0, 0x95, 0x02, 0x00, 0xF0, 0xAF, 0x1D, 0x4B, 0x27, 0xDF, 0x37, 0x05,
    0xA0, 0x1D, 0x4B, 0x27, 0xDF, 0x37, 0x05, 0xA0, 0xAB, 0x0C, 0x9E, 0xDA,
    0xE8, 0x05, 0xA0, 0x8F, 0x09, 0x9E, 0xDA, 0xE8, 0x05, 0xA0, 0x1D, 0x8B,
    0x72, 0xE0, 0x37, 0x15, 0xA0, 0x1D, 0x0B, 0x09, 0x63, 0x9A, 0x24, 0xA0,
    0xAB, 0xEC, 0x40, 0x4F, 0xDF, 0x24, 0xA0, 0xAB, 0x6C, 0xEF, 0x14, 0x64,
    0x35, 0xA0, 0x8F, 0xE9, 0x40, 0x4F, 0xDF, 0x34, 0xA0, 0x8F, 0x69, 0xEF,
    0x14, 0x64, 0x15, 0xA0, 0x1D, 0xCB, 0xBD, 0xE1, 0xCE, 0x64, 0x50, 0xC5,
    0xAC, 0x45, 0x44, 0xD9, 0x52, 0x60, 0x3E, 0x4B, 0x36, 0xA8, 0xBD, 0x65,
    0x80, 0xB8, 0x4C, 0xC3, 0x89, 0xDC, 0x53, 0xA0, 0x1D, 0x2B, 0x62, 0x2A,
    0x1A, 0x83, 0x00, 0x76, 0xA9, 0x45, 0x44, 0xD9, 0x52, 0xF0, 0xFC, 0x4A,
    0x36, 0xA8, 0xBD, 0x85, 0xD0, 0x82, 0x49, 0xC3, 0x89, 0xDC, 0x53, 0xA0,
    0x1D, 0x4B, 0xDF, 0x64, 0x3A, 0xE4, 0xA0, 0x1D, 0x2B, 0x63, 0x6F, 0xEC,
    0xC4, 0x20, 0x46, 0xED, 0x36, 0xA8, 0x83, 0xB3, 0x00, 0xE8, 0x0C, 0x00,
    0xC0, 0x8D, 0x0D, 0x21, 0xF5, 0xE8, 0x36, 0xA8, 0x83, 0xF3, 0x40, 0x53,
    0x09, 0x00, 0xC0, 0x8D, 0xED, 0xA0, 0x1D, 0x2B, 0x0D, 0x6C, 0xB5, 0x43,
    0xB1, 0x33, 0xB0, 0x0B, 0xA8, 0x8F, 0x43, 0xF1, 0xBC, 0x4E, 0x21, 0xA8,
    0x89, 0x43, 0x21, 0x46, 0xED, 0x36, 0xA8, 0x83, 0x63, 0x91, 0x07, 0xA6,
    0x0B, 0xA8, 0x8F, 0x63, 0x61, 0x7E, 0x47, 0x21, 0xA8, 0x89, 0x63, 0x21,
    0xF5, 0xE8, 0x36, 0xA8, 0x83, 0x93, 0x01, 0xFA, 0x71, 0xF1, 0xA7, 0xAF,
    0x93, 0x71, 0x88, 0xF1, 0xF7, 0xA7, 0xA7, 0x93, 0x31, 0xDD, 0x52, 0xE4,
    0xA7, 0xBF, 0xC3, 0x41, 0x41, 0x64, 0xF1, 0xA7, 0xAF, 0xC3, 0xD1, 0xB2,
    0xE4, 0xF7, 0xA7, 0xA7, 0xC3, 0x21, 0x5E, 0x43, 0xE4, 0xA7, 0xBF, 0x13,
    0x32, 0xBC, 0x34, 0xAE, 0xA6, 0x21, 0x17, 0x22, 0xC4, 0x54, 0xBA, 0x66,
    0xA3, 0x14, 0x62, 0xA6, 0x74, 0x98, 0xA6, 0x43, 0x12, 0x32, 0x6C, 0x14,
    0x69, 0x66, 0x07, 0x10, 0x32, 0x24, 0xB4, 0x11, 0xA0, 0xBF, 0x13, 0xC2,
    0xC7, 0xB4, 0x32, 0xA3, 0xBF, 0x13, 0x32, 0xDD, 0x52, 0xE4, 0xA7, 0x89,
    0x44, 0x12, 0x7F, 0x21, 0xAE, 0xA6, 0x21, 0x47, 0x22, 0x77, 0x41, 0xBA,
    0x66, 0xA3, 0x44, 0xE2, 0x94, 0x61, 0x98, 0xA6, 0x43, 0x42, 0x12, 0xCF,
    0x01, 0x69, 0x66, 0x07, 0x40, 0x12, 0x17, 0xA2, 0x11, 0xA0, 0xBF, 0x43,
    0x82, 0x73, 0xA1, 0x32, 0xA3, 0xBF, 0x43, 0x22, 0x5E, 0x43, 0xE4, 0xA7,
    0x89, 0x54, 0x82, 0x8B, 0x35, 0xAE, 0x66, 0x29, 0x67, 0xF2, 0x9F, 0x55,
    0xBA, 0x26, 0xA3, 0x74, 0x22, 0x79, 0x75, 0x98, 0x66, 0x3C, 0x82, 0x42,
    0x1E, 0x15, 0x69, 0x26, 0x00, 0xB0, 0x42, 0xFE, 0x53, 0x51, 0xE6, 0x7B,
    0xC8, 0xD2, 0xAF, 0x20, 0xAE, 0x66, 0x29, 0xD7, 0x62, 0x9B, 0x40, 0xBA,
    0x26, 0xA3, 0xE4, 0x32, 0xC2, 0x60, 0x98, 0x66, 0x3C, 0xF2, 0x12, 0x1D,
    0x01, 0x69, 0x26, 0x00, 0x20, 0xF3, 0x3C, 0x42, 0x51, 0xE6, 0x7B, 0x38,
    0xE3, 0x20, 0x36, 0xAE, 0xA6, 0x29, 0x47, 0x53, 0x3B, 0x56, 0xBA, 0x26,
    0xA3, 0x54, 0xB3, 0x11, 0x76, 0x98, 0xA6, 0x3C, 0x62, 0x03, 0xA1, 0x15,
    0x69, 0x26, 0x00, 0x70, 0x23, 0x8C, 0x14, 0x95, 0x25, 0x0A, 0x8A, 0x63,
    0x1A, 0x20, 0xAE, 0xA6, 0x29, 0x97, 0x03, 0x00, 0x40, 0xBA, 0x26, 0xA3,
    0xA4, 0x93, 0x29, 0x60, 0x98, 0xA6, 0x3C, 0xB2, 0x53, 0x9A, 0x00, 0x69,
    0x26, 0x00, 0xC0, 0x23, 0xAF, 0x01, 0x95, 0x25, 0x0A, 0x0A,
};
constexpr uint8_t kStockSkeletonV2[0x21A] = {
    0x47, 0x00, 0x00, 0x00, 0x02, 0x44, 0xED, 0x36, 0x5E, 0x39, 0x2E, 0xBF,
    0x6B, 0xB0, 0x80, 0x3C, 0xF2, 0x7B, 0x88, 0xBD, 0x51, 0xE4, 0x08, 0x10,
    0xB5, 0xDB, 0x00, 0x00, 0x00, 0x93, 0x02, 0x00, 0xD0, 0x96, 0x02, 0x00,
    0xE0, 0x94, 0x02, 0x00, 0xF0, 0xBF, 0xBF, 0x6B, 0x30, 0xA0, 0x9F, 0x04,
    0xB0, 0xBF, 0x8B, 0x34, 0xA1, 0x9F, 0x04, 0xB0, 0x4D, 0x4D, 0xA7, 0x5B,
    0x50, 0x05, 0xB0, 0x31, 0x4A, 0xA7, 0x5B, 0x50, 0x05, 0xB0, 0xBF, 0xAB,
    0x7B, 0xA1, 0x9F, 0x14, 0xB0, 0xBF, 0x8B, 0x1A, 0x26, 0x02, 0x24, 0xB0,
    0x4D, 0xCD, 0x6F, 0x4F, 0x33, 0x24, 0xB0, 0x4D, 0x8D, 0x8B, 0xD5, 0xCB,
    0x34, 0xB0, 0x31, 0xCA, 0x6F, 0x4F, 0x33, 0x34, 0xB0, 0x31, 0x8A, 0x8B,
    0xD5, 0xCB, 0x14, 0xB0, 0xBF, 0x2B, 0xCB, 0xA3, 0x36, 0x64, 0x40, 0x69,
    0xAD, 0xA1, 0x43, 0x06, 0x52, 0x60, 0xE0, 0xAB, 0x47, 0x6B, 0x25, 0x65,
    0xD0, 0x59, 0x0D, 0xA3, 0x49, 0x3D, 0x53, 0xB0, 0xBF, 0xAB, 0x73, 0xED,
    0x81, 0x82, 0x20, 0x16, 0xAA, 0xA1, 0x43, 0x06, 0x52, 0x00, 0x9F, 0xAB,
    0x47, 0x6B, 0x25, 0x85, 0x80, 0x25, 0x0A, 0xA3, 0x49, 0x3D, 0x53, 0xB0,
    0xBF, 0xCB, 0xF0, 0x27, 0xA2, 0xE3, 0xB0, 0xBF, 0x8B, 0xB4, 0x31, 0x0E,
    0xC4, 0x30, 0xE8, 0x6D, 0x48, 0x6B, 0xEB, 0xB2, 0xC0, 0x86, 0x0D, 0x00,
    0xC0, 0x1F, 0x0B, 0x31, 0x97, 0x69, 0x48, 0x6B, 0xEB, 0xF2, 0x90, 0xF8,
    0x09, 0x00, 0xC0, 0x1F, 0xEB, 0xB0, 0xBF, 0xAB, 0xDE, 0xEE, 0x05, 0x43,
    0x01, 0xB5, 0x31, 0x10, 0xEB, 0xFA, 0x42, 0x91, 0xCE, 0xCF, 0x33, 0x2B,
    0xF1, 0x42, 0xA1, 0xFB, 0x6D, 0x48, 0x6B, 0xEB, 0x62, 0x61, 0xCA, 0x25,
    0x10, 0xEB, 0xFA, 0x62, 0xD1, 0xB0, 0xC7, 0x33, 0x2B, 0xF1, 0x62, 0xC1,
    0x83, 0x69, 0x48, 0x6B, 0xEB, 0x92, 0xB1, 0xA8, 0x73, 0xF3, 0x2A, 0x1E,
    0x93, 0xC1, 0x2B, 0x93, 0xFD, 0xEA, 0x11, 0x93, 0x91, 0xA2, 0xF4, 0xE4,
    0xAA, 0x2F, 0xC3, 0xA1, 0xD6, 0x63, 0xF3, 0x2A, 0x1E, 0xC3, 0x91, 0x53,
    0x84, 0xFD, 0xEA, 0x11, 0xC3, 0xC1, 0xDC, 0xE2, 0xE4, 0xAA, 0x2F, 0x13,
    0xB2, 0x39, 0x76, 0xDD, 0xA9, 0x0F, 0x16, 0x82, 0x40, 0xB6, 0xE7, 0x29,
    0xF1, 0x13, 0x32, 0x27, 0xF6, 0xCA, 0xA9, 0xEC, 0x11, 0xC2, 0xF5, 0x95,
    0xA2, 0x69, 0x06, 0x10, 0x92, 0xB8, 0xB5, 0x3E, 0xA4, 0x2F, 0x13, 0x92,
    0x43, 0x96, 0xE7, 0xA6, 0x2F, 0x13, 0x92, 0xA2, 0xF4, 0xE4, 0x6A, 0xDB,
    0x43, 0xA2, 0x45, 0x61, 0xDD, 0xA9, 0x0F, 0x46, 0xD2, 0x3E, 0xA1, 0xE7,
    0x29, 0xF1, 0x43, 0x22, 0x58, 0xE1, 0xCA, 0xA9, 0xEC, 0x41, 0xA2, 0x89,
    0x81, 0xA2, 0x69, 0x06, 0x40, 0xC2, 0xC6, 0xA1, 0x3E, 0xA4, 0x2F, 0x43,
    0xD2, 0x3B, 0x81, 0xE7, 0xA6, 0x2F, 0x43, 0xD2, 0xDC, 0xE2, 0xE4, 0x6A,
    0xDB, 0x53, 0xF2, 0xE9, 0x76, 0xDD, 0x69, 0x16, 0x66, 0x52, 0xFB, 0xB6,
    0xE7, 0xE9, 0xF0, 0x73, 0x52, 0xDA, 0xF6, 0xCA, 0xA9, 0xE6, 0x81, 0x12,
    0x8D, 0x96, 0xA2, 0x29, 0x00, 0xB0, 0x52, 0x98, 0x75, 0x8E, 0x29, 0x36,
    0xC7, 0x72, 0x95, 0x60, 0xDD, 0x69, 0x16, 0xD6, 0x12, 0x84, 0xA0, 0xE7,
    0xE9, 0xF0, 0xE3, 0x12, 0xA5, 0xE0, 0xCA, 0xA9, 0xE6, 0xF1, 0x52, 0xF2,
    0x80, 0xA2, 0x29, 0x00, 0x20, 0x03, 0xE7, 0x61, 0x8E, 0x29, 0x36, 0x37,
    0xF3, 0x68, 0x77, 0xDD, 0x69, 0x16, 0x46, 0x63, 0x7F, 0xB7, 0xE7, 0x29,
    0xF1, 0x53, 0x03, 0x5C, 0xF7, 0xCA, 0xA9, 0xE6, 0x61, 0x43, 0xFC, 0x96,
    0xA2, 0x29, 0x00, 0x70, 0xE3, 0x10, 0x76, 0xEE, 0xA8, 0x88, 0x88, 0x73,
    0x16, 0x60, 0xDD, 0x69, 0x16, 0x96, 0x03, 0x00, 0xA0, 0xE7, 0x29, 0xF1,
    0xA3, 0x53, 0x23, 0xE0, 0xCA, 0xA9, 0xE6, 0xB1, 0x23, 0x83, 0x80, 0xA2,
    0x29, 0x00, 0xC0, 0x73, 0x6E, 0x61, 0xEE, 0xA8, 0x88, 0x08,
};

constexpr uint32_t kMaxBones = 0x48;  // 0x81970830 refuses more.
constexpr uint32_t kBoneSize = 0x60;

struct BoneQuantization {
  float scale;
  float origin[3];
  uint32_t width[3];
};

// 0x81976D78: scale, three origins and three widths of 6 bits, 0x92 bits.
void ReadBoneQuantization(const BitReader& bits, uint64_t at,
                          BoneQuantization* q) {
  uint32_t scale_bits = bits.Read(at, 32);
  std::memcpy(&q->scale, &scale_bits, 4);
  for (uint32_t i = 0; i < 3; ++i) {
    uint32_t origin_bits = bits.Read(at + 0x20 + i * 0x20, 32);
    std::memcpy(&q->origin[i], &origin_bits, 4);
    q->width[i] = bits.Read(at + 0x80 + i * 6, 6);
  }
}

// 0x81971328 and 0x81970FA0: three indices into the hexagonal lattice.
void DequantizeBoneVector(const BitReader& bits, uint64_t* at,
                          const BoneQuantization& q, float out[3]) {
  uint32_t index[3];
  for (uint32_t i = 0; i < 3; ++i) {
    index[i] = bits.Read(*at, q.width[i]);
    *at += q.width[i];
  }
  float step[3];
  for (uint32_t i = 0; i < 3; ++i) {
    step[i] = q.scale * kLatticeStep[i];
  }
  out[1] = float(index[1]) * step[1] + q.origin[1];
  out[2] = float(index[2]) * step[2] + q.origin[2];
  if (index[1] & 1) {
    out[2] += step[2] * kLatticeOddLayerShift;
  }
  out[0] = float(index[0]) * step[0] + q.origin[0];
  if ((index[1] ^ index[2]) & 1) {
    out[0] += step[0] * kLatticeOddRowShift;
  }
}

// A bone of the skeleton block, 0x60 bytes in the result: the parent, the
// child and the next sibling (0xFF for none), the position and rotation in
// the model's space, the same relative to the parent, and the scale.
struct Bone {
  uint32_t parent = 0xFF;
  uint32_t child = 0xFF;
  uint32_t sibling = 0xFF;
  float position[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  float rotation[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  float local_position[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  float local_rotation[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  float scale[4] = {1.0f, 1.0f, 1.0f, 1.0f};
};

// 0x81970110: a rotation vector (axis times the half angle) to a quaternion.
void RotationVectorToQuaternion(const float v[3], float q[4]) {
  float angle = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  if (angle == 0.0f) {
    q[0] = q[1] = q[2] = 0.0f;
    q[3] = 1.0f;
    return;
  }
  float s = std::sin(angle) / angle;
  q[0] = v[0] * s;
  q[1] = v[1] * s;
  q[2] = v[2] * s;
  q[3] = std::cos(angle);
}

bool ReadStockSkeleton(std::vector<Bone>* bones) {
  std::span<const uint8_t> data(skinning_version < 2 ? kStockSkeletonV1
                                                      : kStockSkeletonV2);
  BitReader bits(data);
  uint32_t count = bits.Read(0, 32);
  if (!count || count > kMaxBones) {
    return false;
  }
  BoneQuantization position_q, rotation_q;
  ReadBoneQuantization(bits, 0x20, &position_q);
  ReadBoneQuantization(bits, 0x20 + 0x92, &rotation_q);
  uint32_t record_bits = 8;
  for (uint32_t i = 0; i < 3; ++i) {
    record_bits += position_q.width[i] + rotation_q.width[i];
  }
  // The records follow the 0x144 header bits (0x819704D0 seeks to byte 0x28
  // and starts at its bit 4).
  if (0x144 + uint64_t(count) * record_bits > data.size() * 8) {
    return false;
  }
  bones->resize(count);
  uint64_t at = 0x144;
  for (uint32_t i = 0; i < count; ++i) {
    Bone& bone = (*bones)[i];
    bone.parent = bits.Read(at, 8);
    at += 8;
    DequantizeBoneVector(bits, &at, position_q, bone.position);
    bone.position[3] = 1.0f;
    float rotation_vector[3];
    DequantizeBoneVector(bits, &at, rotation_q, rotation_vector);
    RotationVectorToQuaternion(rotation_vector, bone.rotation);
    if (coordinate_mode != 1) {
      // 0x81970830 flips the handedness: z, and x and y of the quaternion
      // (0x81633270, 0x81633280).
      bone.position[2] = -bone.position[2];
      bone.rotation[0] = -bone.rotation[0];
      bone.rotation[1] = -bone.rotation[1];
    }
  }
  return true;
}

// 0x8196F260: each bone's child is the last bone that names it as parent,
// and from there the siblings chain down.
void LinkBones(std::vector<Bone>& bones) {
  int32_t count = int32_t(bones.size());
  for (int32_t i = count - 1; i >= 0; --i) {
    bones[i].child = 0xFF;
    bones[i].sibling = 0xFF;
    int32_t last = 0xFF;
    int32_t j = count - 1;
    for (; j > i; --j) {
      if (int32_t(bones[j].parent) == i) {
        bones[i].child = uint32_t(j);
        last = j;
        break;
      }
    }
    for (--j; j > i; --j) {
      if (int32_t(bones[j].parent) == i) {
        bones[last].sibling = uint32_t(j);
        last = j;
      }
    }
  }
}

void QuaternionMultiply(const float a[4], const float b[4], float out[4]) {
  out[0] = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
  out[1] = a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0];
  out[2] = a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3];
  out[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
}

// 0x8196F520: the position and rotation of each bone relative to its
// parent (the parent's rotation inverted: conjugate over the norm, as the
// VMX code does), the root keeping its own; the scale starts at one.
void LocalizeBones(std::vector<Bone>& bones) {
  for (size_t i = bones.size(); i-- > 0;) {
    Bone& bone = bones[i];
    for (uint32_t k = 0; k < 4; ++k) {
      bone.scale[k] = 1.0f;
    }
    if (i == 0 || bone.parent >= bones.size()) {
      std::memcpy(bone.local_position, bone.position, sizeof(bone.position));
      std::memcpy(bone.local_rotation, bone.rotation, sizeof(bone.rotation));
      continue;
    }
    const Bone& parent = bones[bone.parent];
    const float* p = parent.rotation;
    float norm = p[0] * p[0] + p[1] * p[1] + p[2] * p[2] + p[3] * p[3];
    float inverse[4] = {-p[0], -p[1], -p[2], p[3]};
    if (norm > 0.0f) {
      for (uint32_t k = 0; k < 4; ++k) {
        inverse[k] /= norm;
      }
    }
    float delta[4] = {bone.position[0] - parent.position[0],
                      bone.position[1] - parent.position[1],
                      bone.position[2] - parent.position[2], 0.0f};
    // Rotate the offset by the inverse: q * v * q^-1.
    float conjugate[4] = {-inverse[0], -inverse[1], -inverse[2], inverse[3]};
    float t[4], rotated[4];
    QuaternionMultiply(inverse, delta, t);
    QuaternionMultiply(t, conjugate, rotated);
    bone.local_position[0] = rotated[0];
    bone.local_position[1] = rotated[1];
    bone.local_position[2] = rotated[2];
    bone.local_position[3] = 1.0f;
    float local[4];
    QuaternionMultiply(inverse, bone.rotation, local);
    float length = std::sqrt(local[0] * local[0] + local[1] * local[1] +
                             local[2] * local[2] + local[3] * local[3]);
    for (uint32_t k = 0; k < 4; ++k) {
      bone.local_rotation[k] = length > 0.0f ? local[k] / length : local[k];
    }
  }
}

// 0x81974E10: each bone's scale gets 1 + (factor - 1) * amount, the amount
// clamped to [0, 1]; the root's position relative to its parent too.
void ScaleBones(std::vector<Bone>& bones, const float (*factors)[4],
                float amount) {
  amount = std::clamp(amount, 0.0f, 1.0f);
  for (size_t i = 0; i < bones.size(); ++i) {
    for (uint32_t k = 0; k < 4; ++k) {
      bones[i].scale[k] *= (factors[i][k] - 1.0f) * amount + 1.0f;
    }
  }
  if (!bones.empty()) {
    for (uint32_t k = 0; k < 4; ++k) {
      bones[0].local_position[k] *= (factors[0][k] - 1.0f) * amount + 1.0f;
    }
  }
}

// The bones the height changes (the hips, the legs, the spine and the arms)
// and the factors for each body type and direction (0x81974EE8, 0x81975140,
// 0x81975018, 0x81975270; the tables at 0x816333B0 to 0x81633570).
struct HeightTable {
  float hips[4], upper_legs[4], spine[4], chest[4], head[4], arms[4];
};
constexpr HeightTable kTallerMale = {
    {1.6f, 1.6f, 2.2f, 1.0f}, {1.5f, 1.0f, 1.5f, 1.0f}, {1.8f, 1.0f, 1.9f, 1.0f},
    {1.5f, 1.0f, 1.4f, 1.0f}, {1.9f, 1.0f, 1.5f, 1.0f}, {1.0f, 1.5f, 1.5f, 1.0f}};
constexpr HeightTable kTallerFemale = {
    {1.5f, 1.5f, 2.0f, 1.0f}, {1.6f, 1.0f, 1.6f, 1.0f}, {1.6f, 1.0f, 2.0f, 1.0f},
    {1.6f, 1.0f, 1.6f, 1.0f}, {2.0f, 1.0f, 1.6f, 1.0f}, {1.0f, 1.6f, 1.6f, 1.0f}};
constexpr HeightTable kShorterMaleV1 = {
    {0.8f, 1.0f, 0.9f, 1.0f}, {0.7f, 1.0f, 0.7f, 1.0f}, {0.6f, 1.0f, 0.6f, 1.0f},
    {0.9f, 1.0f, 0.8f, 1.0f}, {0.7f, 1.0f, 0.7f, 1.0f}, {1.0f, 0.7f, 0.7f, 1.0f}};
constexpr HeightTable kShorterMaleV2 = {
    {0.84f, 1.0f, 0.92f, 1.0f}, {0.76f, 1.0f, 0.76f, 1.0f},
    {0.68f, 1.0f, 0.68f, 1.0f}, {0.92f, 1.0f, 0.84f, 1.0f},
    {0.76f, 1.0f, 0.76f, 1.0f}, {1.0f, 0.76f, 0.76f, 1.0f}};
constexpr HeightTable kShorterFemaleV1 = {
    {0.65f, 1.0f, 0.7f, 1.0f}, {0.7f, 1.0f, 0.7f, 1.0f}, {0.7f, 1.0f, 0.5f, 1.0f},
    {0.8f, 1.0f, 0.7f, 1.0f}, {0.7f, 1.0f, 0.7f, 1.0f}, {1.0f, 0.7f, 0.7f, 1.0f}};
constexpr HeightTable kShorterFemaleV2 = {
    {0.79f, 1.0f, 0.82f, 1.0f}, {0.82f, 1.0f, 0.82f, 1.0f},
    {0.82f, 1.0f, 0.7f, 1.0f},  {0.88f, 1.0f, 0.82f, 1.0f},
    {0.82f, 1.0f, 0.82f, 1.0f}, {1.0f, 0.82f, 0.82f, 1.0f}};

void FillHeightFactors(const HeightTable& table, float (*factors)[4]) {
  auto set = [&](uint32_t bone, const float (&v)[4]) {
    std::memcpy(factors[bone], v, sizeof(v));
  };
  set(4, table.hips);
  set(7, table.upper_legs);
  set(9, table.upper_legs);
  set(13, table.upper_legs);
  set(17, table.upper_legs);
  set(10, table.spine);
  set(18, table.chest);
  set(24, table.head);
  for (uint32_t bone : {26u, 27u, 29u, 30u, 32u, 35u}) {
    set(bone, table.arms);
  }
}

// 0x81975C00: the weight (0x819753F0, 0x819753A0: the root and bone 19)
// and then the height by body type.
void ScaleStockSkeleton(std::vector<Bone>& bones, float height, float weight,
                        uint32_t body_type) {
  float factors[kMaxBones][4];
  auto reset = [&] {
    for (auto& f : factors) {
      f[0] = f[1] = f[2] = f[3] = 1.0f;
    }
  };
  reset();
  if (weight < 0.0f) {
    factors[0][0] = factors[0][1] = factors[0][2] = 0.9f;
    factors[19][0] = factors[19][1] = factors[19][2] = 1.05f;
  } else {
    factors[0][0] = factors[0][1] = factors[0][2] = 1.1f;
    factors[19][0] = factors[19][1] = factors[19][2] = 0.9f;
  }
  ScaleBones(bones, factors, std::fabs(weight));
  reset();
  if (body_type == 1) {
    FillHeightFactors(height < 0.0f ? (skinning_version < 2 ? kShorterMaleV1
                                                             : kShorterMaleV2)
                                    : kTallerMale,
                      factors);
  } else if (body_type == 2) {
    FillHeightFactors(height > 0.0f ? kTallerFemale
                                    : (skinning_version < 2 ? kShorterFemaleV1
                                                            : kShorterFemaleV2),
                      factors);
  } else {
    return;
  }
  ScaleBones(bones, factors, std::fabs(height));
}

void WriteBone(uint8_t* out, const Bone& bone) {
  std::memset(out, 0, kBoneSize);
  StoreU32(out + 0x0, bone.parent);
  StoreU32(out + 0x4, bone.child);
  StoreU32(out + 0x8, bone.sibling);
  for (uint32_t k = 0; k < 4; ++k) {
    StoreFloat(out + 0x10 + k * 4, bone.position[k]);
    StoreFloat(out + 0x20 + k * 4, bone.rotation[k]);
    StoreFloat(out + 0x30 + k * 4, bone.local_position[k]);
    StoreFloat(out + 0x40 + k * 4, bone.local_rotation[k]);
    StoreFloat(out + 0x50 + k * 4, bone.scale[k]);
  }
}

// Fills a skeleton block (0x1B10 bytes: the count, the pointer to the bones
// at +0x10, then the bones) with the stock skeleton of the manifest's body:
// the height at +4 and the weight at +8 of the manifest, -1 to 1. Titles
// walk the bones from the first without looking at the count (Kinect
// Adventures 0x82A73418, Kinect Sports 0x82558F30), so an empty block sends
// them in circles.
bool FillStockSkeleton(uint8_t* block, uint32_t block_address, float height,
                       float weight, uint32_t body_type) {
  std::vector<Bone> bones;
  if (!ReadStockSkeleton(&bones)) {
    return false;
  }
  LinkBones(bones);
  LocalizeBones(bones);
  ScaleStockSkeleton(bones, height, weight, body_type);
  StoreU32(block, uint32_t(bones.size()));
  StoreU32(block + 4, block_address + 0x10);
  for (size_t i = 0; i < bones.size(); ++i) {
    WriteBone(block + 0x10 + i * kBoneSize, bones[i]);
  }
  return true;
}

}  // namespace

X_HRESULT LoadAnimation(const uint8_t* id, uint32_t output_ptr) {
  auto file = FindAsset(id);
  std::vector<StrbBlockView> blocks;
  if (!file || !ParseStrb(*file, &blocks)) {
    XELOGW("Avatar: animation {:016X} not available",
           xe::load_and_swap<uint64_t>(id));
    return X_E_FAIL;
  }
  const StrbBlockView* block = nullptr;
  for (const auto& b : blocks) {
    if (b.type == kStrbBlockAnimation) {
      block = &b;
      break;
    }
  }
  if (!block || block->body.size() < 0x28) {
    return X_E_FAIL;
  }
  // 0x81977310: a 0x28 byte header, then the data, copied to where the
  // title's animation says (+0x598C, with room for +0x5988 bytes). The
  // tracks are read out of the data; the key frames stay packed for the
  // title's avatar library.
  uint8_t* animation = TranslateVirtual(output_ptr);
  BitReader header(block->body.subspan(0, 0x28));
  uint32_t size = header.Read(0x120, 32);
  uint32_t room = load_be32(animation + 0x5988);
  uint32_t data_ptr = load_be32(animation + 0x598C);
  if (size > room || !data_ptr || 0x28 + size_t(size) > block->body.size()) {
    XELOGW("Avatar: animation of {:X} bytes for room of {:X}", size, room);
    return X_E_FAIL;
  }
  StoreU32(animation + 0x5988, size);
  static const std::pair<uint32_t, uint32_t> kFields[] = {
      {0x00, 0x5964}, {0x20, 0x5968}, {0x40, 0x596C}, {0x60, 0x5970},
      {0xA0, 0x5978}, {0x80, 0x5974}, {0xC0, 0x597C}, {0xE0, 0x5984},
      {0x100, 0x5980}};
  for (const auto& field : kFields) {
    StoreU32(animation + field.second, header.Read(field.first, 32));
  }
  std::span<const uint8_t> data = block->body.subspan(0x28, size);
  std::memcpy(TranslateVirtual(data_ptr), data.data(), size);
  uint32_t first = load_be32(animation + 0x597C);
  uint32_t second = load_be32(animation + 0x5980);
  uint32_t third = load_be32(animation + 0x5984);
  if (first > second || second > third || third > size) {
    return X_E_FAIL;
  }
  bool tracks_read =
      ReadAnimationTracks(data.subspan(0, first), 0xF6E, 0x48, 3,
                          animation) &&
      ReadAnimationTracks(data.subspan(first, second - first), 0xF6E, 0x48, 3,
                          animation + 0x2BEC) &&
      ReadAnimationTracks(data.subspan(second, third - second), 0x76, 3, 2,
                          animation + 0x57D8) &&
      ReadAnimationPairs(data.subspan(third), animation + 0x591C);
  if (!tracks_read) {
    XELOGW("Avatar: bad tracks in animation {:016X}",
           xe::load_and_swap<uint64_t>(id));
    return X_E_FAIL;
  }
  std::lock_guard<std::mutex> lock(state_mutex);
  if (skinning_version > 1 && coordinate_mode != 1) {
    // Turn the tracks to the other handedness, as 0x81977310 does.
    auto negate = [&](uint32_t offset) {
      StoreFloat(animation + offset,
                 -xe::load_and_swap<float>(animation + offset));
    };
    for (uint32_t i = 0; i < 0x48; ++i) {
      uint32_t b = 0x5814 - i * 0x9C;
      for (uint32_t offset : {0x2CACu, 0x2CB8u, 0x2C80u, 0x2C7Cu, 0x2C8Cu,
                              0x2C88u, 0xC0u, 0xCCu, 0x94u, 0x90u, 0xA0u,
                              0x9Cu}) {
        negate(b - offset);
      }
    }
    for (uint32_t i = 0; i < 3; ++i) {
      uint32_t b = 0x5958 - i * 0x68;
      for (uint32_t offset : {0x8Cu, 0x98u, 0x60u, 0x5Cu, 0x6Cu, 0x68u}) {
        negate(b - offset);
      }
    }
  }
  XELOGD("Avatar: animation {:016X}: {} bytes, {} and {} tracks",
         xe::load_and_swap<uint64_t>(id), size, load_be32(animation + 4),
         load_be32(animation + 0x2BF0));
  return X_E_SUCCESS;
}

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
  initialize_version = version;
  skinning_version = 2 - (version < 3 ? 1 : 0);
  coordinate_mode = mode;
}

void BeginAssetEnumeration(uint32_t mask, uint8_t body_type, uint32_t flags,
                           uint32_t language) {
  std::lock_guard<std::mutex> lock(state_mutex);
  asset_enumeration = AssetEnumeration();
  asset_enumeration.mask = mask;
  asset_enumeration.body_type = body_type;
  asset_enumeration.flags = flags;
  asset_enumeration.language = language < 13 ? language : 1;
  asset_enumeration.active = LoadAssetPack() && asset_pack.entry_count != 0;
}

uint32_t EnumerateAssets(uint8_t* out, uint32_t count) {
  std::lock_guard<std::mutex> lock(state_mutex);
  auto& e = asset_enumeration;
  if (!e.active || initialize_version < 4) {
    return 0;
  }
  uint32_t written = 0;
  for (; written < count && e.next < asset_pack.entry_count; ++e.next) {
    const uint8_t* entry = PackEntry(e.next);
    // The filter (0x8197BDA0): bit 1 of byte 6 hides the entry, bit 0 lists
    // it in the editor.
    uint8_t listing = entry[6];
    uint8_t body = (e.flags & 0x10) ? entry[5] : entry[4];
    if ((listing & 2) || (!(listing & 1) && !(e.flags & 2))) {
      continue;
    }
    uint32_t mask = load_be32(entry);
    if ((e.flags & 4) ? mask != e.mask : !(mask & e.mask)) {
      continue;
    }
    if ((e.flags & 8) ? body != e.body_type : !(body & e.body_type)) {
      continue;
    }
    // The item (0x8197E300).
    uint8_t* item = out + size_t(written) * kAssetItemSize;
    std::memset(item, 0, kAssetItemSize);
    MakeStockId(item, mask, uint16_t(e.next), entry[4]);
    xe::store_and_swap<uint32_t>(item + 0x10, mask);
    item[0x14] = entry[4];
    item[0x15] = entry[5];
    xe::store_and_swap<uint32_t>(item + 0x18, listing);
    std::memcpy(item + 0x1C, entry + 7, 0x91);
    xe::store_and_swap<uint32_t>(item + 0xC0, load_be32(entry + 0x98));
    std::memcpy(item + 0xC4, entry + 0x9C, 0x20);
    xe::store_and_swap<uint32_t>(item + 0x164, PackDataStart(entry));
    xe::store_and_swap<uint32_t>(item + 0x168, PackDataSize(entry));
    // The first pack's bit.
    item[0x2C0] = 1;
    if (e.flags & 1) {
      const auto& toc = asset_pack.toc;
      size_t o = load_be32(entry + 0xBC + e.language * 4);
      for (uint32_t c = 0; c < 0x3F && o + 1 < toc.size() &&
                           (toc[o] || toc[o + 1]);
           ++c, o += 2) {
        item[0xE4 + c * 2] = toc[o];
        item[0xE5 + c * 2] = toc[o + 1];
      }
    }
    ++written;
  }
  return written;
}

void EndAssetEnumeration() {
  std::lock_guard<std::mutex> lock(state_mutex);
  asset_enumeration = AssetEnumeration();
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
  if (built) {
    // 0x81969490 then gives the result the body's skeleton (0x819660A0),
    // scaled by the manifest's height and weight; the body component's
    // skeleton, when there is one, takes the root's scale.
    float height = xe::load_and_swap<float>(manifest + 4);
    float weight = xe::load_and_swap<float>(manifest + 8);
    uint8_t* block = TranslateVirtual(skeleton);
    if (!FillStockSkeleton(block, skeleton, height, weight,
                           ManifestBodyType(manifest))) {
      XELOGW("Avatar: the stock skeleton couldn't be read");
    } else if (component_mask & kBodyComponent) {
      uint8_t* body_block =
          TranslateVirtual(xe::load_and_swap<uint32_t>(header + 0x4));
      uint8_t* body_root =
          TranslateVirtual(xe::load_and_swap<uint32_t>(body_block + 4));
      std::memcpy(body_root + 0x50, block + 0x10 + 0x50, 16);
      for (uint32_t k = 0; k < 4; ++k) {
        float s = xe::load_and_swap<float>(block + 0x10 + 0x50 + k * 4);
        StoreFloat(body_root + 0x30 + k * 4,
                   xe::load_and_swap<float>(body_root + 0x30 + k * 4) * s);
      }
    }
  }
  return built ? X_E_SUCCESS : X_E_FAIL;
}

}  // namespace avatar
}  // namespace xam
}  // namespace kernel
}  // namespace xe
