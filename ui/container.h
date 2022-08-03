#pragma once

#include <vector>

#include "component.h"
#include "event/container_listener.h"

namespace xpp::ui {

class XContainer : public XComponent {
 public:
  XContainer();

  virtual void AddComponent(std::unique_ptr<XComponent> component);
  virtual void RemoveComponent(XComponent*);
  virtual void RemoveAll();
  virtual void SetLayout(std::unique_ptr<Layout>);
  virtual void AddComponentListener(std::shared_ptr<ContainerListener>);

  // XComponent overrides
  virtual void Paint(Graphics* g) override;

  // XComponent listener overrides
  virtual void MouseEntered(MouseMotionEvent) override;
  virtual void MouseExited(MouseMotionEvent) override;
  virtual void MouseMoved(MouseMotionEvent) override;
  virtual void MouseDragged(MouseMotionEvent) override;
  virtual void MousePressed(MouseEvent) override;
  virtual void MouseClicked(MouseEvent) override;
  virtual void MouseReleased(MouseEvent) override;
  virtual void WheelScrolled(MouseWheelEvent) override;

 protected:
  std::vector<std::unique_ptr<XComponent>> components_;
  std::unique_ptr<Layout> layout_;

  std::vector<std::shared_ptr<ContainerListener>> container_listeners_;
};

}  // namespace xpp::ui