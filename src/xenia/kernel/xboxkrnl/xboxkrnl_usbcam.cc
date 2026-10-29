/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2026 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/kernel/kernel_state.h"
#include "xenia/kernel/util/shim_utils.h"
#include "xenia/kernel/xboxkrnl/xboxkrnl_private.h"
#include "xenia/xbox.h"

// The Xbox Live Vision camera (the USB webcam that came before the Kinect).
// No camera is ever connected here: XUsbcamCreate succeeds, as titles expect
// (Carcassonne gives up its initialization otherwise), the state stays "not
// connected" and everything that needs the camera says so. Seventeen Kinect
// titles on the shelf import these for their photo modes (The Gunstringer,
// Crashed Ice, Darts VS Zombies, ...) and only call them with a camera.

namespace xe {
namespace kernel {
namespace xboxkrnl {

dword_result_t XUsbcamCreate_entry(dword_t buffer,
                                   dword_t buffer_size,  // 0x4B000 640x480?
                                   lpdword_t handle_out) {
  // This function should return success.
  // It looks like it only allocates space for usbcam support.
  // returning error code might cause games to initialize incorrectly.
  // "Carcassonne" initalization function checks for result from this
  // function. If value is different than 0 instead of loading
  // rest of the game it returns from initalization function and tries
  // to run game normally which causes crash, due to uninitialized data.
  return X_STATUS_SUCCESS;
}
DECLARE_XBOXKRNL_EXPORT1(XUsbcamCreate, kNone, kStub);

dword_result_t XUsbcamDestroy_entry() { return X_ERROR_SUCCESS; }
DECLARE_XBOXKRNL_EXPORT1(XUsbcamDestroy, kNone, kImplemented);

enum class XUsbCamState : uint32_t {
  CamNotConnected = 0x00000000,
  CamInitilizied = 0x00000001,
  CamConnected = 0x00000002,
};

dword_result_t XUsbcamGetState_entry() {
  return static_cast<uint32_t>(XUsbCamState::CamNotConnected);
}
DECLARE_XBOXKRNL_EXPORT1(XUsbcamGetState, kNone, kImplemented);

dword_result_t XUsbcamReset_entry() { return X_ERROR_SUCCESS; }
DECLARE_XBOXKRNL_EXPORT1(XUsbcamReset, kNone, kImplemented);

// The capture configuration (resolution, format, frame rate) and the view
// (crop and zoom): nothing to configure without a camera.
dword_result_t XUsbcamSetConfig_entry(lpvoid_t config) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XBOXKRNL_EXPORT1(XUsbcamSetConfig, kNone, kImplemented);

dword_result_t XUsbcamGetConfig_entry(lpvoid_t config) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XBOXKRNL_EXPORT1(XUsbcamGetConfig, kNone, kImplemented);

dword_result_t XUsbcamSetView_entry(lpvoid_t view) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XBOXKRNL_EXPORT1(XUsbcamSetView, kNone, kImplemented);

dword_result_t XUsbcamGetView_entry(lpvoid_t view) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XBOXKRNL_EXPORT1(XUsbcamGetView, kNone, kImplemented);

dword_result_t XUsbcamSetCaptureMode_entry(dword_t mode) {
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XBOXKRNL_EXPORT1(XUsbcamSetCaptureMode, kNone, kImplemented);

// A frame or a still picture into the title's buffer, with an optional
// XOVERLAPPED: no frame ever comes.
dword_result_t XUsbcamReadFrame_entry(lpvoid_t buffer, dword_t buffer_size,
                                      lpdword_t timestamp,
                                      pointer_t<XAM_OVERLAPPED> overlapped) {
  if (overlapped) {
    kernel_state()->CompleteOverlappedImmediate(overlapped,
                                                X_ERROR_DEVICE_NOT_CONNECTED);
    return X_ERROR_IO_PENDING;
  }
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XBOXKRNL_EXPORT1(XUsbcamReadFrame, kNone, kImplemented);

dword_result_t XUsbcamSnapshot_entry(lpvoid_t buffer, dword_t buffer_size,
                                     pointer_t<XAM_OVERLAPPED> overlapped) {
  if (overlapped) {
    kernel_state()->CompleteOverlappedImmediate(overlapped,
                                                X_ERROR_DEVICE_NOT_CONNECTED);
    return X_ERROR_IO_PENDING;
  }
  return X_ERROR_DEVICE_NOT_CONNECTED;
}
DECLARE_XBOXKRNL_EXPORT1(XUsbcamSnapshot, kNone, kImplemented);

}  // namespace xboxkrnl
}  // namespace kernel
}  // namespace xe

DECLARE_XBOXKRNL_EMPTY_REGISTER_EXPORTS(Usbcam);
