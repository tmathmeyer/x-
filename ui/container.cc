#include "container.h"

#include "../gfx/util.h"
#include "layout/fill_layout.h"

#include <iostream>

namespace xpp::ui {

XContainer::XContainer()
    : XComponent(), layout_(std::make_unique<FillLayout>()) {}

void XContainer::AddComponent(std::unique_ptr<XComponent> component,
                              int32_t key) {
  component->SetParent(this);
  auto packed = std::make_tuple<std::unique_ptr<XComponent>, int32_t>(
      std::move(component), std::move(key));
  components_.push_back(std::move(packed));
}

void XContainer::AddComponent(std::unique_ptr<XComponent> component) {
  component->SetParent(this);
  auto packed = std::make_tuple<std::unique_ptr<XComponent>, int32_t>(
      std::move(component), 0);
  components_.push_back(std::move(packed));
}

void XContainer::SetLayout(std::unique_ptr<Layout> layout) {
  layout_ = std::move(layout);
}

void XContainer::RemoveComponent(XComponent* to_remove) {
  std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>> replacement;
  for (auto& comp : components_) {
    if (std::get<0>(comp)->GetName() == to_remove->GetName()) {
      ContainerEvent event = {.parent = this,
                              .child = std::move(std::get<0>(comp))};
      for (const auto& listener : container_listeners_)
        listener->ComponentRemoved(&event);
    } else {
      replacement.push_back(std::move(comp));
    }
  }
  components_ = std::move(replacement);
}

void XContainer::RemoveAll() {
  for (auto& comp : components_) {
    ContainerEvent event = {.parent = this,
                            .child = std::move(std::get<0>(comp))};
    for (const auto& listener : container_listeners_)
      listener->ComponentRemoved(&event);
  }
  components_.clear();
}

void XContainer::AddComponentListener(
    std::shared_ptr<ContainerListener> listener) {
  container_listeners_.push_back(listener);
}

void XContainer::Paint(Graphics* g) {
  XComponent::Paint(g);
  for (auto position : layout_->DoLayout(components_, g->GetDimensions())) {
    Graphics sub = g->SubGraphics(position.at, position.size);
    position.component->Paint(&sub);
  }
}

void XContainer::MouseEntered(MouseMotionEvent* event) {
  XComponent::MouseEntered(event);
  if (!event->active)
    return;
  for (auto position : layout_->DoLayout(components_, GetDimensions())) {
    auto inner = InnerPosition({position.at, position.size}, event->location);
    if (inner.has_value()) {
      MouseMotionEvent copy = {inner.value(), inner.value(),
                               position.component};
      position.component->MouseEntered(&copy);
      event->active = copy.active;
      return;
    }
  }
}

void XContainer::MouseExited(MouseMotionEvent* event) {
  XComponent::MouseExited(event);
  if (!event->active)
    return;
  for (auto position : layout_->DoLayout(components_, GetDimensions())) {
    auto inner = InnerPosition({position.at, position.size}, event->location);
    if (inner.has_value()) {
      MouseMotionEvent copy = {inner.value(), inner.value(),
                               position.component};
      position.component->MouseExited(&copy);
      event->active = copy.active;
      return;
    }
  }
}

void XContainer::MouseMoved(MouseMotionEvent* event) {
  XComponent::MouseMoved(event);
  if (!event->active)
    return;
  bool moved = false;
  for (auto position : layout_->DoLayout(components_, GetDimensions())) {
    auto at = InnerPosition({position.at, position.size}, event->location);
    auto prev =
        InnerPosition({position.at, position.size}, event->previous_location);
    if (at.has_value()) {
      MouseMotionEvent copy = {at.value(), at.value(), position.component};
      if (!prev.has_value()) {
        position.component->MouseEntered(&copy);
        copy.active = true;
      }
      if (!moved) {
        moved = true;
        copy.previous_location = *prev;
        position.component->MouseMoved(&copy);
        event->active = copy.active;
      }
    } else if (prev.has_value()) {
      MouseMotionEvent copy = {prev.value(), prev.value(), position.component};
      position.component->MouseExited(&copy);
    }
  }
}

void XContainer::MouseDragged(MouseMotionEvent* event) {
  XComponent::MouseDragged(event);
  if (!event->active)
    return;
  bool moved = false;
  for (auto position : layout_->DoLayout(components_, GetDimensions())) {
    auto at = InnerPosition({position.at, position.size}, event->location);
    auto prev =
        InnerPosition({position.at, position.size}, event->previous_location);
    if (at.has_value()) {
      MouseMotionEvent copy = {at.value(), at.value(), position.component};
      if (!prev.has_value()) {
        position.component->MouseEntered(&copy);
        copy.active = true;
      }
      if (!moved) {
        moved = true;
        copy.previous_location = *prev;
        position.component->MouseDragged(&copy);
        event->active = copy.active;
      }
    } else if (prev.has_value()) {
      MouseMotionEvent copy = {prev.value(), prev.value(), position.component};
      position.component->MouseExited(&copy);
    }
  }
}

void XContainer::MousePressed(MouseEvent* event) {
  XComponent::MousePressed(event);
  if (!event->active)
    return;
  for (auto position : layout_->DoLayout(components_, GetDimensions())) {
    auto inner = InnerPosition({position.at, position.size}, event->location);
    if (inner.has_value()) {
      MouseEvent copy = {inner.value(), event->mouse_button,
                         position.component};
      position.component->MousePressed(&copy);
      event->active = copy.active;
      return;
    }
  }
}

void XContainer::MouseClicked(MouseEvent* event) {
  XComponent::MouseClicked(event);
  if (!event->active)
    return;
  for (auto position : layout_->DoLayout(components_, GetDimensions())) {
    auto inner = InnerPosition({position.at, position.size}, event->location);
    if (inner.has_value()) {
      MouseEvent copy = {inner.value(), event->mouse_button,
                         position.component};
      position.component->MouseClicked(&copy);
      event->active = copy.active;
      return;
    }
  }
}

void XContainer::MouseReleased(MouseEvent* event) {
  XComponent::MouseReleased(event);
  if (!event->active)
    return;
  for (auto position : layout_->DoLayout(components_, GetDimensions())) {
    auto inner = InnerPosition({position.at, position.size}, event->location);
    if (inner.has_value()) {
      MouseEvent copy = {inner.value(), event->mouse_button,
                         position.component};
      position.component->MouseReleased(&copy);
      event->active = copy.active;
      return;
    }
  }
}

void XContainer::WheelScrolled(MouseWheelEvent* event) {
  XComponent::WheelScrolled(event);
  if (!event->active)
    return;
  for (auto position : layout_->DoLayout(components_, GetDimensions())) {
    auto inner = InnerPosition({position.at, position.size}, event->location);
    if (inner.has_value()) {
      MouseWheelEvent copy = {inner.value(), event->vector, position.component};
      position.component->WheelScrolled(&copy);
      event->active = copy.active;
      return;
    }
  }
}

}  // namespace xpp::ui