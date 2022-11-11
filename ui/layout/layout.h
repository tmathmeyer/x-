#pragma once

#include <memory>
#include <vector>

#include "../../gfx/coord.h"
#include "../../gfx/rect.h"

namespace xpp::ui {

class XComponent;

class Layout {
 public:
  struct Position {
    XComponent* component;
    gfx::Coord at;
    gfx::Rect size;
    int32_t z_index = 0;

    Position(XComponent* comp, gfx::Coord at, gfx::Rect size, int32_t z_index)
        : component(comp), at(at), size(size), z_index(z_index) {}

    Position(XComponent* comp, gfx::Coord at, gfx::Rect size)
        : Position(comp, at, size, 0) {}

    Position(XComponent* comp, gfx::Rect size) : Position(comp, {0, 0}, size) {}

    Position() : Position(nullptr, {0, 0}) {}
  };

  virtual ~Layout() = default;

  virtual std::vector<Position> DoLayout(
      std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>>& entries,
      gfx::Rect size) = 0;
};

}  // namespace xpp::ui