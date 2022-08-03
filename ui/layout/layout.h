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
  };

  virtual std::vector<Position> DoLayout(
      std::vector<std::unique_ptr<XComponent>>& entries,
      gfx::Rect size) = 0;
};

}  // namespace xpp::ui