#include "panel.h"

#include "layout/panel_layout.h"

namespace xpp::ui {

XPanel::XPanel() {
  SetLayout(std::make_unique<PanelLayout>());
}

void XPanel::Paint(xpp::ui::Graphics* g) {
  g->SetColor("PanelBackground");
  g->FillRect({0, 0}, g->GetDimensions());
  XContainer::Paint(g);
}

}  // namespace xpp::ui