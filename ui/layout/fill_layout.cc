#include "fill_layout.h"

namespace xpp::ui {

std::vector<xpp::ui::Layout::Position> FillLayout::DoLayout(
    std::vector<std::unique_ptr<XComponent>>& entries,
    gfx::Rect size) {
  if (entries.size())
    return {{entries[0].get(), {0, 0}, size}};
  return {};
}

}  // namespace xpp::ui