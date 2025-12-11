#pragma once

#include <window/gui/GuiMenuBar.h>

namespace LighthouseGui {
class LighthouseMenuBar : public Ship::GuiMenuBar {
  public:
    using Ship::GuiMenuBar::GuiMenuBar;

  protected:
    void DrawElement() override;
    void InitElement() override;
    void UpdateElement() override{};
};
} // namespace LighthouseGui
