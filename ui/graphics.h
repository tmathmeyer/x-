#pragma once

#include "look_and_feel.h"

#include "../gfx/color.h"
#include "../gfx/coord.h"
#include "../gfx/font.h"
#include "../gfx/rect.h"
#include "../xlib/xgraphics.h"

namespace xpp::ui {

class XCanvas;

class Graphics {
 public:
  Graphics(std::shared_ptr<xlib::XGraphics>,
           std::shared_ptr<xlib::XColorMap>,
           std::shared_ptr<LookAndFeel>,
           std::shared_ptr<xlib::XWindow>,
           uint32_t depth,
           gfx::Rect size);
  
  Graphics(std::shared_ptr<xlib::XGraphics>,
           std::shared_ptr<xlib::XColorMap>,
           std::shared_ptr<LookAndFeel>,
           std::shared_ptr<xlib::XWindow> window,
           uint32_t depth,
           gfx::Rect size,
           gfx::Coord offset,
           std::shared_ptr<LookAndFeel::FontCache> fonts);

  void SetColor(gfx::Color color);
  void SetColor(std::string name);
  void SetFont(std::string font);
  void SetFontSize(uint16_t size);
  void SetFont(gfx::Font font);

  gfx::Rect GetDimensions() const;
  uint32_t GetFontHeight() const;

  std::unique_ptr<XCanvas> CreateCanvas(gfx::Rect size) const;

  void FillRect(gfx::Coord at, gfx::Rect size);
  void DrawRect(gfx::Coord at, gfx::Rect size);
  void DrawText(gfx::Coord at, std::string message);

  void DrawRoundedRect(gfx::Coord at, gfx::Rect size, uint32_t radius);
  void FillRoundedRect(gfx::Coord at, gfx::Rect size, uint32_t radius);

  void CopyArea(std::shared_ptr<xlib::XDrawable> d, gfx::Coord at);

  Graphics SubGraphics(gfx::Coord at, gfx::Rect size);

 private:
  std::shared_ptr<xlib::XGraphics> graphics_;
  std::shared_ptr<xlib::XColorMap> colormap_;
  std::shared_ptr<LookAndFeel> laf_;
  std::shared_ptr<xlib::XWindow> window_;
  std::shared_ptr<LookAndFeel::FontCache> fonts_;

  uint32_t depth_;
  gfx::Rect size_;
  gfx::Coord offset_;

  gfx::Color color_ = gfx::Color::BLACK;
  gfx::Font font_;
};

}  // namespace xpp::ui