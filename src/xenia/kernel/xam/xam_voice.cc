/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2022 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include <atomic>
#include <cfloat>
#include <vector>

#include "xenia/base/memory.h"
#include "xenia/hid/kinect/usb_audio.h"
#include "xenia/kernel/kernel_state.h"
#include "xenia/kernel/kinect/kinect_audio.h"
#include "xenia/kernel/kinect/kinect_device.h"
#include "xenia/kernel/nui.h"
#include "xenia/kernel/util/shim_utils.h"
#include "xenia/kernel/xevent.h"
#include "xenia/kernel/xam/xam_private.h"
#include "xenia/xbox.h"

namespace xe {
namespace kernel {
namespace xam {

dword_result_t XamVoiceIsActiveProcess_entry() {
  // Returning 0 here will short-circuit a bunch of voice stuff.
  return 0;
}
DECLARE_XAM_EXPORT1(XamVoiceIsActiveProcess, kNone, kStub);

dword_result_t XamVoiceCreate_entry(dword_t user_index,
                                    dword_t max_attached_packets,  // 0xF
                                    lpdword_t out_voice_ptr) {
  // Null out the ptr.
  out_voice_ptr.Zero();
  return X_ERROR_ACCESS_DENIED;
}
DECLARE_XAM_EXPORT1(XamVoiceCreate, kNone, kStub);

dword_result_t XamVoiceClose_entry(lpunknown_t voice_ptr) { return 0; }
DECLARE_XAM_EXPORT1(XamVoiceClose, kNone, kStub);

dword_result_t XamVoiceHeadsetPresent_entry(lpunknown_t voice_ptr) { return 0; }
DECLARE_XAM_EXPORT1(XamVoiceHeadsetPresent, kNone, kStub);

dword_result_t XamVoiceSubmitPacket_entry(lpdword_t unk1, dword_t unk2,
                                          lpdword_t unk3) {
  // also may return 0xD000009D
  return 0x800700AA;
}
DECLARE_XAM_EXPORT1(XamVoiceSubmitPacket, kNone, kStub);

// 2 without the Kinect's microphone array (0x816FBBE8).
dword_result_t XamVoiceGetMicArrayStatus_entry() {
  return kernel_state()->nui()->audio()->microphones_available() ? 1 : 2;
}
DECLARE_XAM_EXPORT1(XamVoiceGetMicArrayStatus, kNone, kImplemented);

// Queues a request for the next 16 ms of the microphones, set by the event
// once filled; see KinectAudio for the request.
dword_result_t XamVoiceGetMicArrayAudioEx_entry(dword_t request,
                                                dword_t event) {
  return kernel_state()->nui()->audio()->ReadMicrophones(request, event);
}
DECLARE_XAM_EXPORT1(XamVoiceGetMicArrayAudioEx, kNone, kImplemented);

// Windows that would overrun are dropped on the host and missing ones come
// as silence, so the title never sees an underrun (McaDeviceRequest 0x20,
// 0x816FBD50).
dword_result_t XamVoiceGetMicArrayUnderrunStatus_entry(lpdword_t flags,
                                                       lpdword_t count) {
  if (flags) {
    *flags = 0;
  }
  if (count) {
    *count = 0;
  }
  return 0x10000000;
}
DECLARE_XAM_EXPORT1(XamVoiceGetMicArrayUnderrunStatus, kNone, kImplemented);

// Points the array's beam, in radians from straight ahead (the sensor forms
// beams within 50 degrees either way), or back to the sensor's choice with
// FLT_MAX: CEMD message 5 to its audio processing (0x816FBD60).
dword_result_t XamVoiceSetMicArrayBeamAngle_entry(double_t angle) {
  const float radians = float(angle);
  constexpr float kMaxBeamAngle = 0.873f;
  if (radians != FLT_MAX &&
      (radians < -kMaxBeamAngle || radians > kMaxBeamAngle)) {
    return X_E_INVALIDARG;
  }
  std::vector<uint8_t> message(16, 0);
  xe::store<uint32_t>(message.data(),
                      hid::kinect::UsbAudio::kAudioMessageMagic);
  xe::store<uint32_t>(message.data() + 8,
                      hid::kinect::UsbAudio::kAudioMessageBeamAngle);
  xe::store<float>(message.data() + 12, radians);
  return kernel_state()->nui()->device()->SendAudioMessage(message)
             ? X_E_SUCCESS
             : X_E_FAIL;
}
DECLARE_XAM_EXPORT1(XamVoiceSetMicArrayBeamAngle, kNone, kImplemented);

// Who the title considers idle at the array; xam keeps the mask for its own
// voice processing (0x816FB9C0), and the microphones read the same.
dword_result_t XamVoiceSetMicArrayIdleUsers_entry(dword_t idle_users,
                                                  dword_t signal) {
  return X_ERROR_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamVoiceSetMicArrayIdleUsers, kNone, kImplemented);

// The older form of XamVoiceGetMicArrayAudioEx (0x816FBCA0): the title
// gives an event handle instead of the event, and xam fills the request's
// words at +0x2C to +0x3C with 0, 5, 5, 5, 5 before handing it to the same
// worker (0x81983128).
dword_result_t XamVoiceGetMicArrayAudio_entry(lpdword_t request,
                                              dword_t event_handle) {
  if (!request) {
    return X_E_INVALIDARG;
  }
  request[0xB] = 0;
  for (uint32_t i = 0xC; i <= 0xF; ++i) {
    request[i] = 5;
  }
  uint32_t event_ptr = 0;
  if (event_handle) {
    auto event = kernel_state()->object_table()->LookupObject<XEvent>(
        event_handle);
    if (!event) {
      return X_STATUS_INVALID_HANDLE;
    }
    event_ptr = event->guest_object();
  }
  return kernel_state()->nui()->audio()->ReadMicrophones(
      request.guest_address(), event_ptr);
}
DECLARE_XAM_EXPORT1(XamVoiceGetMicArrayAudio, kNone, kImplemented);

// 0x816FBA08: muting notes the time (0x81AACBE8) so that the chat's per user
// talk timers (0x81AACA50) skip the muted stretch when unmuted; nothing
// here keeps those timers, so only the status check remains.
dword_result_t XamVoiceMuteMicArray_entry(dword_t mute) {
  return kernel_state()->nui()->audio()->microphones_available() ? 0 : 2;
}
DECLARE_XAM_EXPORT1(XamVoiceMuteMicArray, kNone, kSketchy);

// Headsets on controllers go through the kernel's XVoiced driver
// (XVoicedGetDirectionalData, 0x327, and the battery query); there are
// none, so they report no device.
dword_result_t XamVoiceGetDirectionalData_entry(dword_t unknown,
                                                lpvoid_t data) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamVoiceGetDirectionalData, kNone, kImplemented);

dword_result_t XamVoiceGetBatteryStatus_entry(dword_t user_index,
                                              lpvoid_t status) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XAM_EXPORT1(XamVoiceGetBatteryStatus, kNone, kImplemented);

// 0x816FBB28: caches, for each of the four users, a voice level from the
// sign-in state and the privileges 0xFC and 0xFB (0 signed out, 10 signed
// in, 0x1E or 0x14 by the privileges); it's read back by xam's own chat
// code, which isn't here.
void XamVoiceRecordUserPrivileges_entry() {}
DECLARE_XAM_EXPORT1(XamVoiceRecordUserPrivileges, kNone, kImplemented);

// 0x817F8C80: swaps the routine xam calls with captured audio (0x81AD1264)
// and returns the previous one.
dword_result_t XamVoiceSetAudioCaptureRoutine_entry(dword_t routine) {
  static std::atomic<uint32_t> capture_routine{0};
  return capture_routine.exchange(routine);
}
DECLARE_XAM_EXPORT1(XamVoiceSetAudioCaptureRoutine, kNone, kImplemented);

// 0x816FBE10: xam's localized string 100, the microphone array's name in
// the title's language; here always English.
dword_result_t XamVoiceGetMicArrayFilenameDesc_entry() {
  static uint32_t name_ptr = 0;
  if (!name_ptr) {
    static const char16_t kName[] = u"Kinect Sensor Microphone Array";
    name_ptr = kernel_memory()->SystemHeapAlloc(sizeof(kName));
    auto* out = kernel_memory()->TranslateVirtual<uint8_t*>(name_ptr);
    for (size_t i = 0; i < xe::countof(kName); ++i) {
      xe::store_and_swap<uint16_t>(out + i * 2, uint16_t(kName[i]));
    }
  }
  return name_ptr;
}
DECLARE_XAM_EXPORT1(XamVoiceGetMicArrayFilenameDesc, kNone, kSketchy);

// 0x816C3750: shows the system's dialog 0x2100E asking to turn the
// microphone array off (an 0x88 byte request with 0xD as its first word)
// and returns what the dialog answers, 5 when it can't be shown. No dialog
// here: the array stays as it is.
dword_result_t XamVoiceDisableMicArray_entry(dword_t user_index) {
  return X_ERROR_ACCESS_DENIED;
}
DECLARE_XAM_EXPORT1(XamVoiceDisableMicArray, kNone, kSketchy);

}  // namespace xam
}  // namespace kernel
}  // namespace xe

DECLARE_XAM_EMPTY_REGISTER_EXPORTS(Voice);
