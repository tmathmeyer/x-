#pragma once

#include "../xlib/xpixmap.h"
#include "graphics.h"

namespace xpp::ui {

class XCanvas {
 public:
  XCanvas(std::shared_ptr<xlib::XPixmap> pixmap, Graphics g);
  Graphics* GetGraphics();
  void MapOnTo(Graphics* g, gfx::Coord at);

 private:
  std::shared_ptr<xlib::XPixmap> pixmap_;
  Graphics g_;
};

}  // namespace xpp::ui
