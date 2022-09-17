#pragma once

#include "../../gfx/coord.h"
#include "../../gfx/rect.h"

namespace xpp::ui {

class XComponent;

struct MouseMotionEvent {
  gfx::Coord location;
  gfx::Coord previous_location;
  XComponent* component;
  int mouse_button = 0;
  bool active = true;
};

class MouseMotionListener {
 public:
  virtual void MouseEntered(MouseMotionEvent*) = 0;
  virtual void MouseExited(MouseMotionEvent*) = 0;
  virtual void MouseMoved(MouseMotionEvent*) = 0;
  virtual void MouseDragged(MouseMotionEvent*) = 0;
};

}  // namespace xpp::ui