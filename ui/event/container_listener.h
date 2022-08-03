#pragma once

#include "base/proxy_ptr.h"

namespace xpp::ui {

class XComponent;

struct ContainerEvent {
  XComponent* parent;
  base::proxy_ptr<XComponent> child;
};

class ContainerListener {
 public:
  virtual void ComponentRemoved(ContainerEvent) = 0;
  virtual void ComponentAdded(ContainerEvent) = 0;
};

}  // namespace xpp::ui