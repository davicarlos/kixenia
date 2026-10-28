/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2026 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_KERNEL_XAM_UI_AVATAR_VIEWER_UI_H_
#define XENIA_KERNEL_XAM_UI_AVATAR_VIEWER_UI_H_

#include <array>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <cstdint>
#include <deque>
#include <memory>
#include <string>
#include <vector>

#include "third_party/imgui/imgui.h"
#include "xenia/kernel/xam/xam_avatar_assets.h"
#include "xenia/kernel/xam/xam_ui.h"
#include "xenia/ui/immediate_drawer.h"

namespace xe {
namespace kernel {
namespace xam {
namespace ui {

// A window to put an avatar together from the AvatarAssetPack of the running
// title and see what XamAvatarGetAssets builds for it: the meshes, drawn
// without textures in each mesh's color, and the textures, frame by frame.
// It builds through the same code titles call, into its own guest buffers.
class AvatarViewerUI final : public XamGamepadDialog {
 public:
  AvatarViewerUI(xe::ui::ImGuiDrawer* imgui_drawer,
                 xe::hid::InputSystem* input_system);
  ~AvatarViewerUI() override;

 protected:
  void OnDraw(ImGuiIO& io) override;

 private:
  static constexpr uint32_t kViewWidth = 360;
  static constexpr uint32_t kViewHeight = 480;

  // Parts of one kind the avatar wears one of (or none of, when optional).
  struct Group {
    const char* label;
    uint32_t mask;      // Catalog entries whose mask has these bits.
    bool face;          // A face texture slot rather than a component.
    uint32_t slot;      // Face texture slot.
    bool optional;
    std::vector<size_t> entries;  // Into catalog_.
    int choice = 0;               // Into entries, -1 for none.
  };
  static constexpr uint32_t kMaxUvSets = 7;
  struct Vertex {
    float position[3];
    float normal[3];
    float uv[kMaxUvSets][2];
  };
  // A material subset of type 1: the whole mesh drawn once more with a
  // texture, through one of its UV sets, over what's under it.
  struct Layer {
    uint32_t id;
    uint32_t texture;  // Into textures_.
    uint32_t uv_set;
    uint32_t flags;
  };
  struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
    std::vector<Layer> layers;
    // The colors the manifest painted, by subset id.
    std::vector<std::pair<uint32_t, uint32_t>> colors;
    uint32_t base_color;
  };
  struct Texture {
    std::string label;
    uint32_t format, width, height, slices, slice_size, data;
  };
  // A frame of a texture, decoded, for sampling.
  struct TextureImage {
    std::vector<uint8_t> rgba;
    uint32_t width = 0, height = 0;
  };
  // What the render thread draws: made whole on the UI thread, never
  // changed after, so the thread can read it without a lock.
  struct Scene {
    std::shared_ptr<const std::vector<Mesh>> meshes;
    std::vector<TextureImage> textures;
    std::array<float, 3> center = {};
    float radius = 1.0f;
    size_t triangle_count = 0;
  };
  struct View {
    float yaw, pitch, zoom;
    bool textured, tinted, smooth;
  };

  void FillGroups();
  void Build();
  void ReadResult(uint32_t result);
  static std::vector<uint8_t> Render(const Scene& scene, const View& view);
  void RequestRender();
  void RenderLoop();
  void UpdateTextureImages();
  void FreeBuffers();
  std::unique_ptr<xe::ui::ImmediateTexture> MakeImage(
      uint32_t width, uint32_t height, const std::vector<uint8_t>& rgba);

  bool has_opened_ = false;
  bool close_requested_ = false;
  std::vector<avatar::CatalogEntry> catalog_;
  std::vector<Group> groups_;
  int body_type_ = 1;
  std::array<float, 3> skin_ = {0.80f, 0.62f, 0.48f};
  std::array<float, 3> clothes_ = {0.25f, 0.35f, 0.60f};
  bool dirty_ = true;
  std::string status_;

  uint32_t result_buffer_ = 0;
  uint32_t gpu_buffer_ = 0;
  std::shared_ptr<std::vector<Mesh>> meshes_;
  size_t triangle_count_ = 0;
  std::array<float, 3> center_ = {};
  float radius_ = 1.0f;
  std::vector<Texture> textures_;
  std::vector<TextureImage> texture_frames_;

  // The render thread: takes the latest scene and view and leaves the image
  // for the UI thread to upload.
  std::thread render_thread_;
  std::mutex render_mutex_;
  std::condition_variable render_signal_;
  std::shared_ptr<const Scene> pending_scene_;
  View pending_view_ = {};
  bool render_requested_ = false;
  bool render_quit_ = false;
  std::vector<uint8_t> rendered_rgba_;
  bool rendered_ready_ = false;

  float yaw_ = 0.0f;
  float pitch_ = 0.0f;
  float zoom_ = 1.0f;
  int frame_ = 0;
  bool textured_ = true;
  bool tinted_ = true;
  bool smooth_ = true;
  bool view_dirty_ = true;
  bool textures_dirty_ = true;
  std::unique_ptr<xe::ui::ImmediateTexture> view_texture_;
  std::vector<std::unique_ptr<xe::ui::ImmediateTexture>> texture_images_;
  // Replaced textures, kept until frames that may still draw them are done.
  std::deque<std::unique_ptr<xe::ui::ImmediateTexture>> retired_textures_;
};

}  // namespace ui
}  // namespace xam
}  // namespace kernel
}  // namespace xe

#endif  // XENIA_KERNEL_XAM_UI_AVATAR_VIEWER_UI_H_
