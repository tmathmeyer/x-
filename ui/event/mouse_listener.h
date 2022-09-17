#pragma once

#include "../../gfx/coord.h"

namespace xpp::ui {

class XComponent;

struct MouseEvent {
  gfx::Coord location;
  int mouse_button;
  XComponent* component;
  bool active = true;
};

class MouseListener {
 public:
  virtual void MouseClicked(MouseEvent*) = 0;
  virtual void MousePressed(MouseEvent*) = 0;
  virtual void MouseReleased(MouseEvent*) = 0;
};

}  // namespace xpp::ui