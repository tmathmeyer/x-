#include "panel.h"

namespace xpp::ui {

class PanelLayout : public Layout {
  virtual std::vector<xpp::ui::Layout::Position> DoLayout(
      std::vector<std::unique_ptr<XComponent>>& entries,
      gfx::Rect size) {
    std::vector<xpp::ui::Layout::Position> positions;
    gfx::Coord tlc = {10, 10};
    for (const auto& comp : entries) {
      std::optional<gfx::Rect> pref = comp->GetPreferredSize();
      if (pref.has_value()) {
        xpp::ui::Layout::Position pos = {comp.get(), tlc, pref.value()};
        tlc = tlc + gfx::Coord(0, pref->height);
        positions.push_back(pos);
      } else {
        uint32_t wh = size.height / entries.size();
        xpp::ui::Layout::Position pos = {comp.get(), tlc, {wh, wh}};
        tlc = tlc + gfx::Coord{0, wh};
        positions.push_back(pos);
      }
    }
    return positions;
  }
};

XPanel::XPanel() {
  SetLayout(std::make_unique<PanelLayout>());
}

void XPanel::Paint(xpp::ui::Graphics* g) {
  g->SetColor("PanelBackground");
  g->FillRect({0, 0}, g->GetDimensions());
  XContainer::Paint(g);
}

}  // namespace xpp::ui