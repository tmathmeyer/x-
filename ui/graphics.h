#pragma once

#include "look_and_feel.h"

#include "../gfx/color.h"
#include "../gfx/coord.h"
#include "../gfx/font.h"
#include "../gfx/rect.h"
#include "../xlib/xgraphics.h"

namespace xpp::ui {

class Graphics {
 public:
  Graphics(std::shared_ptr<xlib::XGraphics>,
           std::shared_ptr<xlib::XColorMap>,
           std::shared_ptr<LookAndFeel>,
           gfx::Coord,
           gfx::Rect);

  void SetColor(gfx::Color color);
  void SetColor(std::string name);
  void SetFont(std::string font);
  void SetFontSize(uint16_t size);
  void SetFont(gfx::Font font);

  gfx::Rect GetDimensions() const;
  uint32_t GetFontHeight() const;

  void FillRect(gfx::Coord at, gfx::Rect size);
  void DrawRect(gfx::Coord at, gfx::Rect size);
  void DrawText(gfx::Coord at, std::string message);

  void DrawRoundedRect(gfx::Coord at, gfx::Rect size, uint32_t radius);
  void FillRoundedRect(gfx::Coord at, gfx::Rect size, uint32_t radius);

  Graphics SubGraphics(gfx::Coord at, gfx::Rect size);

 private:
  Graphics(std::shared_ptr<xlib::XGraphics>,
           std::shared_ptr<xlib::XColorMap>,
           std::shared_ptr<LookAndFeel>,
           gfx::Color,
           gfx::Font,
           gfx::Coord,
           gfx::Rect);

  std::shared_ptr<xlib::XGraphics> graphics_;
  std::shared_ptr<xlib::XColorMap> colormap_;
  std::shared_ptr<LookAndFeel> laf_;
  gfx::Color color_ = gfx::Color::BLACK;
  gfx::Font font_;


  gfx::Coord offset_;
  gfx::Rect size_;
};

}  // namespace xpp::ui