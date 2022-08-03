#pragma once

#include <optional>

#include "coord.h"
#include "rect.h"

namespace xpp::gfx {

struct Box {
  const Coord top_left;
  const Rect size;
};

std::optional<Coord> InnerPosition(Box subbox, Coord coord);

}  // namespace xpp::gfx