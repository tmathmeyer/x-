#include "accordion_layout.h"

#include <optional>

#include "xpp/ui/component.h"

namespace xpp::ui {

std::vector<xpp::ui::Layout::Position> AccordionLayout::DoLayout(
    std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>>& entries,
    gfx::Rect size) {
  std::vector<xpp::ui::Layout::Position> positions;
  gfx::Coord tlc = {0, 0};

  for (const auto& tagged : entries) {
    const auto& comp = std::get<0>(tagged);
    uint32_t wh = size.height / entries.size();
    xpp::ui::Layout::Position pos = {comp.get(), tlc, {size.width, wh}};
    tlc = tlc + gfx::Coord{0, wh};
    positions.push_back(pos);
  }
  return positions;
}

}  // namespace xpp::ui