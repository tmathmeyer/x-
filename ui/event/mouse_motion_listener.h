#pragma once

#include "../../gfx/coord.h"
#include "../../gfx/rect.h"

namespace xpp::ui {

class XComponent;

struct MouseMotionEvent {
  gfx::Coord location = {0, 0};
  gfx::Coord previous_location = {0, 0};
  XComponent* component;
  int mouse_button = 0;
  bool active = true;
};

class MouseMotionListener {
 public:
  virtual ~MouseMotionListener() = default;
  virtual void MouseEntered(MouseMotionEvent*) = 0;
  virtual void MouseExited(MouseMotionEvent*) = 0;
  virtual void MouseMoved(MouseMotionEvent*) = 0;
  virtual void MouseDragged(MouseMotionEvent*) = 0;
};

}  // namespace xpp::ui