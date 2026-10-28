/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2025 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_KERNEL_XAM_XAM_AVATAR_ASSETS_H_
#define XENIA_KERNEL_XAM_XAM_AVATAR_ASSETS_H_

#include <cstdint>
#include <string>
#include <vector>

#include "xenia/xbox.h"

namespace xe {
namespace kernel {
namespace xam {
namespace avatar {

constexpr uint32_t kManifestSize = 0x3E8;

// What the title passed to XamAvatarInitialize that changes how assets are
// built: the skinning version it expects and its coordinate system.
void SetInitializeParameters(uint32_t version, uint32_t coordinate_mode);

// The body type of a manifest: 1 for the stock male body, 2 for the stock
// female one, 0 for anything else.
uint32_t ManifestBodyType(const uint8_t* manifest);

// A part of the AvatarAssetPack the running title ships.
struct CatalogEntry {
  uint16_t index;      // Position in AvatarAssetPack.toc, the stock ID's index.
  uint32_t mask;       // Components it covers (or face texture/shape slot).
  uint8_t body_type;   // 1 male, 2 female, 3 both.
  std::string name;
};
// The parts with data, named in the language (an XC_LANGUAGE value, English
// when out of range). Empty when the title has no AvatarAssetPack.
std::vector<CatalogEntry> AssetCatalog(uint32_t language);

// The 16-byte ID of a stock part: {mask, index, body type} and the stock GUID.
void MakeStockId(uint8_t* id, uint32_t mask, uint16_t index,
                 uint16_t body_type);

// XamAvatarBeginEnumAssets, EnumAssets and EndEnumAssets, which walk the
// AvatarAssetPack through the TOC reader (0x8197C398, 0x8197CE48,
// 0x8197C420). The filter keeps the parts whose mask and body type share a
// bit with the given ones (flags 4 and 8: equal them), listed in the editor
// unless flag 2 asks for the hidden ones too; flag 1 fills the name in the
// language and flag 0x10 matches the second body byte of the entry. Each
// item is 0x2C4 bytes (0x8197E300): the stock ID, the entry, the name
// (UTF-16BE, up to 63 characters) and the pack's bit.
constexpr uint32_t kAssetItemSize = 0x2C4;
void BeginAssetEnumeration(uint32_t mask, uint8_t body_type, uint32_t flags,
                           uint32_t language);
// Writes up to count items to out and returns how many; 0 once the pack is
// walked, without an enumeration, or for a title initialized with a version
// below 4, as xam answers (0x8196B6C0).
uint32_t EnumerateAssets(uint8_t* out, uint32_t count);
void EndAssetEnumeration();

// Fills count manifests with random avatars of the allowed body types (1
// male, 2 female, 3 either), as xam's generator does. False without an
// AvatarAssetPack.
bool RandomManifests(uint8_t* out, uint32_t count, uint32_t body_types);

// Sizes of the two buffers XamAvatarGetAssets fills for a component mask.
void GetAssetsResultSize(uint16_t component_mask, uint32_t* result_size,
                         uint32_t* gpu_resource_size);

// Adds a title supplied asset to the manifest in guest memory, as xam's
// avatar app does.
X_HRESULT SetCustomAsset(uint32_t buffer_size, uint32_t asset_data_ptr,
                         uint32_t custom_color_count,
                         uint32_t custom_colors_ptr, uint32_t slot_enabled,
                         uint32_t manifest_ptr);

// Reads an animation into the title's animation (0x5990 bytes, its data
// where +0x598C points, with room for +0x5988 bytes), as xam does.
X_HRESULT LoadAnimation(const uint8_t* id, uint32_t output_ptr);

// Builds the components of the manifest into the title's buffers.
X_HRESULT GetAssets(const uint8_t* manifest, uint16_t component_mask,
                    uint32_t flags, uint32_t result_buffer_ptr,
                    uint32_t gpu_resource_buffer_ptr);

}  // namespace avatar
}  // namespace xam
}  // namespace kernel
}  // namespace xe

#endif  // XENIA_KERNEL_XAM_XAM_AVATAR_ASSETS_H_
