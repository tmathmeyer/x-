#pragma once

#include <map>
#include <memory>

#include "../gfx/color.h"
#include "../gfx/font.h"

#include "../xlib/xgraphics.h"

namespace xpp::ui {

class Graphics;

class LookAndFeel {
 public:
  const gfx::Color PanelBackground = gfx::Color::GRAY8;

 public:
  LookAndFeel();

  gfx::Font GetFont(std::shared_ptr<xlib::XGraphics> gc,
                    std::string name,
                    gfx::Font existing);
  gfx::Font GetFont(std::shared_ptr<xlib::XGraphics> gc,
                    uint16_t size,
                    gfx::Font existing);
  gfx::Font AllocateFont(std::shared_ptr<xlib::XGraphics> gc,
                         std::string key,
                         std::string name,
                         uint16_t size);

  XColor GetXColor(std::shared_ptr<xlib::XColorMap>, gfx::Color);
  XftColor GetXFTColor(std::shared_ptr<xlib::XGraphics>, gfx::Color);
  gfx::Color GetColorByName(std::string);
  void SetColor(std::string, gfx::Color);

 private:
  friend class Graphics;

  std::map<std::string, gfx::Color> colors_;
  std::map<std::string, gfx::Font> fonts_;
  std::map<gfx::Color, XColor> xcolors_;
  std::map<gfx::Color, XftColor> xft_colors_;
  XftDraw* xft_ctx_ = nullptr;
};

}  // namespace xpp::ui