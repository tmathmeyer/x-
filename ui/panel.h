#pragma once

#include "container.h"

namespace xpp::ui {

class XPanel : public XContainer {
 public:
  XPanel();
  ~XPanel() override = default;
  void Paint(xpp::ui::Graphics* g) override;
  gfx::Rect CalculatePreferredSize() const;
};

}  // namespace xpp::ui