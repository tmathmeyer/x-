#pragma once

#include <memory>

#include "event/mouse_listener.h"
#include "event/mouse_motion_listener.h"
#include "event/mouse_wheel_listener.h"
#include "graphics.h"
#include "layout/layout.h"

namespace xpp::ui {

class XContainer;

class XComponent {
 public:
  XComponent();

  virtual void Paint(Graphics* g) = 0;

  virtual const XContainer* GetParent() const;
  virtual const std::string& GetName() const;
  virtual const gfx::Rect& GetDimensions() const;
  virtual void SetParent(XContainer* parent);
  virtual void Repaint();
  virtual void SetDimensions(gfx::Rect size);
  virtual std::optional<gfx::Rect> GetPreferredSize() const;

  virtual void AddMouseMotionListener(std::shared_ptr<MouseMotionListener>);
  virtual void AddMouseListener(std::shared_ptr<MouseListener>);
  virtual void AddMouseWheelListener(std::shared_ptr<MouseWheelListener>);

  virtual void MouseEntered(MouseMotionEvent);
  virtual void MouseExited(MouseMotionEvent);
  virtual void MouseMoved(MouseMotionEvent);
  virtual void MouseDragged(MouseMotionEvent);
  virtual void MousePressed(MouseEvent);
  virtual void MouseClicked(MouseEvent);
  virtual void MouseReleased(MouseEvent);
  virtual void WheelScrolled(MouseWheelEvent);

 private:
  XContainer* parent_ = nullptr;
  std::string uuid_ = "";
  gfx::Rect size_ = {0, 0};

  std::vector<std::shared_ptr<MouseMotionListener>> motion_listeners_;
  std::vector<std::shared_ptr<MouseListener>> mouse_listeners_;
  std::vector<std::shared_ptr<MouseWheelListener>> wheel_listeners_;
};

}  // namespace xpp::ui