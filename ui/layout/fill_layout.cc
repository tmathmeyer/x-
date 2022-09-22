#include "fill_layout.h"

namespace xpp::ui {

std::vector<xpp::ui::Layout::Position> FillLayout::DoLayout(
    std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>>& entries,
    gfx::Rect size) {
  if (entries.size())
    return {{std::get<0>(entries[0]).get(), {0, 0}, size}};
  return {};
}

}  // namespace xpp::ui