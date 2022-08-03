#include "util.h"

namespace xpp::gfx {

std::optional<Coord> InnerPosition(Box subbox, Coord coord) {
  if (coord.x < subbox.top_left.x)
    return std::nullopt;
  if (coord.y < subbox.top_left.y)
    return std::nullopt;
  if (coord.x > subbox.top_left.x + subbox.size.width)
    return std::nullopt;
  if (coord.y > subbox.top_left.y + subbox.size.height)
    return std::nullopt;
  return coord - subbox.top_left;
}

}  // namespace xpp::gfx