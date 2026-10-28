/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2026 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/kernel/xam/ui/avatar_viewer_ui.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>

#include "third_party/imgui/imgui.h"
#include "xenia/base/byte_order.h"
#include "xenia/base/cvar.h"
#include "xenia/base/math.h"
#include "xenia/gpu/texture_address.h"
#include "xenia/kernel/kernel_state.h"
#include "xenia/kernel/util/shim_utils.h"
#include "xenia/ui/imgui_drawer.h"

DECLARE_int32(user_language);

namespace xe {
namespace kernel {
namespace xam {
namespace ui {

namespace {

constexpr size_t kRetiredTextureCount = 128;
constexpr uint32_t kMaxWearables = 13;
// The manifest's layout (see xam_avatar_assets.cc).
constexpr uint32_t kBodyA = 0x120;
constexpr uint32_t kBodyB = 0x140;
constexpr uint32_t kWearables = 0x160;
constexpr uint32_t kFaceTextures = 0x3C;
constexpr uint32_t kColors = 0xFC;
constexpr uint32_t kHeadMask = 0x1;
constexpr uint32_t kBodyMask = 0x2;
// The component bits a wearable covers; the lowest one picks its group.
constexpr uint32_t kWearableBits = 0xFFC;

uint32_t Load32(uint32_t address) {
  return xe::load_and_swap<uint32_t>(
      kernel_memory()->TranslateVirtual<uint8_t*>(address));
}

uint32_t ToArgb(const std::array<float, 3>& rgb) {
  auto c = [](float v) {
    return uint32_t(std::clamp(v, 0.0f, 1.0f) * 255.0f + 0.5f);
  };
  return 0xFF000000 | (c(rgb[0]) << 16) | (c(rgb[1]) << 8) | c(rgb[2]);
}

std::array<uint8_t, 4> Rgb565(uint16_t c) {
  return {uint8_t(((c >> 11) & 31) * 255 / 31),
          uint8_t(((c >> 5) & 63) * 255 / 63), uint8_t((c & 31) * 255 / 31),
          255};
}

// One 4x4 DXT1 or DXT5 block, already in little-endian order, to RGBA.
void DecodeBlock(const uint8_t* block, bool dxt5, uint8_t* out,
                 uint32_t out_pitch) {
  uint8_t alpha[16];
  std::fill(std::begin(alpha), std::end(alpha), uint8_t(255));
  if (dxt5) {
    uint32_t a0 = block[0], a1 = block[1];
    uint32_t table[8] = {a0, a1};
    for (uint32_t i = 0; i < 6; ++i) {
      table[2 + i] = a0 > a1 ? ((6 - i) * a0 + (1 + i) * a1) / 7
                     : i < 4 ? ((4 - i) * a0 + (1 + i) * a1) / 5
                     : i == 4 ? 0
                              : 255;
    }
    uint64_t bits = 0;
    for (uint32_t i = 0; i < 6; ++i) {
      bits |= uint64_t(block[2 + i]) << (8 * i);
    }
    for (uint32_t i = 0; i < 16; ++i) {
      alpha[i] = uint8_t(table[(bits >> (3 * i)) & 7]);
    }
    block += 8;
  }
  uint16_t c0 = uint16_t(block[0] | (block[1] << 8));
  uint16_t c1 = uint16_t(block[2] | (block[3] << 8));
  uint32_t indices =
      block[4] | (block[5] << 8) | (block[6] << 16) |
      (uint32_t(block[7]) << 24);
  auto p0 = Rgb565(c0), p1 = Rgb565(c1);
  std::array<uint8_t, 4> palette[4] = {p0, p1};
  for (uint32_t k = 0; k < 3; ++k) {
    if (dxt5 || c0 > c1) {
      palette[2][k] = uint8_t((2 * p0[k] + p1[k]) / 3);
      palette[3][k] = uint8_t((p0[k] + 2 * p1[k]) / 3);
    } else {
      palette[2][k] = uint8_t((p0[k] + p1[k]) / 2);
      palette[3][k] = 0;
    }
  }
  palette[2][3] = 255;
  palette[3][3] = (dxt5 || c0 > c1) ? 255 : 0;
  for (uint32_t i = 0; i < 16; ++i) {
    uint8_t* texel = out + (i / 4) * out_pitch + (i % 4) * 4;
    std::memcpy(texel, palette[(indices >> (2 * i)) & 3].data(), 4);
    if (dxt5) {
      texel[3] = alpha[i];
    }
  }
}

}  // namespace

AvatarViewerUI::AvatarViewerUI(xe::ui::ImGuiDrawer* imgui_drawer,
                               xe::hid::InputSystem* input_system)
    : XamGamepadDialog(imgui_drawer, input_system) {
  catalog_ = avatar::AssetCatalog(uint32_t(cvars::user_language));
  FillGroups();
}

AvatarViewerUI::~AvatarViewerUI() { FreeBuffers(); }

void AvatarViewerUI::FillGroups() {
  groups_ = {
      {"Hair", 0x4, false, 0, false},
      {"Top", 0x8, false, 0, false},
      {"Bottom", 0x10, false, 0, false},
      {"Shoes", 0x20, false, 0, false},
      {"Hat", 0x40, false, 0, true},
      {"Gloves", 0x80, false, 0, true},
      {"Glasses", 0x100, false, 0, true},
      {"Wrist", 0x200, false, 0, true},
      {"Earrings", 0x400, false, 0, true},
      {"Ring", 0x800, false, 0, true},
      {"Eyes", 0x2000, true, 1, false},
      {"Eyebrows", 0x4000, true, 2, false},
      {"Mouth", 0x8000, true, 0, false},
      {"Facial hair", 0x10000, true, 3, true},
      {"Eye shadow", 0x40000, true, 4, true},
      {"Face paint", 0x20000, true, 5, true},
  };
  for (size_t i = 0; i < catalog_.size(); ++i) {
    const auto& part = catalog_[i];
    if (!(part.body_type & body_type_)) {
      continue;
    }
    uint32_t wearable = part.mask & kWearableBits;
    for (auto& group : groups_) {
      bool belongs = group.face ? part.mask == group.mask
                                : wearable && (wearable & (0u - wearable)) ==
                                                  group.mask;
      if (belongs) {
        group.entries.push_back(i);
        break;
      }
    }
  }
  for (auto& group : groups_) {
    group.choice = (group.optional || group.entries.empty()) ? -1 : 0;
  }
  dirty_ = true;
}

void AvatarViewerUI::FreeBuffers() {
  auto* memory = kernel_memory();
  if (result_buffer_) {
    memory->SystemHeapFree(result_buffer_);
    result_buffer_ = 0;
  }
  if (gpu_buffer_) {
    memory->SystemHeapFree(gpu_buffer_);
    gpu_buffer_ = 0;
  }
}

// Writes a manifest the way xam lays it out and builds it with
// XamAvatarGetAssets' own code.
void AvatarViewerUI::Build() {
  dirty_ = false;
  view_dirty_ = true;
  textures_dirty_ = true;
  triangles_.clear();
  textures_.clear();
  FreeBuffers();

  uint8_t manifest[avatar::kManifestSize] = {};
  avatar::MakeStockId(manifest + kBodyA, kBodyMask, body_type_ == 1 ? 0 : 1,
                      uint16_t(body_type_));
  xe::store_and_swap<uint16_t>(manifest + kBodyA + 0x10, kBodyMask);
  for (const auto& part : catalog_) {
    if (part.mask == kHeadMask) {
      avatar::MakeStockId(manifest + kBodyB, kHeadMask, part.index,
                          part.body_type);
      xe::store_and_swap<uint16_t>(manifest + kBodyB + 0x10, kHeadMask);
      break;
    }
  }
  uint32_t used = kHeadMask | kBodyMask;
  uint32_t wearables = 0;
  for (const auto& group : groups_) {
    if (group.choice < 0) {
      continue;
    }
    const auto& part = catalog_[group.entries[group.choice]];
    if (group.face) {
      uint8_t* record = manifest + kFaceTextures + group.slot * 0x20;
      avatar::MakeStockId(record, part.mask, part.index, part.body_type);
      // Face texture records end with 1.0f, as xam writes them.
      xe::store_and_swap<float>(record + 0x10, 1.0f);
      continue;
    }
    uint16_t mask = uint16_t(part.mask);
    if ((mask & used) || wearables >= kMaxWearables) {
      continue;  // A dress already covers the bottom, and so on.
    }
    used |= mask;
    uint8_t* record = manifest + kWearables + wearables++ * 0x20;
    avatar::MakeStockId(record, part.mask, part.index, part.body_type);
    xe::store_and_swap<uint16_t>(record + 0x10, mask);
    for (uint32_t c = 0; c < 3; ++c) {
      xe::store_and_swap<uint32_t>(record + 0x14 + c * 4, ToArgb(clothes_));
    }
  }
  for (uint32_t i = 0; i < 9; ++i) {
    xe::store_and_swap<uint32_t>(manifest + kColors + i * 4,
                                 i ? 0xFF606060 : ToArgb(skin_));
  }

  uint16_t mask = uint16_t(used & 0xFFF);
  uint32_t result_size, gpu_size;
  avatar::GetAssetsResultSize(mask, &result_size, &gpu_size);
  auto* memory = kernel_memory();
  result_buffer_ = memory->SystemHeapAlloc(result_size, 0x10);
  gpu_buffer_ = memory->SystemHeapAlloc(gpu_size, 0x1000);
  if (!result_buffer_ || !gpu_buffer_) {
    status_ = "Out of guest memory for the avatar.";
    FreeBuffers();
    return;
  }
  X_HRESULT result =
      avatar::GetAssets(manifest, mask, 0, result_buffer_, gpu_buffer_);
  if (XFAILED(result)) {
    status_ = "XamAvatarGetAssets failed; see the log.";
    return;
  }
  ReadResult(result_buffer_);
}

// Walks the result header, the component tables and their meshes and
// textures the way a title would.
void AvatarViewerUI::ReadResult(uint32_t result) {
  auto* memory = kernel_memory();
  uint32_t count = Load32(result + 0x8);
  uint32_t records = Load32(result + 0xC);
  uint32_t components = Load32(result + 0x10);
  std::array<float, 3> lo = {std::numeric_limits<float>::max(),
                             std::numeric_limits<float>::max(),
                             std::numeric_limits<float>::max()};
  std::array<float, 3> hi = {-lo[0], -lo[1], -lo[2]};
  for (uint32_t c = 0; c < count; ++c) {
    uint32_t component = components + c * 0x34;
    uint16_t index = xe::load_and_swap<uint16_t>(
        memory->TranslateVirtual<uint8_t*>(records + c * 0x20 + 4));
    std::string name = "?";
    for (const auto& part : catalog_) {
      if (part.index == index) {
        name = part.name;
        break;
      }
    }
    uint32_t meshes = Load32(component + 0x14);
    uint32_t mesh_base = Load32(component + 0x2C);
    for (uint32_t m = 0; m < meshes; ++m) {
      uint32_t mesh = mesh_base + m * 0x200;
      uint32_t triangles = Load32(mesh + 0x1E4);
      uint32_t stride = Load32(mesh + 0x1F0);
      const uint8_t* vertices =
          memory->TranslateVirtual<uint8_t*>(Load32(mesh + 0x1F8));
      const uint8_t* indices =
          memory->TranslateVirtual<uint8_t*>(Load32(mesh + 0x1FC));
      uint32_t vertex_count = Load32(mesh + 0x1E8);
      // The mesh's first painted color, or the clothes color.
      uint32_t color = ToArgb(clothes_);
      for (uint32_t s = 0; s < 20; ++s) {
        uint32_t subset = mesh + 4 + s * 0x18;
        if (Load32(subset) == 1 || !Load32(subset + 4)) {
          continue;
        }
        std::array<float, 3> rgb;
        for (uint32_t k = 0; k < 3; ++k) {
          rgb[k] = xe::load_and_swap<float>(
              memory->TranslateVirtual<uint8_t*>(subset + 8 + k * 4));
        }
        if (rgb[0] + rgb[1] + rgb[2] > 0.0f) {
          color = ToArgb(rgb);
          break;
        }
      }
      for (uint32_t t = 0; t < triangles; ++t) {
        Triangle triangle;
        triangle.color = color;
        bool valid = true;
        for (uint32_t k = 0; k < 3; ++k) {
          uint32_t v = xe::load_and_swap<uint16_t>(indices + (t * 3 + k) * 2);
          if (v >= vertex_count) {
            valid = false;
            break;
          }
          for (uint32_t a = 0; a < 3; ++a) {
            float value =
                xe::load_and_swap<float>(vertices + v * stride + a * 4);
            triangle.vertices[k * 3 + a] = value;
            lo[a] = std::min(lo[a], value);
            hi[a] = std::max(hi[a], value);
          }
        }
        if (valid) {
          triangles_.push_back(triangle);
        }
      }
    }
    uint32_t texture_count = Load32(component + 0x18);
    uint32_t texture_base = Load32(component + 0x30);
    for (uint32_t t = 0; t < texture_count; ++t) {
      uint32_t record = texture_base + t * 0x2C;
      Texture texture;
      texture.format = Load32(record);
      texture.width = Load32(record + 4);
      texture.height = Load32(record + 8);
      texture.slices = std::max(Load32(record + 0x20), 1u);
      texture.slice_size = Load32(record + 0x14);
      texture.data = Load32(record + 0x24);
      texture.label = name + " #" + std::to_string(t);
      if (texture.data && texture.width && texture.height &&
          texture.width <= 1024 && texture.height <= 1024) {
        textures_.push_back(texture);
      }
    }
  }
  if (triangles_.empty()) {
    status_ = "Built " + std::to_string(count) + " components, no meshes.";
    return;
  }
  radius_ = 0.0f;
  for (uint32_t a = 0; a < 3; ++a) {
    center_[a] = (lo[a] + hi[a]) * 0.5f;
    radius_ = std::max(radius_, (hi[a] - lo[a]) * 0.5f);
  }
  status_ = "Built " + std::to_string(count) + " components, " +
            std::to_string(triangles_.size()) + " triangles, " +
            std::to_string(textures_.size()) + " textures.";
}

// Draws the triangles with a depth buffer, lit by their face normals.
void AvatarViewerUI::Render() {
  view_dirty_ = false;
  view_rgba_.assign(size_t(kViewWidth) * kViewHeight * 4, 0);
  for (size_t i = 0; i < view_rgba_.size(); i += 4) {
    view_rgba_[i] = 32;
    view_rgba_[i + 1] = 34;
    view_rgba_[i + 2] = 40;
    view_rgba_[i + 3] = 255;
  }
  depth_.assign(size_t(kViewWidth) * kViewHeight,
                -std::numeric_limits<float>::max());
  if (triangles_.empty() || radius_ <= 0.0f) {
    return;
  }
  float cy = std::cos(yaw_), sy = std::sin(yaw_);
  float cp = std::cos(pitch_), sp = std::sin(pitch_);
  // The avatar's largest extent fills nine tenths of the view's height.
  float scale = 0.45f * float(kViewHeight) / radius_ * zoom_;
  auto transform = [&](const float* p, float* out) {
    float x = p[0] - center_[0], y = p[1] - center_[1], z = p[2] - center_[2];
    float x1 = x * cy + z * sy, z1 = -x * sy + z * cy;
    float y1 = y * cp - z1 * sp, z2 = y * sp + z1 * cp;
    out[0] = float(kViewWidth) * 0.5f + x1 * scale;
    out[1] = float(kViewHeight) * 0.5f - y1 * scale;
    out[2] = z2 * scale;
  };
  for (const auto& triangle : triangles_) {
    float p[3][3];
    for (uint32_t k = 0; k < 3; ++k) {
      transform(&triangle.vertices[k * 3], p[k]);
    }
    float ux = p[1][0] - p[0][0], uy = p[1][1] - p[0][1],
          uz = p[1][2] - p[0][2];
    float vx = p[2][0] - p[0][0], vy = p[2][1] - p[0][1],
          vz = p[2][2] - p[0][2];
    // The face normal in view space (pixels, y down), turned to the viewer,
    // lit from above, the left and the front.
    float nx = uy * vz - uz * vy, ny = uz * vx - ux * vz,
          nz = ux * vy - uy * vx;
    float length = std::sqrt(nx * nx + ny * ny + nz * nz);
    float light = 0.25f;
    if (length > 0.0f) {
      if (nz < 0.0f) {
        nx = -nx, ny = -ny, nz = -nz;
      }
      float facing = (-0.35f * nx - 0.5f * ny + 0.8f * nz) / (length * 1.0055f);
      light += 0.75f * std::max(facing, 0.0f);
    }
    uint8_t rgb[3] = {uint8_t(((triangle.color >> 16) & 0xFF) * light),
                      uint8_t(((triangle.color >> 8) & 0xFF) * light),
                      uint8_t((triangle.color & 0xFF) * light)};
    float area = ux * vy - uy * vx;
    if (std::abs(area) < 1e-6f) {
      continue;
    }
    int x0 =
        std::max(0, int(std::floor(std::min({p[0][0], p[1][0], p[2][0]}))));
    int x1 = std::min(int(kViewWidth) - 1,
                      int(std::ceil(std::max({p[0][0], p[1][0], p[2][0]}))));
    int y0 =
        std::max(0, int(std::floor(std::min({p[0][1], p[1][1], p[2][1]}))));
    int y1 = std::min(int(kViewHeight) - 1,
                      int(std::ceil(std::max({p[0][1], p[1][1], p[2][1]}))));
    for (int y = y0; y <= y1; ++y) {
      for (int x = x0; x <= x1; ++x) {
        float px = float(x) + 0.5f, py = float(y) + 0.5f;
        float w0 = ((p[1][0] - px) * (p[2][1] - py) -
                    (p[1][1] - py) * (p[2][0] - px)) / area;
        float w1 = ((p[2][0] - px) * (p[0][1] - py) -
                    (p[2][1] - py) * (p[0][0] - px)) / area;
        float w2 = 1.0f - w0 - w1;
        if (w0 < 0.0f || w1 < 0.0f || w2 < 0.0f) {
          continue;
        }
        float z = w0 * p[0][2] + w1 * p[1][2] + w2 * p[2][2];
        size_t pixel = size_t(y) * kViewWidth + x;
        if (z <= depth_[pixel]) {
          continue;
        }
        depth_[pixel] = z;
        std::memcpy(&view_rgba_[pixel * 4], rgb, 3);
      }
    }
  }
}

std::unique_ptr<xe::ui::ImmediateTexture> AvatarViewerUI::MakeImage(
    uint32_t width, uint32_t height, const std::vector<uint8_t>& rgba) {
  return imgui_drawer()->CreateRgbaTexture(width, height, rgba.data());
}

// Decodes the current frame of every texture: DXT1 or DXT5 blocks, stored
// with 16-bit words byte swapped, tiled when the format says so.
void AvatarViewerUI::UpdateTextureImages() {
  textures_dirty_ = false;
  for (auto& image : texture_images_) {
    if (image) {
      retired_textures_.push_back(std::move(image));
    }
  }
  texture_images_.clear();
  auto* memory = kernel_memory();
  for (const auto& texture : textures_) {
    bool dxt5 = (texture.format & 0x3F) != 0x12;
    uint32_t block_bytes = dxt5 ? 16 : 8;
    uint32_t blocks_wide = std::max(texture.width / 4, 1u);
    uint32_t blocks_high = std::max(texture.height / 4, 1u);
    uint32_t pitch_blocks = xe::align(blocks_wide, 32u);
    uint32_t slice = std::min(uint32_t(frame_), texture.slices - 1);
    bool tiled = texture.format & 0x100;
    const uint8_t* data = memory->TranslateVirtual<uint8_t*>(texture.data) +
                          size_t(slice) * texture.slice_size;
    std::vector<uint8_t> rgba(size_t(blocks_wide) * 4 * blocks_high * 4 * 4);
    uint8_t block[16];
    for (uint32_t by = 0; by < blocks_high; ++by) {
      for (uint32_t bx = 0; bx < blocks_wide; ++bx) {
        size_t offset =
            tiled ? size_t(gpu::texture_address::Tiled2D(
                        int32_t(bx), int32_t(by), pitch_blocks,
                        dxt5 ? 4 : 3))
                  : (size_t(by) * pitch_blocks + bx) * block_bytes;
        for (uint32_t i = 0; i < block_bytes; i += 2) {
          block[i] = data[offset + i + 1];
          block[i + 1] = data[offset + i];
        }
        DecodeBlock(block, dxt5,
                    &rgba[(size_t(by) * 4 * blocks_wide * 4 + bx * 4) * 4],
                    blocks_wide * 4 * 4);
      }
    }
    texture_images_.push_back(
        MakeImage(blocks_wide * 4, blocks_high * 4, rgba));
  }
}

void AvatarViewerUI::OnDraw(ImGuiIO& io) {
  if (close_requested_) {
    Close();
    return;
  }
  if (dirty_ && !catalog_.empty()) {
    Build();
  }
  if (view_dirty_) {
    Render();
    if (view_texture_) {
      retired_textures_.push_back(std::move(view_texture_));
    }
    view_texture_ = MakeImage(kViewWidth, kViewHeight, view_rgba_);
  }
  if (textures_dirty_) {
    UpdateTextureImages();
  }
  while (retired_textures_.size() > kRetiredTextureCount) {
    retired_textures_.pop_front();
  }

  if (!has_opened_) {
    ImGui::SetNextWindowPos(
        ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    has_opened_ = true;
  }
  bool open = true;
  if (!ImGui::Begin("Avatar Viewer##avatar_viewer", &open,
                    ImGuiWindowFlags_AlwaysAutoResize |
                        ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  if (catalog_.empty()) {
    ImGui::TextWrapped(
        "The running title has no AvatarAssetPack in its folder, or it's "
        "inside a disc image.");
  } else {
    ImGui::BeginChild("##parts", ImVec2(320.0f, float(kViewHeight)), true);
    int body = body_type_;
    ImGui::RadioButton("Male", &body, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Female", &body, 2);
    if (body != body_type_) {
      body_type_ = body;
      FillGroups();
    }
    dirty_ |= ImGui::ColorEdit3("Skin", skin_.data(),
                                ImGuiColorEditFlags_NoInputs);
    ImGui::SameLine();
    dirty_ |= ImGui::ColorEdit3("Clothes", clothes_.data(),
                                ImGuiColorEditFlags_NoInputs);
    ImGui::Separator();
    for (auto& group : groups_) {
      const char* current =
          group.choice < 0 ? "None"
                           : catalog_[group.entries[group.choice]].name.c_str();
      ImGui::SetNextItemWidth(200.0f);
      if (ImGui::BeginCombo(group.label, current)) {
        if (group.optional && ImGui::Selectable("None", group.choice < 0)) {
          group.choice = -1;
          dirty_ = true;
        }
        for (size_t i = 0; i < group.entries.size(); ++i) {
          ImGui::PushID(int(i));
          if (ImGui::Selectable(catalog_[group.entries[i]].name.c_str(),
                                group.choice == int(i))) {
            group.choice = int(i);
            dirty_ = true;
          }
          ImGui::PopID();
        }
        ImGui::EndCombo();
      }
    }
    ImGui::EndChild();
    ImGui::SameLine();

    ImGui::BeginGroup();
    ImVec2 origin = ImGui::GetCursorScreenPos();
    const ImVec2 size = ImVec2(float(kViewWidth), float(kViewHeight));
    if (view_texture_) {
      ImGui::Image(reinterpret_cast<ImTextureID>(view_texture_.get()), size);
    } else {
      ImGui::Dummy(size);
    }
    ImGui::SetCursorScreenPos(origin);
    ImGui::InvisibleButton("##view", size);
    if (ImGui::IsItemActive()) {
      yaw_ += io.MouseDelta.x * 0.01f;
      pitch_ = std::clamp(pitch_ + io.MouseDelta.y * 0.01f, -1.5f, 1.5f);
      view_dirty_ = true;
    }
    if (ImGui::IsItemHovered() && io.MouseWheel != 0.0f) {
      zoom_ = std::clamp(zoom_ * (1.0f + io.MouseWheel * 0.1f), 0.5f, 8.0f);
      view_dirty_ = true;
    }
    ImGui::TextUnformatted("Drag to turn, wheel to zoom.");
    if (ImGui::Button("Front")) {
      yaw_ = pitch_ = 0.0f;
      zoom_ = 1.0f;
      view_dirty_ = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Face")) {
      yaw_ = pitch_ = 0.0f;
      zoom_ = 4.0f;
      view_dirty_ = true;
    }
    ImGui::EndGroup();

    ImGui::TextUnformatted(status_.c_str());
    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::SliderInt("Texture frame", &frame_, 0, 13)) {
      textures_dirty_ = true;
    }
    float row_width = 0.0f;
    for (size_t i = 0; i < texture_images_.size(); ++i) {
      if (!texture_images_[i]) {
        continue;
      }
      const auto& texture = textures_[i];
      float height = 64.0f;
      float width = height * float(texture.width) / float(texture.height);
      if (row_width > 0.0f && row_width + width < 680.0f) {
        ImGui::SameLine();
      } else {
        row_width = 0.0f;
      }
      row_width += width + 8.0f;
      ImGui::Image(reinterpret_cast<ImTextureID>(texture_images_[i].get()),
                   ImVec2(width, height));
      if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("%s\n%ux%u, %u frames, format %08X",
                          texture.label.c_str(), texture.width,
                          texture.height, texture.slices, texture.format);
      }
    }
  }

  if (ImGui::Button("Close")) {
    open = false;
  }
  ImGui::End();
  if (!open) {
    close_requested_ = true;
  }
}

}  // namespace ui
}  // namespace xam
}  // namespace kernel
}  // namespace xe
