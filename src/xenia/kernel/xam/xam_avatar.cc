/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2025 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/kernel/xam/xam_avatar.h"

#include <array>
#include <cstring>

#include "xenia/base/cvar.h"
#include "xenia/base/logging.h"
#include "xenia/base/math.h"
#include "xenia/kernel/util/shim_utils.h"
#include "xenia/kernel/xam/xam_avatar_assets.h"
#include "xenia/kernel/xam/xam_private.h"

// Titles don't expect XamAvatarInitialize to fail: a console always has the
// avatar library. Kinect Sports, for one, keeps going after a failure and
// crashes later, when a player steps in front of the sensor and it builds
// their avatar (0x82559B78 reads the table its initializer 0x82557F50 never
// filled). With the assets, animations and manifests implemented here, saying
// yes is the answer that matches the console.
DEFINE_bool(allow_avatar_initialization, true,
            "Let titles initialize the avatar library (XamAvatarInitialize). "
            "Off, titles that assume it works, like Kinect Sports, can crash.",
            "Kernel");

DECLARE_int32(user_language);

namespace xe {
namespace kernel {
namespace xam {

// Start/End
dword_result_t XamAvatarInitialize_entry(
    dword_t coordinate_system,  // 1, 2, 4, etc
    dword_t unk2,               // 0 or 1
    dword_t processor_number,   // for thread creation?
    lpdword_t function_ptrs,    // 20b, 5 pointers
    lpdword_t unk5,             // data segment ptr
    dword_t unk6                // flags - 0x00300000, 0x30, etc
) {
  if (kernel_state()->title_id() == kAvatarEditorID) {
    return X_STATUS_SUCCESS;
  }

  avatar::SetInitializeParameters(coordinate_system, unk2);
  return cvars::allow_avatar_initialization ? X_STATUS_SUCCESS : ~0u;
}
DECLARE_XAM_EXPORT1(XamAvatarInitialize, kAvatars, kImplemented);

void XamAvatarShutdown_entry() {
  // Calls XMsgStartIORequestEx(0xf3,0x600002,0,0,0,0).
  // in 12611 its XamUnloadSysApp(0xf2,1)
}
DECLARE_XAM_EXPORT1(XamAvatarShutdown, kAvatars, kSketchy);

// Get & Set
dword_result_t XamAvatarGetManifestLocalUser_entry(
    dword_t user_index, pointer_t<X_AVATAR_METADATA> avatar_metadata_ptr,
    pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  auto run = [=](uint32_t& extended_error, uint32_t& length) {
    extended_error = X_ERROR_SUCCESS;
    length = 0;

    if (user_index >= XUserMaxUserCount) {
      extended_error = X_E_INVALIDARG;
      return X_ERROR_INVALID_PARAMETER;
    }

    if (!avatar_metadata_ptr) {
      extended_error = X_E_INVALIDARG;
      return X_ERROR_INVALID_PARAMETER;
    }

    const auto user_profile =
        kernel_state()->xam_state()->GetUserProfile(user_index);

    if (!user_profile) {
      extended_error = X_E_NO_SUCH_USER;
      return X_ERROR_FUNCTION_FAILED;
    }

    const uint32_t avatar_info_id =
        static_cast<uint32_t>(UserSettingId::XPROFILE_GAMERCARD_AVATAR_INFO_1);

    X_USER_PROFILE_SETTING avatar_info_setting_data = {};

    avatar_info_setting_data.user_index = static_cast<uint32_t>(user_index);
    avatar_info_setting_data.setting_id = avatar_info_id;
    avatar_info_setting_data.data.type = X_USER_DATA_TYPE::BINARY;
    avatar_info_setting_data.data.data.binary.size = kMaxUserDataSize;
    avatar_info_setting_data.data.data.binary.ptr = 0;

    uint32_t avatar_metadata_address = avatar_metadata_ptr.guest_address();

    const bool has_avatar_info_setting =
        kernel_state()->xam_state()->user_tracker()->GetUserSetting(
            user_profile->xuid(), kDashboardID, avatar_info_id,
            &avatar_info_setting_data, avatar_metadata_address);

    // Every profile on a console has an avatar, the dashboard makes one. A
    // profile without one gets a random avatar, kept in the profile.
    if (!avatar_info_setting_data.data.data.binary.ptr) {
      uint8_t* manifest =
          kernel_memory()->TranslateVirtual<uint8_t*>(avatar_metadata_address);
      if (!avatar::RandomManifests(manifest, 1, 3)) {
        extended_error = X_E_FAIL;
        return X_ERROR_FUNCTION_FAILED;
      }
      X_USER_PROFILE_SETTING setting_data = {};
      setting_data.user_index = static_cast<uint32_t>(user_index);
      setting_data.setting_id = avatar_info_id;
      setting_data.data.type = X_USER_DATA_TYPE::BINARY;
      setting_data.data.data.binary.size = avatar::kManifestSize;
      setting_data.data.data.binary.ptr = avatar_metadata_address;
      const UserSetting setting = UserSetting(&setting_data);
      kernel_state()->xam_state()->user_tracker()->UpsertSetting(
          user_profile->xuid(), kDashboardID, &setting);
    }

    return X_ERROR_SUCCESS;
  };

  if (!overlapped_ptr) {
    uint32_t extended_error, length;
    X_RESULT result = run(extended_error, length);

    return result == X_ERROR_SUCCESS ? result : extended_error;
  }

  kernel_state()->CompleteOverlappedDeferredEx(run, overlapped_ptr);
  return X_ERROR_IO_PENDING;
}
DECLARE_XAM_EXPORT1(XamAvatarGetManifestLocalUser, kAvatars, kSketchy);

dword_result_t XamAvatarGetManifestsByXuid_entry(
    dword_t user_index, dword_t xuid_count, lpqword_t xuid, dword_t unk,
    dword_t avatar_info_ptr, pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  // set unk4 to 0 or 0x80990001 after

  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediate(overlapped_ptr,
                                                X_ERROR_SUCCESS);
    return X_ERROR_IO_PENDING;
  }

  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamAvatarGetManifestsByXuid, kAvatars, kStub)

dword_result_t XamAvatarGetAssetsResultSize_entry(
    dword_t avatar_component_mask, lpdword_t result_buffer_size_ptr,
    lpdword_t gpu_resource_buffer_size_ptr) {
  uint32_t result_size, gpu_resource_size;
  avatar::GetAssetsResultSize(static_cast<uint16_t>(avatar_component_mask),
                              &result_size, &gpu_resource_size);
  if (result_buffer_size_ptr) {
    *result_buffer_size_ptr = result_size;
  }
  if (gpu_resource_buffer_size_ptr) {
    *gpu_resource_buffer_size_ptr = gpu_resource_size;
  }
  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamAvatarGetAssetsResultSize, kAvatars, kImplemented);

dword_result_t XamAvatarGetAssets_entry(
    pointer_t<X_AVATAR_METADATA> avatar_metadata_ptr,
    dword_t avatar_component_mask, dword_t flags, lpdword_t result_buffer_ptr,
    lpdword_t gpu_resource_buffer_ptr,
    pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  // xam works on a copy of the manifest.
  std::array<uint8_t, avatar::kManifestSize> manifest;
  std::memcpy(
      manifest.data(),
      reinterpret_cast<const void*>(avatar_metadata_ptr.host_address()),
      manifest.size());
  X_HRESULT result = avatar::GetAssets(
      manifest.data(), static_cast<uint16_t>(avatar_component_mask), flags,
      result_buffer_ptr.guest_address(),
      gpu_resource_buffer_ptr.guest_address());
  // 58410907 doesn't crash if we return failure.
  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediateEx(
        overlapped_ptr,
        XSUCCEEDED(result) ? X_ERROR_SUCCESS : X_ERROR_FUNCTION_FAILED,
        result, 0);
    return X_ERROR_IO_PENDING;
  }
  return result;
}
DECLARE_XAM_EXPORT1(XamAvatarGetAssets, kAvatars, kSketchy);

dword_result_t XamAvatarSetCustomAsset_entry(
    dword_t buffer_size, lpdword_t asset_data_ptr, dword_t custom_color_count,
    lpdword_t custom_colors_ptr, dword_t slot_enabled,
    pointer_t<X_AVATAR_METADATA> avatar_metadata_ptr) {
  return avatar::SetCustomAsset(
      buffer_size, asset_data_ptr.guest_address(), custom_color_count,
      custom_colors_ptr.guest_address(), slot_enabled,
      avatar_metadata_ptr.guest_address());
}
DECLARE_XAM_EXPORT1(XamAvatarSetCustomAsset, kAvatars, kSketchy)

dword_result_t XamAvatarSetManifest_entry(
    dword_t user_index, dword_t avatar_info_ptr,
    pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  auto run = [=](uint32_t& extended_error, uint32_t& length) {
    extended_error = X_ERROR_SUCCESS;
    length = 0;
    // Update and save settings.
    const auto& user_profile =
        kernel_state()->xam_state()->GetUserProfile(user_index);

    // Skip writing data about users with id != 0 they're not supported
    if (!user_profile) {
      extended_error = X_E_NO_SUCH_USER;
      return X_ERROR_FUNCTION_FAILED;
    }

    const uint32_t avatar_info_id =
        static_cast<uint32_t>(UserSettingId::XPROFILE_GAMERCARD_AVATAR_INFO_1);

    X_USER_PROFILE_SETTING setting_data;
    setting_data.user_index = static_cast<uint32_t>(user_index);
    setting_data.setting_id = avatar_info_id;
    setting_data.data.type = X_USER_DATA_TYPE::BINARY;
    setting_data.data.data.binary.size = kMaxUserDataSize;
    setting_data.data.data.binary.ptr = static_cast<uint32_t>(avatar_info_ptr);

    const UserSetting setting = UserSetting(&setting_data);

    kernel_state()->xam_state()->user_tracker()->UpsertSetting(
        user_profile->xuid(), kDashboardID, &setting);

    return X_STATUS_SUCCESS;
  };

  if (!overlapped_ptr) {
    uint32_t extended_error, length;
    X_RESULT result = run(extended_error, length);

    return result == X_ERROR_SUCCESS ? result : extended_error;
  }

  kernel_state()->CompleteOverlappedDeferredEx(run, overlapped_ptr);
  return X_ERROR_IO_PENDING;
}
DECLARE_XAM_EXPORT1(XamAvatarSetManifest, kAvatars, kSketchy);

dword_result_t XamAvatarGetMetadataRandom_entry(
    dword_t body_type, dword_t avatars_count,
    pointer_t<X_AVATAR_METADATA> avatar_metadata_ptr,
    pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  // 0x8197FF28: body types 1 (male), 2 (female) or 3 (either), up to 100
  // avatars of kManifestSize bytes each.
  X_HRESULT result = X_E_INVALIDARG;
  if (!(body_type & ~3u) && avatars_count && avatars_count <= 100 &&
      avatar_metadata_ptr) {
    result = avatar::RandomManifests(
                 reinterpret_cast<uint8_t*>(avatar_metadata_ptr.host_address()),
                 avatars_count, body_type)
                 ? X_E_SUCCESS
                 : X_E_FAIL;
  }
  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediateEx(
        overlapped_ptr,
        XSUCCEEDED(result) ? X_ERROR_SUCCESS : X_ERROR_FUNCTION_FAILED,
        result, 0);
    return X_ERROR_IO_PENDING;
  }
  return result;
}
DECLARE_XAM_EXPORT1(XamAvatarGetMetadataRandom, kAvatars, kImplemented);

dword_result_t XamAvatarGetMetadataSignedOutProfileCount_entry(
    lpdword_t profile_count_ptr, pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediate(overlapped_ptr,
                                                X_ERROR_SUCCESS);
    return X_ERROR_IO_PENDING;
  }

  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamAvatarGetMetadataSignedOutProfileCount, kAvatars, kStub);

dword_result_t XamAvatarGetMetadataSignedOutProfile_entry(
    dword_t profile_index, pointer_t<X_AVATAR_METADATA> avatar_metadata_ptr,
    pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediate(overlapped_ptr,
                                                X_ERROR_SUCCESS);
    return X_ERROR_IO_PENDING;
  }

  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamAvatarGetMetadataSignedOutProfile, kAvatars, kStub);

dword_result_t XamAvatarManifestGetBodyType_entry(
    pointer_t<X_AVATAR_METADATA> avatar_metadata_ptr) {
  if (!avatar_metadata_ptr) {
    return static_cast<uint8_t>(X_AVATAR_BODY_TYPE::Unknown);
  }
  return avatar::ManifestBodyType(
      reinterpret_cast<const uint8_t*>(avatar_metadata_ptr.host_address()));
}
DECLARE_XAM_EXPORT1(XamAvatarManifestGetBodyType, kAvatars, kImplemented);

dword_result_t XamAvatarGetInstrumentation_entry(qword_t unk1, lpdword_t unk2) {
  /* Notes:
     - unk1 not used?
     - unk1 recieves values of 1, 2, and 6
     - mark implemented once confirmed first param not used and params named
  */
  if (unk2) {
    *unk2 = 0;
  }
  return 1;
}
DECLARE_XAM_EXPORT1(XamAvatarGetInstrumentation, kAvatars, kStub);

dword_result_t XamAvatarGetAssetIcon_entry(
    lpqword_t unk1, dword_t unk2, lpqword_t unk3, lpqword_t unk4,
    pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediate(overlapped_ptr,
                                                X_ERROR_SUCCESS);
    return X_ERROR_IO_PENDING;
  }

  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamAvatarGetAssetIcon, kAvatars, kStub);

dword_result_t XamAvatarGetAssetBinary_entry(
    lpvoid_t asset_metadata, dword_t unk2, dword_t unk3, dword_t unk4,
    pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediate(overlapped_ptr,
                                                X_ERROR_SUCCESS);
    return X_ERROR_IO_PENDING;
  }

  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamAvatarGetAssetBinary, kAvatars, kStub);

void XamAvatarGetInstalledAssetPackageDescription_entry(
    pointer_t<X_ASSET_ID> asset_id_ptr,
    pointer_t<XCONTENT_AGGREGATE_DATA>
        content_data_ptr  // pointer_t<XCONTENT_DATA_INTERNAL>
) {
  XCONTENT_AGGREGATE_DATA content_data;
  content_data.content_type = XContentType::kAvatarItem;
  content_data.xuid = 0;
  content_data.title_id = asset_id_ptr->title_id;
  std::string file_name =
      fmt::format("{:016X}{:08X}{:08X}", asset_id_ptr->data,
                  asset_id_ptr->data2, asset_id_ptr->title_id);
  content_data.set_file_name(file_name);
  *content_data_ptr = content_data;
  XELOGD("Looking for avatar asset: {:X}", file_name);
}
DECLARE_XAM_EXPORT1(XamAvatarGetInstalledAssetPackageDescription, kAvatars,
                    kSketchy);

void XamAvatarSetMocks_entry() {
  // No-op.
}
DECLARE_XAM_EXPORT1(XamAvatarSetMocks, kAvatars, kStub);

// Animation
const static std::map<uint64_t, std::string> XAnimationTypeMap = {
    // Animation Generic Stand
    {0x0040000000030003, "Animation Generic Stand 0"},
    {0x0040000000040003, "Animation Generic Stand 1"},
    {0x0040000000050003, "Animation Generic Stand 2"},
    {0x0040000000270003, "Animation Generic Stand 3"},
    {0x0040000000280003, "Animation Generic Stand 4"},
    {0x0040000000290003, "Animation Generic Stand 5"},
    {0x00400000002A0003, "Animation Generic Stand 6"},
    {0x00400000002B0003, "Animation Generic Stand 7"},
    // Animation Idle
    {0x0040000000130001, "Animation Male Idle Looks Around"},
    {0x0040000000140001, "Animation Male Idle Stretch"},
    {0x0040000000150001, "Animation Male Idle Shifts Weight"},
    {0x0040000000260001, "Animation Male Idle Checks Hand"},
    {0x0040000000090002, "Animation Female Idle Check Nails"},
    {0x00400000000A0002, "Animation Female Idle Looks Around"},
    {0x00400000000B0002, "Animation Female Idle Shifts Weight"},
    {0x00400000000C0002, "Animation Female Idle Fixes Shoe"},
};

// https://github.com/xenia-canary/xenia-canary/commit/212c99eee2724de15f471148d10197d89794ff32
dword_result_t XamAvatarLoadAnimation_entry(
    lpqword_t asset_id_ptr, dword_t flags, lpvoid_t output,
    pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  /* Notes:
      - unknown[4] & unknown[0] = 0x10000000
      - Calls XMsgStartIORequestEx(0xf3, 0x60000F, overlapped_ptr, stack1, 0x18,
     unknown) 0xf2 12611
  */
  assert_true(asset_id_ptr);

  std::string summary = "Request to load avatar animation: ";

  if (XAnimationTypeMap.contains(*asset_id_ptr)) {
    summary += XAnimationTypeMap.at(*asset_id_ptr);
  } else {
    summary += fmt::format("Unknown animation 0x{:016x}",
                           static_cast<uint64_t>(*asset_id_ptr));
  }

  XELOGD("{}", summary);

  // The ID is the 16-byte asset ID of the animation (mask 0x400000).
  X_HRESULT result =
      output ? avatar::LoadAnimation(reinterpret_cast<const uint8_t*>(
                                         asset_id_ptr.host_address()),
                                     output.guest_address())
             : X_E_INVALIDARG;
  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediateEx(
        overlapped_ptr,
        XSUCCEEDED(result) ? X_ERROR_SUCCESS : X_ERROR_FUNCTION_FAILED,
        result, 0);
    return X_ERROR_IO_PENDING;
  }
  return result;
}
DECLARE_XAM_EXPORT1(XamAvatarLoadAnimation, kAvatars, kSketchy);

dword_result_t XamAvatarGenerateMipMaps_entry(
    lpdword_t avatar_assets_ptr, dword_t flags, dword_t buffer_size,
    lpdword_t mip_map_buffer_ptr, pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediate(overlapped_ptr,
                                                X_ERROR_SUCCESS);
    return X_ERROR_IO_PENDING;
  }

  return X_STATUS_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamAvatarGenerateMipMaps, kAvatars, kStub);

// Enum: messages 0x60000C, 0x60000D and 0x60000E of the avatarcore app,
// which hands them to the TOC reader of the AvatarAssetPack (0x8196B578,
// 0x8196B6C0, 0x8196B528). The first two arguments of Begin aren't used by
// the reader; the mask, the body type and the flags make the filter.
dword_result_t XamAvatarBeginEnumAssets_entry(
    dword_t unk1, dword_t unk2, dword_t mask, word_t body_type, dword_t flags,
    pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  avatar::BeginAssetEnumeration(mask, uint8_t(body_type), flags,
                                uint32_t(cvars::user_language));
  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediate(overlapped_ptr,
                                                X_ERROR_SUCCESS);
    return X_ERROR_IO_PENDING;
  }
  return X_ERROR_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamAvatarBeginEnumAssets, kAvatars, kImplemented);

// count is in and out: how many 0x2C4-byte items fit in the buffer, then
// how many were written. 0 ends the walk; xam answers success then too.
dword_result_t XamAvatarEnumAssets_entry(
    lpvoid_t buffer, lpdword_t count,
    pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  X_RESULT result = X_ERROR_SUCCESS;
  if (!buffer || !count) {
    result = X_ERROR_INVALID_PARAMETER;
  } else {
    *count = avatar::EnumerateAssets(buffer.as<uint8_t*>(), *count);
  }
  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediate(overlapped_ptr, result);
    return X_ERROR_IO_PENDING;
  }
  return result;
}
DECLARE_XAM_EXPORT1(XamAvatarEnumAssets, kAvatars, kImplemented);

dword_result_t XamAvatarEndEnumAssets_entry(
    pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  avatar::EndAssetEnumeration();
  if (overlapped_ptr) {
    kernel_state()->CompleteOverlappedImmediate(overlapped_ptr,
                                                X_ERROR_SUCCESS);
    return X_ERROR_IO_PENDING;
  }
  return X_ERROR_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamAvatarEndEnumAssets, kAvatars, kImplemented);

// Other
dword_result_t XamAvatarWearNow_entry(
    qword_t unk1, lpdword_t unk2, pointer_t<XAM_OVERLAPPED> overlapped_ptr) {
  X_RESULT result = X_ERROR_SUCCESS;
  if (kernel_state()->title_id() == kAvatarEditorID) {
    /*
      - ops
    XamSendMessageToLoadedApps(0xffffffff8000000e,0xffffffff80050018,lVar5);
    XNotifyBroadcast(0xffffffff80050018,lVar5);
    if (overlapped_ptr) {
      XMsgCompleteIORequest(overlapped_ptr,0,0,0);
    }
    */
  } else {
    // buffer_ptr = concat(unk1, *unk2);
    //  XMsgStartIORequestEx(0xf3,0x600018,overlapped_ptr,&buffer_ptr,0x14,0);
    if (overlapped_ptr) {
      kernel_state()->CompleteOverlappedImmediate(overlapped_ptr, result);
      return X_ERROR_IO_PENDING;
    }
  }
  return result;
}
DECLARE_XAM_EXPORT1(XamAvatarWearNow, kAvatars, kStub);

dword_result_t XamAvatarReinstallAwardedAsset_entry(lpstring_t string_out_ptr,
                                                    dword_t string_size,
                                                    lpdword_t unk_ptr) {
  return X_ERROR_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamAvatarReinstallAwardedAsset, kAvatars, kStub);

}  // namespace xam
}  // namespace kernel
}  // namespace xe

DECLARE_XAM_EMPTY_REGISTER_EXPORTS(Avatar);
