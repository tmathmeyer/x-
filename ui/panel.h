#pragma once

#include "container.h"

namespace xpp::ui {

class XPanel : public XContainer {
 public:
  XPanel();
  void Paint(xpp::ui::Graphics* g) override;
};

}  // namespace xpp::ui