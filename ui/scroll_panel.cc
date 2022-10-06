#include "scroll_panel.h"

#include "canvas.h"
#include "layout/panel_layout.h"

namespace xpp::ui {

namespace {

gfx::Rect DeterminePaintSize(const XComponent* comp, const gfx::Rect size) {
  const auto preferred = comp->GetPreferredSize();
  if (preferred.has_value())
    return *preferred;
  return gfx::Rect{
      comp->GetPreferredWidth().value_or(size.width),
      comp->GetPreferredHeight().value_or(size.height),
  };
}

}  // namespace

namespace internal {

ScrollBarMouseMotion::ScrollBarMouseMotion(ScrollBar* scrollbar)
    : MouseMotionListener(), scrollbar_(scrollbar) {}

void ScrollBarMouseMotion::MouseEntered(MouseMotionEvent*) {
  scrollbar_->MouseEntered();
}

void ScrollBarMouseMotion::MouseExited(MouseMotionEvent*) {
  scrollbar_->MouseExited();
}

void ScrollBarMouseMotion::MouseDragged(MouseMotionEvent* e) {
  int32_t y_delta = e->location.y - e->previous_location.y;
  int32_t x_delta = e->location.x - e->previous_location.x;
  y_delta = std::max(-5, std::min(y_delta, 5));
  x_delta = std::max(-5, std::min(x_delta, 5));
  scrollbar_->Scroll({x_delta, y_delta});
}

void ScrollBarMouseMotion::MouseMoved(MouseMotionEvent*) {}

ScrollBar::ScrollBar(XScrollPanel* panel, ScrollBarTrack::Mode mode)
    : XComponent(), panel_(panel), mode_(mode) {
  this->AddMouseMotionListener(std::make_unique<ScrollBarMouseMotion>(this));
}

void ScrollBar::Paint(Graphics* g) {
  XComponent::Paint(g);
  if (hovered_)
    g->SetColor("ScrollbarHoveredColor");
  else
    g->SetColor("ScrollbarColor");
  g->FillRoundedRect({0, 0}, g->GetDimensions(), 4);
  g->SetColor("ScrollbarBorderColor");
  g->DrawRoundedRect({0, 0}, g->GetDimensions(), 4);
}

void ScrollBar::MouseEntered() {
  hovered_ = true;
  Repaint();
}

void ScrollBar::MouseExited() {
  hovered_ = false;
  Repaint();
}

void ScrollBar::Scroll(gfx::Coord vec) {
  if (mode_ == ScrollBarTrack::kHorizontal)
    panel_->Scroll({vec.x, 0});
  else
    panel_->Scroll({0, vec.y});
}

ScrollBarTrack::ScrollBarTrack(XScrollPanel* panel, Mode mode) : XContainer() {
  this->SetLayout(std::make_unique<ScrollBarTrackLayout>(panel, mode));
  this->AddComponent(std::make_unique<ScrollBar>(panel, mode));
}

void ScrollBarTrack::Paint(Graphics* g) {
  g->SetColor("ScrollbarTrackColor");
  g->FillRect({0, 0}, g->GetDimensions());
  g->SetColor("ScrollbarTrackBorderColor");
  g->FillRect({0, 0}, {2, g->GetDimensions().height});
  XContainer::Paint(g);
}

ScrollBarTrackLayout::ScrollBarTrackLayout(XScrollPanel* panel,
                                           ScrollBarTrack::Mode mode)
    : Layout(), panel_(panel), mode_(mode) {}

std::vector<Layout::Position> ScrollBarTrackLayout::DoLayout(
    std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>>& entries,
    gfx::Rect size) {
  std::vector<Layout::Position> result;
  if (entries.size() != 1)
    return result;

  int32_t bar_padding_end = 5;
  int32_t bar_padding_side = 5;

  auto viewport_true_size = panel_->ViewportExtents();
  auto viewport_real_size = panel_->GetDimensions();

  auto shared_den =
      (mode_ == ScrollBarTrack::kHorizontal ? viewport_true_size.width
                                            : viewport_true_size.height);
  auto size_num =
      (mode_ == ScrollBarTrack::kHorizontal ? viewport_real_size.width
                                            : viewport_real_size.height) -
      (bar_padding_end * 2);
  auto position_num =
      (mode_ == ScrollBarTrack::kHorizontal ? panel_->ScrollPosition().x
                                            : panel_->ScrollPosition().y) -
      bar_padding_end;

  auto bar_length = (size_num * size_num) / shared_den;
  auto position = (position_num * size_num) / shared_den;
  auto bar_width = size.width - (bar_padding_side * 2);

  if (mode_ == ScrollBarTrack::kVertical) {
    result.push_back({std::get<0>(entries[0]).get(),
                      {bar_padding_side, position + bar_padding_end},
                      {bar_width, bar_length}});
  } else {
    result.push_back({std::get<0>(entries[0]).get(),
                      {position + bar_padding_end, bar_padding_side},
                      {bar_length, bar_width}});
  }

  return result;
}

ScrollPanelLayout::ScrollPanelLayout(XScrollPanel* panel)
    : Layout(), panel_(panel) {}

std::vector<Layout::Position> ScrollPanelLayout::DoLayout(
    std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>>& entries,
    gfx::Rect size) {
  std::vector<Layout::Position> result;

  gfx::Rect viewport_extent = panel_->ViewportExtents();
  gfx::Rect viewport_size = size;
  uint32_t barheight = 0;
  uint32_t barwidth = 0;

  for (int i = 0; i < 2; i++) {
    if (viewport_extent.width > viewport_size.width)
      barheight = 50;
    if (viewport_extent.height > viewport_size.height)
      barwidth = 50;
    viewport_size = {size.width - barwidth, size.height - barheight};
  }

  for (const auto& tagged : entries) {
    const auto& comp = std::get<0>(tagged);
    switch (static_cast<XScrollPanel::ComponentUsage>(std::get<1>(tagged))) {
      case XScrollPanel::ComponentUsage::kScrollVert: {
        if (barwidth) {
          result.push_back({comp.get(),
                            {viewport_size.width, 0},
                            {barwidth, viewport_size.height}});
        }
        break;
      }
      case XScrollPanel::ComponentUsage::kScrollHoriz: {
        if (barheight) {
          result.push_back({comp.get(),
                            {0, viewport_size.height},
                            {viewport_size.width, barheight}});
        }
        break;
      }
      case XScrollPanel::ComponentUsage::kViewport: {
        result.push_back({comp.get(), {0, 0}, viewport_size});
        break;
      }
      default:
        break;
    }
  }

  return result;
}

ScrollPanelViewport::ScrollPanelViewport(XScrollPanel* panel)
    : XPanel(), panel_(panel) {}

gfx::Rect ScrollPanelViewport::GetCanvasSize(gfx::Rect size) const {
  uint32_t height = 0;
  uint32_t width = 0;
  for (const auto& comp_key : components_) {
    const auto& component = std::get<0>(comp_key);
    const gfx::Rect actual = DeterminePaintSize(component.get(), size);

    height += actual.height;
    width = std::max(width, actual.width);
  };
  return {width, height};
}

void ScrollPanelViewport::Paint(Graphics* g) {
  // Skip the container paint routine - we want to redo the layout ourselves.
  XComponent::Paint(g);
  g->SetColor("PanelBackground");
  g->FillRect({0, 0}, g->GetDimensions());

  // Dimensions we are given to draw in
  auto viewport = g->GetDimensions();

  // Dimensions of the underlying content
  auto content = GetCanvasSize(viewport);

  // Grab a canvas that can fit the underlying content
  auto canvas = g->CreateCanvas(content);

  // draw the panel layout stuff into the canvas
  XPanel::Paint(canvas->GetGraphics());

  // Let the canvas write itself into our graphics object
  canvas->MapOnTo(g, panel_->ScrollPosition());
}

}  // namespace internal

XScrollPanel::XScrollPanel() : XPanel() {
  layout_ = std::make_unique<internal::ScrollPanelLayout>(this);

  auto v_track = std::make_unique<internal::ScrollBarTrack>(
      this, internal::ScrollBarTrack::kVertical);
  auto h_track = std::make_unique<internal::ScrollBarTrack>(
      this, internal::ScrollBarTrack::kHorizontal);
  auto container = std::make_unique<internal::ScrollPanelViewport>(this);

  v_track->SetParent(this);
  h_track->SetParent(this);
  container->SetParent(this);

  container_ = container.get();

  components_.push_back(std::make_tuple(
      std::move(v_track), static_cast<int>(ComponentUsage::kScrollVert)));
  components_.push_back(std::make_tuple(
      std::move(h_track), static_cast<int>(ComponentUsage::kScrollHoriz)));
  components_.push_back(std::make_tuple(
      std::move(container), static_cast<int>(ComponentUsage::kViewport)));
}

gfx::Rect XScrollPanel::ViewportExtents() {
  return container_->GetCanvasSize(GetDimensions());
}

gfx::Coord XScrollPanel::ScrollPosition() {
  return position_;
}

void XScrollPanel::AddComponent(std::unique_ptr<XComponent> component,
                                int32_t key) {
  container_->AddComponent(std::move(component), key);
}

void XScrollPanel::AddComponent(std::unique_ptr<XComponent> component) {
  container_->AddComponent(std::move(component));
}

void XScrollPanel::RemoveComponent(XComponent* component) {
  container_->RemoveComponent(component);
}

void XScrollPanel::RemoveAll() {
  container_->RemoveAll();
}

void XScrollPanel::SetLayout(std::unique_ptr<Layout> layout) {
  container_->SetLayout(std::move(layout));
}

void XScrollPanel::AddComponentListener(
    std::shared_ptr<ContainerListener> listener) {
  container_->AddComponentListener(std::move(listener));
}

void XScrollPanel::Scroll(gfx::Coord vec) {
  auto viewport_size = ViewportExtents();
  int64_t max_width = viewport_size.width;
  max_width -= GetDimensions().width;
  int64_t max_height = viewport_size.height;
  max_height -= GetDimensions().height;
  position_ = {std::max(0l, std::min(max_width, position_.x + vec.x)),
               std::max(0l, std::min(max_height, position_.y + vec.y))};
  Repaint();
}

}  // namespace xpp::ui