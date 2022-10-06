#include "canvas.h"

namespace xpp::ui {

XCanvas::XCanvas(std::shared_ptr<xlib::XPixmap> pixmap, Graphics g) 
  :pixmap_(std::move(pixmap)), g_(std::move(g)) {}

Graphics* XCanvas::GetGraphics() {
  return &g_;
}

void XCanvas::MapOnTo(Graphics* g, gfx::Coord at) {
  g->CopyArea(pixmap_, at);
}


}  // namespace xpp::ui