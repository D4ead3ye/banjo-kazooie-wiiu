#include "port/ui/LighthouseMenu.h"
#include "port/Engine.h"
#include "port/ui/Notification.h"
#include "port/ui/LighthouseModals.h"
#include "port/ResourceHelpers.h"
#include "port/ui/UIWidgets.hpp"

#include "port/Rando/StaticData/StaticData.h"

extern "C" {
#include "variables.h"
}

namespace LighthouseGui {

extern std::shared_ptr<LighthouseMenu> mLighthouseMenu;
extern std::shared_ptr<LighthouseModalWindow> mModalWindow;
using namespace UIWidgets;

void LighthouseMenu::AddMenuRando() {

// Add Rando Menu
AddMenuEntry("Rando", CVAR_SETTING("Menu.RandoSidebarSection"));

// Rando - General
AddSidebarEntry("Rando", "General", 1);
WidgetPath path = { "Rando", "General", SECTION_COLUMN_1 };

AddWidget(path, "Enable Rando", WIDGET_SEPARATOR_TEXT);

AddWidget(path, "Enable Rando", WIDGET_CVAR_CHECKBOX)
    .CVar(CVAR_RANDOMIZER_SETTING("Enable"))
    .Options(CheckboxOptions().Tooltip("Enables Randomizer on the next new save file."));

// Rando - Shuffle Options
AddSidebarEntry("Rando", "Shuffle Options", 1);
path = { "Rando", "Shuffle Options", SECTION_COLUMN_1 };

AddWidget(path, "Shuffle Collectables", WIDGET_SEPARATOR_TEXT);

AddWidget(path, "Shuffle Empty Honeycombs", WIDGET_CVAR_CHECKBOX)
    .CVar(Rando::StaticData::Options[RO_SHUFFLE_EMPTY_HONEYCOMBS].cvar)
    .Options(CheckboxOptions().Tooltip("Shuffles Empty Honeycombs into the Pool."));
AddWidget(path, "Shuffle Jiggies", WIDGET_CVAR_CHECKBOX)
    .CVar(Rando::StaticData::Options[RO_SHUFFLE_JIGGIES].cvar)
    .Options(CheckboxOptions().Tooltip("Shuffles Jiggies into the Pool."));
AddWidget(path, "Shuffle Jinjos", WIDGET_CVAR_CHECKBOX)
    .CVar(Rando::StaticData::Options[RO_SHUFFLE_JINJOS].cvar)
    .Options(CheckboxOptions().Tooltip("Shuffles Jinjos into the Pool."));
AddWidget(path, "Shuffle Mumbo Tokens", WIDGET_CVAR_CHECKBOX)
    .CVar(Rando::StaticData::Options[RO_SHUFFLE_MUMBO_TOKENS].cvar)
    .Options(CheckboxOptions().Tooltip("Shuffles Mumbo Tokens into the Pool."));

}

} // namespace LighthouseGui