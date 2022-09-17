#pragma once

#include "../../gfx/coord.h"

namespace xpp::ui {

class XComponent;

struct MouseWheelEvent {
  gfx::Coord location;
  gfx::Coord vector;
  XComponent* component;
  bool active = true;
};

class MouseWheelListener {
 public:
  virtual void WheelScrolled(MouseWheelEvent*) = 0;
};

}  // namespace xpp::ui