#pragma once

#include "UIWidgets.hpp"
#include "InputViewer.h"
#include "LighthouseModals.h"

namespace LighthouseGui {
    void SetupHooks();
    void SetupGuiElements();
    void Draw();
    void Destroy();
    UIWidgets::Colors GetMenuThemeColor();
}

#define THEME_COLOR LighthouseGui::GetMenuThemeColor()
