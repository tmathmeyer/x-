#include "panel.h"

#include "layout/panel_layout.h"

namespace xpp::ui {

XPanel::XPanel() : XContainer() {
  SetLayout(std::make_unique<PanelLayout>());
}

void XPanel::Paint(xpp::ui::Graphics* g) {
  g->SetColor("PanelBackground");
  g->FillRect({0, 0}, g->GetDimensions());
  XContainer::Paint(g);
}

gfx::Rect XPanel::CalculatePreferredSize() const {
  uint32_t total_components = 0;
  uint32_t heightless_components = 0;
  uint32_t max_width = 0;
  uint32_t specified_height = 0;

  for (const auto& component : XContainer::GetComponents()) {
    total_components++;
    const auto& comp = std::get<0>(component);
    const auto pref_dims = comp->GetPreferredSize();
    if (pref_dims.has_value()) {
      max_width = std::max(max_width, pref_dims->width);
      specified_height += pref_dims->height;
    } else {
      max_width = std::max(max_width, comp->GetPreferredWidth().value_or(0));
      const auto pref_height = comp->GetPreferredHeight();
      if (pref_height.has_value()) {
        specified_height += *pref_height;
      } else {
        heightless_components++;
      }
    }
  }
  if (!total_components)
    return {0, 0};

  uint32_t actual_height = (specified_height * total_components) /
                           (total_components - heightless_components);
  return {max_width, actual_height};
}

std::string XPanel::GetTypeName() const {
  return "Panel";
}


}  // namespace xpp::ui