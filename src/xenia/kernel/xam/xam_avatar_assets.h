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

// Sizes of the two buffers XamAvatarGetAssets fills for a component mask.
void GetAssetsResultSize(uint16_t component_mask, uint32_t* result_size,
                         uint32_t* gpu_resource_size);

// Adds a title supplied asset to the manifest in guest memory, as xam's
// avatar app does.
X_HRESULT SetCustomAsset(uint32_t buffer_size, uint32_t asset_data_ptr,
                         uint32_t custom_color_count,
                         uint32_t custom_colors_ptr, uint32_t slot_enabled,
                         uint32_t manifest_ptr);

// Builds the components of the manifest into the title's buffers.
X_HRESULT GetAssets(const uint8_t* manifest, uint16_t component_mask,
                    uint32_t flags, uint32_t result_buffer_ptr,
                    uint32_t gpu_resource_buffer_ptr);

}  // namespace avatar
}  // namespace xam
}  // namespace kernel
}  // namespace xe

#endif  // XENIA_KERNEL_XAM_XAM_AVATAR_ASSETS_H_
