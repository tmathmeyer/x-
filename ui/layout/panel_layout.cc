#include "panel_layout.h"

#include <optional>

#include "xpp/ui/component.h"

namespace xpp::ui {

std::vector<xpp::ui::Layout::Position> PanelLayout::DoLayout(
    std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>>& entries,
    gfx::Rect size) {
  std::vector<xpp::ui::Layout::Position> positions;
  gfx::Coord tlc = {0, 0};
  for (const auto& tagged : entries) {
    const auto& comp = std::get<0>(tagged);
    std::optional<gfx::Rect> pref = comp->GetPreferredSize();
    if (pref.has_value()) {
      xpp::ui::Layout::Position pos = {comp.get(), tlc, pref.value()};
      tlc = tlc + gfx::Coord(0, pref->height);
      positions.push_back(pos);
    } else {
      uint32_t height = comp->GetPreferredHeight().value_or(
        size.height / entries.size());
      uint32_t width = comp->GetPreferredWidth().value_or(
        size.width);
      xpp::ui::Layout::Position pos = {comp.get(), tlc, {width, height}};
      tlc = tlc + gfx::Coord{0, height};
      positions.push_back(pos);
    }
  }
  return positions;
}

}