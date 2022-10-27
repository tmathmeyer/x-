#pragma once

#include <map>
#include <memory>

#include "../gfx/color.h"
#include "../xlib/xgraphics.h"
#include "font.h"

namespace xpp::ui {

class Graphics;

class LookAndFeel {
 public:
  const gfx::Color PanelBackground = gfx::Color::GRAY8;

  class FontCache {
   public:
    FontCache(std::shared_ptr<xlib::XGraphics> gc);
    ~FontCache();

    std::map<std::string, gfx::Font> fonts;
    XftDraw* xft_ctx = nullptr;

   private:
    std::shared_ptr<xlib::XGraphics> gc_;
  };

 public:
  LookAndFeel();

  gfx::Font GetFont(std::shared_ptr<xlib::XGraphics> gc,
                    std::string name,
                    gfx::Font existing,
                    FontCache* fonts);
  gfx::Font GetFont(std::shared_ptr<xlib::XGraphics> gc,
                    uint16_t size,
                    gfx::Font existing,
                    FontCache* fonts);
  gfx::Font AllocateFont(std::shared_ptr<xlib::XGraphics> gc,
                         std::string name,
                         uint16_t size,
                         FontCache* fonts);

  XColor GetXColor(std::shared_ptr<xlib::XColorMap>, gfx::Color);
  XftColor GetXFTColor(std::shared_ptr<xlib::XGraphics>, gfx::Color);
  gfx::Color GetColorByName(std::string);
  void SetColor(std::string, gfx::Color);

 private:
  friend class Graphics;

  std::map<std::string, gfx::Color> colors_;
  std::map<gfx::Color, XColor> xcolors_;
  std::map<gfx::Color, XftColor> xft_colors_;
};

}  // namespace xpp::ui