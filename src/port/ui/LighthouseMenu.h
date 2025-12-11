#pragma once

#include "UIWidgets.hpp"
#include "Menu.h"
#include <graphic/Fast3D/backends/gfx_rendering_api.h>

namespace LighthouseGui {

class LighthouseMenu : public Ship::Menu {
  public:
    LighthouseMenu(const std::string& consoleVariable, const std::string& name);

    void InitElement() override;
    void DrawElement() override;
    void UpdateElement() override;
    void Draw() override;

    void AddSidebarEntry(std::string sectionName, std::string sidbarName, uint32_t columnCount);
    WidgetInfo& AddWidget(WidgetPath& pathInfo, std::string widgetName, WidgetType widgetType);
    void AddSettings();
    void AddEnhancements();
    void AddDevTools();
};
} // namespace LighthouseGui
