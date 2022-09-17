#pragma once

#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <string>

namespace xpp::ui {
class LookAndFeel;
class Graphics;
}

namespace xpp::gfx {

class Font {
 public:
  enum class TextRenderingMode : uint8_t { kXorg, kXFT, kXMB, kNone, kFailed };

  uint32_t Height() const {
    switch (mode_) {
      case TextRenderingMode::kXFT:
        return xft_font_->ascent;
      default:
        return 0;
    }
  }

 private:
  friend class xpp::ui::LookAndFeel;
  friend class xpp::ui::Graphics;

  std::string font_name_;
  uint16_t size_;

  TextRenderingMode mode_;

  XftDraw* xft_ctx_ = nullptr;
  XftFont* xft_font_ = nullptr;

  XFontStruct* xfont_ = nullptr;
};

}  // namespace xpp::gfx