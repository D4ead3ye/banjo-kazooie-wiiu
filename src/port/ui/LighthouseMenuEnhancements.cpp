#include "LighthouseMenu.h"

#define CVAR_INT_SHIP_INIT(cvar, val) \
    CVarSetInteger(cvar, val);        \
    ShipInit::Init(cvar);

static std::string comboboxTooltip = "";
bool isBetaQuestEnabled = false;

extern "C" {
void enableBetaQuest() {
    isBetaQuestEnabled = true;
}
void disableBetaQuest() {
    isBetaQuestEnabled = false;
}
}

namespace LighthouseGui {

extern std::shared_ptr<LighthouseMenu> mLighthouseMenu;
using namespace UIWidgets;

void LighthouseMenu::AddMenuEnhancements() {
    // Add Enhancements Menu
    AddMenuEntry("Enhancements", CVAR_SETTING("Menu.EnhancementsSidebarSection"));

    // Quality of Life
    WidgetPath path = { "Enhancements", "Placeholder", SECTION_COLUMN_1 };
    AddSidebarEntry("Enhancements", path.sidebarName, 3);
    path.column = SECTION_COLUMN_1;

    AddWidget(path, "Placeholder Section", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Lighthouse", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_ENHANCEMENT("Autosave"))
        .RaceDisable(false)
        .Options(CheckboxOptions().Tooltip("Empty... Bottles..."));
}

} // namespace LighthouseGui
