#pragma once

#include "event/mouse_listener.h"
#include "event/mouse_motion_listener.h"

namespace xpp::ui {

template <typename T>
class ButtonListener : public MouseMotionListener, public MouseListener {
 public:
  ButtonListener(T* button) { button_ = button; }

  void MouseEntered(MouseMotionEvent* event) {
    event->active = false;
    button_->Enter();
    button_->Repaint();
  }

  void MouseExited(MouseMotionEvent* event) {
    event->active = false;
    button_->Exit();
    button_->Repaint();
  }

  void MousePressed(MouseEvent* event) {
    event->active = false;
    button_->Press();
    button_->Repaint();
  }

  void MouseReleased(MouseEvent* event) {
    event->active = false;
    button_->Release();
    button_->Repaint();
  }

  void MouseMoved(MouseMotionEvent*) {}
  void MouseDragged(MouseMotionEvent*) {}
  void MouseClicked(MouseEvent*) {}

 private:
  T* button_;
};

}  // namespace xpp::ui