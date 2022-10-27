#include "scroll_panel.h"

#include "canvas.h"
#include "layout/panel_layout.h"

namespace xpp::ui {

namespace {

gfx::Rect DeterminePaintSize(XComponent* comp, const gfx::Rect size) {
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

ScrollWheelListener::ScrollWheelListener(XScrollPanel* panel) : panel_(panel) {}

void ScrollWheelListener::WheelScrolled(MouseWheelEvent* event) {
  int32_t y_delta = -std::max(-2l, std::min(event->vector.y, 2l)) * 16;
  int32_t x_delta = -std::max(-2l, std::min(event->vector.x, 2l)) * 16;
  panel_->Scroll({x_delta, y_delta});
  event->active = false;
}

void ScrollBar::Paint(Graphics* g) {
  XComponent::Paint(g);
  if (hovered_)
    g->SetColor("ScrollbarHoveredColor");
  else
    g->SetColor("ScrollbarColor");

  g->FillRoundedRect({0, 0}, g->GetDimensions(), 5);
  g->SetColor("ScrollbarBorderColor");
  g->DrawRoundedRect({0, 0}, g->GetDimensions(), 5);
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

ScrollBarTrack::ScrollBarTrack(XScrollPanel* panel, Mode mode)
    : XContainer(), mode_(mode) {
  this->SetLayout(std::make_unique<ScrollBarTrackLayout>(panel, mode));
  this->AddComponent(std::make_unique<ScrollBar>(panel, mode));
}

void ScrollBarTrack::Paint(Graphics* g) {
  g->SetColor("ScrollbarTrackColor");
  auto box_size = g->GetDimensions();
  uint8_t width = 18;
  gfx::Rect roundedsize = {0, 0};
  uint8_t margin = 0;
  if (mode_ == Mode::kVertical) {
    margin = (box_size.width - width) / 2;
    roundedsize = {width, box_size.height - margin * 2};
  } else {
    margin = (box_size.height - width) / 2;
    roundedsize = {box_size.width - margin * 2, width};
  }

  g->FillRoundedRect({margin, margin}, roundedsize, 5);

  g->SetColor("ScrollbarTrackBorderColor");
  g->DrawRoundedRect({margin, margin}, roundedsize, 5);
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

  // Get the margins of the bar, assuming it has a fixed thickness.
  int32_t bar_thickness = 18;
  int32_t track_true_thickness =
      mode_ == ScrollBarTrack::kHorizontal ? size.height : size.width;
  int32_t scroll_bar_margins = (track_true_thickness - bar_thickness) / 2;

  // Full track length, which is useful to see what amount of the underlying
  // viewport is actuall in view.
  int32_t full_scroll_track_length =
      mode_ == ScrollBarTrack::kHorizontal ? size.width : size.height;

  // Get the dimension of the viewport that we're scolling in.
  auto viewport_true_size = panel_->ViewportExtents();
  int32_t viewport_relevant_dimension = mode_ == ScrollBarTrack::kHorizontal
                                            ? viewport_true_size.width
                                            : viewport_true_size.height;

  // How long the track is once you remove the top/bottom or left/right margins.
  int32_t scroll_track_length =
      full_scroll_track_length - scroll_bar_margins * 2;

  // scrollbar length is (frame_dim / viewport_dim) * track_len.
  // (frame_dim / viewport_dim) is always >0 and <1.
  int32_t scroll_bar_length = (scroll_track_length * full_scroll_track_length) /
                              viewport_relevant_dimension;

  // scroll length is when top is at top, to when bottom is at bottom, aka top
  // is |scroll_bar_length| away from bottom.
  int32_t scroll_range = scroll_track_length - scroll_bar_length;

  // get how far along the scrolling is
  int32_t position_value = mode_ == ScrollBarTrack::kHorizontal
                               ? panel_->ScrollPosition().x
                               : panel_->ScrollPosition().y;

  // Get the scroll position away from the start.
  int32_t scroll_bar_location =
      ((scroll_track_length * position_value) / viewport_relevant_dimension) +
      scroll_bar_margins;

  if (mode_ == ScrollBarTrack::kVertical) {
    result.push_back({std::get<0>(entries[0]).get(),
                      {scroll_bar_margins, scroll_bar_location},
                      {bar_thickness, scroll_bar_length}});
  } else {
    result.push_back({std::get<0>(entries[0]).get(),
                      {scroll_bar_location, scroll_bar_margins},
                      {scroll_bar_length, bar_thickness}});
  }

  return result;
}

ScrollPanelLayout::ScrollPanelLayout(XScrollPanel* panel)
    : Layout(), panel_(panel) {}

std::vector<Layout::Position> ScrollPanelLayout::DoLayout(
    std::vector<std::tuple<std::unique_ptr<XComponent>, int32_t>>& entries,
    gfx::Rect size) {
  std::vector<Layout::Position> result;
  CHECK(entries.size() == 3);

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
                            {barwidth, viewport_size.height},
                            1});
        }
        break;
      }
      case XScrollPanel::ComponentUsage::kScrollHoriz: {
        if (barheight) {
          result.push_back({comp.get(),
                            {0, viewport_size.height},
                            {viewport_size.width, barheight},
                            1});
        }
        break;
      }
      case XScrollPanel::ComponentUsage::kViewport: {
        viewport_size = {size.width + barwidth, size.height + barheight};
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

gfx::Coord ScrollPanelViewport::FixLocation(gfx::Coord loc) {
  return loc + panel_->ScrollPosition();
}

void ScrollPanelViewport::MouseEntered(MouseMotionEvent* event) {
  MouseMotionEvent copy = {FixLocation(event->location),
                           FixLocation(event->previous_location),
                           event->component, event->mouse_button};
  XPanel::MouseEntered(&copy);
  event->active = copy.active;
}

void ScrollPanelViewport::MouseExited(MouseMotionEvent* event) {
  MouseMotionEvent copy = {FixLocation(event->location),
                           FixLocation(event->previous_location),
                           event->component, event->mouse_button};
  XPanel::MouseExited(&copy);
  event->active = copy.active;
}

void ScrollPanelViewport::MouseMoved(MouseMotionEvent* event) {
  MouseMotionEvent copy = {FixLocation(event->location),
                           FixLocation(event->previous_location),
                           event->component, event->mouse_button};
  XPanel::MouseMoved(&copy);
  event->active = copy.active;
}

void ScrollPanelViewport::MouseDragged(MouseMotionEvent* event) {
  MouseMotionEvent copy = {FixLocation(event->location),
                           FixLocation(event->previous_location),
                           event->component, event->mouse_button};
  XPanel::MouseDragged(&copy);
  event->active = copy.active;
}

void ScrollPanelViewport::MousePressed(MouseEvent* event) {
  MouseEvent copy = {FixLocation(event->location), event->mouse_button,
                     event->component};
  XPanel::MousePressed(&copy);
  event->active = copy.active;
}

void ScrollPanelViewport::MouseClicked(MouseEvent* event) {
  MouseEvent copy = {FixLocation(event->location), event->mouse_button,
                     event->component};
  XPanel::MouseClicked(&copy);
  event->active = copy.active;
}

void ScrollPanelViewport::MouseReleased(MouseEvent* event) {
  MouseEvent copy = {FixLocation(event->location), event->mouse_button,
                     event->component};
  XPanel::MouseReleased(&copy);
  event->active = copy.active;
}


}  // namespace internal

XScrollPanel::XScrollPanel() : XPanel() {
  AddMouseWheelListener(std::make_unique<internal::ScrollWheelListener>(this));
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
  int64_t max_height = viewport_size.height;
  int64_t canvas_width = GetDimensions().width;
  int64_t canvas_height = GetDimensions().height;
  if (canvas_width < max_width)
    canvas_height -= 50;
  if (canvas_height < max_height)
    canvas_width -= 50;
  if (canvas_width != GetDimensions().width)
    if (canvas_width < max_width)
      canvas_height -= 50;

  max_width -= canvas_width;
  max_height -= canvas_height;
  position_ = {std::max(0l, std::min(max_width, position_.x + vec.x)),
               std::max(0l, std::min(max_height, position_.y + vec.y))};
  Repaint();
}

}  // namespace xpp::ui