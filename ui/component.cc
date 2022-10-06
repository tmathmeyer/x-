#include "component.h"

#include <random>

#include "container.h"

namespace xpp::ui {

namespace {

std::string get_uuid() {
  static std::random_device dev;
  static std::mt19937 rng(dev());

  std::uniform_int_distribution<int> dist(0, 15);

  const char* v = "0123456789abcdef";
  const bool dash[] = {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0};

  std::string res;
  for (int i = 0; i < 16; i++) {
    if (dash[i])
      res += "-";
    res += v[dist(rng)];
    res += v[dist(rng)];
  }
  return res;
}

}  // namespace

XComponent::XComponent() : uuid_(get_uuid()) {}

void XComponent::SetParent(XContainer* parent) {
  parent_ = parent;
}

void XComponent::Paint(Graphics* g) {
  SetDimensions(g->GetDimensions());
}

const XContainer* XComponent::GetParent() const {
  return parent_;
}

const std::string& XComponent::GetName() const {
  return uuid_;
}

const gfx::Rect& XComponent::GetDimensions() const {
  return size_;
}

std::optional<gfx::Rect> XComponent::GetPreferredSize() const {
  auto height = GetPreferredHeight();
  if (!height.has_value())
    return std::nullopt;
  auto width = GetPreferredWidth();
  if (!width.has_value())
    return std::nullopt;
  return gfx::Rect{*width, *height};
}

std::optional<uint32_t> XComponent::GetPreferredHeight() const {
  return std::nullopt;
}

std::optional<uint32_t> XComponent::GetPreferredWidth() const {
  return std::nullopt;
}

void XComponent::SetDimensions(gfx::Rect size) {
  size_ = size;
}

void XComponent::Repaint() {
  if (parent_)
    parent_->Repaint();
}

void XComponent::AddMouseMotionListener(
    std::shared_ptr<MouseMotionListener> listener) {
  motion_listeners_.push_back(listener);
}

void XComponent::AddMouseListener(std::shared_ptr<MouseListener> listener) {
  mouse_listeners_.push_back(listener);
}

void XComponent::AddMouseWheelListener(
    std::shared_ptr<MouseWheelListener> listener) {
  wheel_listeners_.push_back(listener);
}

void XComponent::MouseEntered(MouseMotionEvent* event) {
  for (const auto& listener : motion_listeners_) {
    if (!event->active)
      return;
    listener->MouseEntered(event);
  }
}

void XComponent::MouseExited(MouseMotionEvent* event) {
  for (const auto& listener : motion_listeners_) {
    if (!event->active)
      return;
    listener->MouseExited(event);
  }
}

void XComponent::MouseMoved(MouseMotionEvent* event) {
  for (const auto& listener : motion_listeners_) {
    if (!event->active)
      return;
    listener->MouseMoved(event);
  }
}

void XComponent::MouseDragged(MouseMotionEvent* event) {
  for (const auto& listener : motion_listeners_) {
    if (!event->active)
      return;
    listener->MouseDragged(event);
  }
}

void XComponent::MousePressed(MouseEvent* event) {
  for (const auto& listener : mouse_listeners_) {
    if (!event->active)
      return;
    listener->MousePressed(event);
  }
}

void XComponent::MouseClicked(MouseEvent* event) {
  for (const auto& listener : mouse_listeners_) {
    if (!event->active)
      return;
    listener->MouseClicked(event);
  }
}

void XComponent::MouseReleased(MouseEvent* event) {
  for (const auto& listener : mouse_listeners_) {
    if (!event->active)
      return;
    listener->MouseReleased(event);
  }
}

void XComponent::WheelScrolled(MouseWheelEvent* event) {
  for (const auto& listener : wheel_listeners_) {
    if (!event->active)
      return;
    listener->WheelScrolled(event);
  }
}

}  // namespace xpp::ui
