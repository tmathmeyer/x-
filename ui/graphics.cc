#include "graphics.h"

namespace xpp::ui {

Graphics::Graphics(std::shared_ptr<xlib::XGraphics> g,
                   std::shared_ptr<xlib::XColorMap> colormap,
                   std::shared_ptr<LookAndFeel> laf,
                   gfx::Coord at,
                   gfx::Rect size)
    : graphics_(std::move(g)),
      colormap_(std::move(colormap)),
      laf_(std::move(laf)),
      tlc_(at),
      size_(size) {}

Graphics::Graphics(std::shared_ptr<xlib::XGraphics> g,
                   std::shared_ptr<xlib::XColorMap> colormap,
                   std::shared_ptr<LookAndFeel> laf,
                   gfx::Color color,
                   gfx::Font font,
                   gfx::Coord at,
                   gfx::Rect size)
    : graphics_(std::move(g)),
      colormap_(std::move(colormap)),
      laf_(std::move(laf)),
      color_(color),
      font_(font),
      tlc_(at),
      size_(size) {}

void Graphics::SetColor(gfx::Color color) {
  color_ = color;
  graphics_->XSetForeground(laf_->GetXColor(colormap_, color).pixel);
}

void Graphics::SetColor(std::string color) {
  SetColor(laf_->GetColorByName(color));
}

void Graphics::SetFont(gfx::Font font) {
  font_ = font;
}

void Graphics::SetFont(std::string fontname) {
  font_ = laf_->GetFont(graphics_, fontname, font_);
}

void Graphics::SetFontSize(uint16_t fontsize) {
  font_ = laf_->GetFont(graphics_, fontsize, font_);
}

gfx::Rect Graphics::GetDimensions() const {
  return size_;
}

void Graphics::FillRect(gfx::Coord at, gfx::Rect size) {
  // TODO: use clamping utils of some sort
  graphics_->XFillRectangle(at.x, at.y, size.width, size.height);
}

void Graphics::DrawRect(gfx::Coord at, gfx::Rect size) {
  // TODO: use clamping utils of some sort
  graphics_->XDrawRectangle(at.x, at.y, size.width, size.height);
}

void Graphics::DrawRoundedRect(gfx::Coord at, gfx::Rect size, uint32_t radius) {
  uint32_t x = at.x + tlc_.x;
  uint32_t y = at.y + tlc_.y;
  uint32_t w = size.width;
  uint32_t h = size.height;
  graphics_->XSetLineAttributes(1, 0, 0, 0);
  graphics_->XDrawLine(x + radius, y, x + w - radius, y);
  graphics_->XDrawLine(x + radius, y + h - 1, x + w - radius, y + h - 1);
  graphics_->XDrawLine(x + w, y + radius, x + w, y + h - radius);
  graphics_->XDrawLine(x, y + radius, x, y + h - radius);

  graphics_->XDrawArc(x, y, radius * 2, radius * 2, 180 * 64, -90 * 64);
  graphics_->XDrawArc(x + w - radius * 2, y, radius * 2, radius * 2, 0,
                      90 * 64);
  graphics_->XDrawArc(x + w - radius * 2, y + h - radius * 2 - 1, radius * 2,
                      radius * 2, 0, -90 * 64);
  graphics_->XDrawArc(x, y + h - radius * 2 - 1, radius * 2, radius * 2,
                      180 * 64, 90 * 64);
}

void Graphics::FillRoundedRect(gfx::Coord at, gfx::Rect size, uint32_t radius) {
  uint32_t x = at.x + tlc_.x;
  uint32_t y = at.y + tlc_.y;
  uint32_t w = size.width;
  uint32_t h = size.height;

  graphics_->XFillRectangle(x + radius, 0, w - radius * 2, h);
  graphics_->XFillRectangle(x, y + radius, w, h - radius * 2);
  graphics_->XFillArc(x, y, radius * 2, radius * 2, 180 * 64, -90 * 64);
  graphics_->XFillArc(x + w - radius * 2, y, radius * 2, radius * 2, 0,
                      90 * 64);
  graphics_->XFillArc(x + w - radius * 2, y + h - radius * 2 - 1, radius * 2,
                      radius * 2, 0, -90 * 64);
  graphics_->XFillArc(x, y + h - radius * 2 - 1, radius * 2, radius * 2,
                      180 * 64, 90 * 64);
}

void Graphics::DrawText(gfx::Coord at, std::string message) {
  switch (font_.mode_) {
    case gfx::Font::TextRenderingMode::kXorg: {
      graphics_->XDrawString(at.x, at.y + font_.Height(), message.c_str(),
                             message.length());
      return;
    }
    case gfx::Font::TextRenderingMode::kXFT: {
      auto xft_color = laf_->GetXFTColor(graphics_, color_);
      XftDrawStringUtf8(laf_->xft_ctx_, &xft_color, font_.xft_font_, at.x,
                        at.y + font_.Height(),
                        (const unsigned char*)message.c_str(),
                        message.length());
      return;
    }
    default: {
      return;
    }
  }
}

Graphics Graphics::SubGraphics(gfx::Coord at, gfx::Rect size) {
  // TODO: clamp size

  uint32_t width = size.width;
  uint32_t height = size.width;
  int64_t x = at.x;
  int64_t y = at.y;

  if (at.x < 0) {
    if (abs(at.x) < size.width) {
      width -= abs(at.x);
    } else {
      width = 0;
    }
    x = 0;
  }
  if (at.y < 0) {
    if (abs(at.y) < size.height) {
      height -= abs(at.y);
    } else {
      height = 0;
    }
    y = 0;
  }

  if (x > size_.width) {
    x = x + tlc_.x + size_.width;
    width = 0;
  } else {
    width = std::min(size.width, static_cast<uint32_t>(size.width - x));
    x = x + tlc_.x;
  }

  if (y > size_.height) {
    y = tlc_.y + size_.height;
    height = 0;
  } else {
    height = std::min(size.height, static_cast<uint32_t>(size.height - y));
    y = y + tlc_.y;
  }

  return {graphics_, colormap_, laf_, color_, font_, {x, y}, {width, height}};
}

}  // namespace xpp::ui