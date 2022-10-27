#pragma once

#include "panel.h"

namespace xpp::ui {

class XScrollPanel;

namespace internal {

class ScrollPanelViewport : public XPanel {
 public:
  ScrollPanelViewport(XScrollPanel* panel);
  void Paint(Graphics* g) override;
  gfx::Rect GetCanvasSize(gfx::Rect size) const;
  std::string GetTypeName() const override;

  virtual void MouseEntered(MouseMotionEvent*) override;
  virtual void MouseExited(MouseMotionEvent*) override;
  virtual void MouseMoved(MouseMotionEvent*) override;
  virtual void MouseDragged(MouseMotionEvent*) override;
  virtual void MousePressed(MouseEvent*) override;
  virtual void MouseClicked(MouseEvent*) override;
  virtual void MouseReleased(MouseEvent*) override;

 private:
  XScrollPanel* panel_;
  
  gfx::Coord FixLocation(gfx::Coord loc);
};

class ScrollBarTrack : public XContainer {
 public:
  enum Mode { kVertical, kHorizontal };
  ScrollBarTrack(XScrollPanel* panel, Mode mode);
  void Paint(Graphics* g) override;

 private:
  Mode mode_;
};

class ScrollBar : public XComponent {
 public:
  ScrollBar(XScrollPanel* panel, ScrollBarTrack::Mode mode);
  void Paint(Graphics* g) override;
  void MouseEntered();
  void MouseExited();
  void Scroll(gfx::Coord vec);

 private:
  XScrollPanel* panel_;
  bool hovered_ = false;
  ScrollBarTrack::Mode mode_;
};

class ScrollPanelLayout : public Layout {
 public:
  ScrollPanelLayout(XScrollPanel* panel);
  virtual std::vector<Position> DoLayout(
      std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>>& entries,
      gfx::Rect size) override;

 private:
  XScrollPanel* panel_;
};

class ScrollBarTrackLayout : public Layout {
 public:
  ScrollBarTrackLayout(XScrollPanel* panel, ScrollBarTrack::Mode mode);
  virtual std::vector<Position> DoLayout(
      std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>>& entries,
      gfx::Rect size) override;

 private:
  XScrollPanel* panel_;
  ScrollBarTrack::Mode mode_;
};

class ScrollBarMouseMotion : public MouseMotionListener {
 public:
  ScrollBarMouseMotion(ScrollBar* scrollbar);
  void MouseEntered(MouseMotionEvent*) override;
  void MouseExited(MouseMotionEvent*) override;
  void MouseDragged(MouseMotionEvent*) override;
  void MouseMoved(MouseMotionEvent*) override;

 private:
  ScrollBar* scrollbar_;
};

class ScrollWheelListener : public MouseWheelListener {
 public:
  ScrollWheelListener(XScrollPanel* panel);
  virtual void WheelScrolled(MouseWheelEvent*) override;

 private:
  XScrollPanel* panel_;
};

}  // namespace internal

class XScrollPanel : public XPanel {
 public:
  enum class ComponentUsage : uint32_t {
    kScrollVert = 1,
    kScrollHoriz = 2,
    kViewport = 3,
  };

  XScrollPanel();
  ~XScrollPanel() override = default;

  virtual void AddComponent(std::unique_ptr<XComponent> component,
                            int32_t key) override;
  virtual void AddComponent(std::unique_ptr<XComponent> component) override;
  virtual void RemoveComponent(XComponent*) override;
  virtual void RemoveAll() override;
  virtual void SetLayout(std::unique_ptr<Layout>) override;
  virtual void AddComponentListener(
      std::shared_ptr<ContainerListener>) override;

 private:
  friend class internal::ScrollPanelLayout;
  friend class internal::ScrollBarTrackLayout;
  friend class internal::ScrollPanelViewport;
  friend class internal::ScrollBar;
  friend class internal::ScrollWheelListener;

  gfx::Rect ViewportExtents();
  gfx::Coord ScrollPosition();
  void Scroll(gfx::Coord vec);

  internal::ScrollPanelViewport* container_;
  gfx::Coord position_ = {0, 0};
};

}  // namespace xpp::ui