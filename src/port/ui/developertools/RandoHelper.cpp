#include "RandoHelper.h"
#include "port/ui/UIWidgets.hpp"
#include "port/ui/Notification.h"
#include "port/ShipUtils.h"

#include <string>
#include <imgui.h>
#include <libultraship/libultraship.h>
#include "port/ui/LighthouseGui.hpp"
#include "port/ui/cvar_prefixes.h"

#include "enums.h"

extern "C" {
void jiggy_spawn(enum jiggy_e jiggy_id, f32 pos[3]);
void player_getPosition(f32 dst[3]);
}

void RandoHelper_ObjectSpawner() {
    if (UIWidgets::Button("Spawn Jiggy")) {
        f32 playerPos[3];
        player_getPosition(playerPos);

        playerPos[1] += 50.0f;

        jiggy_spawn(JIGGY_33_LAIR_1ST_JIGGY, playerPos);
    }
}

void RandoHelper_DrawTabBar() {
    UIWidgets::PushStyleTabs(THEME_COLOR);
    if (ImGui::BeginTabBar("RandoHelperTabBar")) {
        if (ImGui::BeginTabItem("General")) {
            RandoHelper_ObjectSpawner();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    UIWidgets::PopStyleTabs();
}

void RandoHelperWindow::DrawElement() {
    RandoHelper_DrawTabBar();
}