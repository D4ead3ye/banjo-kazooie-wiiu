#include "CheckTracker.h"
#include "port/Rando/Logic/Logic.h"
#include "port/ShipUtils.h"
#include "port/ui/UIWidgets.hpp"
#include <cstring>


#define DEFAULT_LOGIC_COLOR \
    Color_RGBA8 {           \
        200, 200, 200, 255  \
    }
#define DEFAULT_COLLECTED_COLOR \
    Color_RGBA8 {               \
        100, 255, 100, 255      \
    }
#define DEFAULT_SKIPPED_COLOR   \
    Color_RGBA8 {               \
        255, 100, 255, 255      \
    }
#define DEFAULT_ITEM_COLOR  \
    Color_RGBA8 {           \
        79, 0, 221, 255     \
    }

#define CVAR_NAME_SHOW_CHECK_TRACKER "gWindows.CheckTracker"
#define CVAR_NAME_CHECK_TRACKER_OPACITY "gRando.CheckTracker.Opacity"
#define CVAR_NAME_CHECK_TRACKER_SCALE "gRando.CheckTracker.Scale"
#define CVAR_NAME_SHOW_CURRENT_LEVEL "gRando.CheckTracker.ShowCurrentLevel"
#define CVAR_NAME_LOGIC_COLOR "gRando.CheckTracker.LogicColor"
#define CVAR_NAME_COLLECTED_COLOR "gRando.CheckTracker.CollectedColor"
#define CVAR_NAME_SKIPPED_COLOR "gRando.CheckTracker.SkippedColor"
#define CVAR_NAME_ITEM_COLOR "gRando.CheckTracker.ItemColor"

#define CVAR_SHOW_CHECK_TRACKER CVarGetInteger(CVAR_NAME_SHOW_CHECK_TRACKER, 0)
#define CVAR_CHECK_TRACKER_OPACITY CVarGetFloat(CVAR_NAME_CHECK_TRACKER_OPACITY, 0.5f)
#define CVAR_CHECK_TRACKER_SCALE CVarGetFloat(CVAR_NAME_CHECK_TRACKER_SCALE, 1.0f)
#define CVAR_SHOW_CURRENT_LEVEL CVarGetInteger(CVAR_NAME_SHOW_CURRENT_LEVEL, 0)
#define CVAR_LOGIC_COLOR CVarGetColor(CVAR_NAME_LOGIC_COLOR ".Value", DEFAULT_LOGIC_COLOR)
#define CVAR_COLLECTED_COLOR CVarGetColor(CVAR_NAME_COLLECTED_COLOR ".Value", DEFAULT_COLLECTED_COLOR)
#define CVAR_SKIPPED_COLOR CVarGetColor(CVAR_NAME_SKIPPED_COLOR ".Value", DEFAULT_SKIPPED_COLOR)
#define CVAR_ITEM_COLOR CVarGetColor(CVAR_NAME_ITEM_COLOR ".Value", DEFAULT_ITEM_COLOR)

namespace LighthouseGui {
extern std::shared_ptr<Rando::CheckTracker::CheckTrackerWindow> mRandoCheckTrackerWindow;
}

std::vector<std::tuple<const char*, Color_RGBA8, const char*>> defaultCheckColorList = {
    { CVAR_NAME_LOGIC_COLOR, DEFAULT_LOGIC_COLOR, "Out of Logic"},
    { CVAR_NAME_COLLECTED_COLOR, DEFAULT_COLLECTED_COLOR, "Check Collected" },
    { CVAR_NAME_SKIPPED_COLOR, DEFAULT_SKIPPED_COLOR, "Check Skipped"},
    { CVAR_NAME_ITEM_COLOR, DEFAULT_ITEM_COLOR, "Obtained Item" },
};

bool checkTrackerPopoutState = false;
ImVec4 checkTrackerBG = ImVec4{ 0, 0, 0, 0.5f };
float checkTrackerScale = 1.0f;

bool expandState = true;

void DrawCheckTrackerList() {
    if (Rando::Logic::shuffledPool.empty()) {
        return;
    }

    // ADD LOGIC CHECK

    if (ImGui::BeginTable("CheckTrackerTable", 1)) {
        ImGui::TableSetupColumn("Check");

        ImGui::TableNextColumn();
        for (auto& entry : Rando::Logic::shuffledPool) {
            ImVec4 checkTextColor = entry.obtained ? VecFromRGBA8(CVAR_COLLECTED_COLOR)
                                                   : UIWidgets::ColorValues.at(UIWidgets::Colors::White);
            ImVec4 itemTextColor = entry.obtained ? VecFromRGBA8(CVAR_ITEM_COLOR)
                                                  : UIWidgets::ColorValues.at(UIWidgets::Colors::Indigo);
            if (entry.skipped) {
                checkTextColor = itemTextColor = VecFromRGBA8(CVAR_SKIPPED_COLOR);
            }

            ImGui::BeginGroup();
            ImGui::TextColored(checkTextColor, Ship_ConvertEnumToReadableName(Rando::StaticData::Checks[entry.randoCheckId].name).c_str());
            if (entry.obtained) {
                ImGui::SameLine();
                RandoItemId randoItemId = Rando::Logic::GetShuffledObject(entry.randoCheckId).randoItemId;
                ImGui::TextColored(itemTextColor, "(%s)", Rando::StaticData::Items[randoItemId].name);
            }
            ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, 0));
            ImGui::EndGroup();
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::IsItemHovered() ? IM_COL32(255, 255, 0, 128)
                                                                                     : IM_COL32(255, 255, 255, 0));
            if (ImGui::IsItemClicked()) {
                entry.skipped = !entry.skipped;
            }
            ImGui::TableNextColumn();
        }
        ImGui::EndTable();
    }
}

namespace Rando {

namespace CheckTracker {

void CheckTrackerWindow::Draw() {
    if (!CVAR_SHOW_CHECK_TRACKER) {
        return;
    }

    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, checkTrackerBG);
    ImGui::PushStyleColor(ImGuiCol_TitleBg, checkTrackerBG);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, checkTrackerBG);
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);

    ImGui::SetNextWindowSize(ImVec2(485.0f, 500.0f), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("CheckTracker", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoFocusOnAppearing)) {
        checkTrackerBG.w = ImGui::IsWindowDocked() ? 1.0f : CVAR_CHECK_TRACKER_OPACITY;
        ImGui::SetWindowFontScale(checkTrackerScale);

        //If Rando Logic color statements

        if (ImGui::BeginChild("CheckTrackerChild")) {
            DrawCheckTrackerList();
            ImGui::EndChild();
        }
    }

    ImGui::End();

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(1);
}

void SettingsWindow::DrawElement() {
    if (CVarGetInteger("gWindows.CheckTracker", 0)) {
        checkTrackerPopoutState = true;
        UIWidgets::WindowButton("Return Check Tracker", "gWindows.CheckTracker",
                                LighthouseGui::mRandoCheckTrackerWindow,
                                { .size = UIWidgets::Sizes::Inline, .color = UIWidgets::Colors::Red });
    } else {
        checkTrackerPopoutState = false;
        UIWidgets::WindowButton("Popout Check Tracker", "gWindows.CheckTracker",
                                LighthouseGui::mRandoCheckTrackerWindow,
                                { .size = UIWidgets::Sizes::Inline, .color = UIWidgets::Colors::Green });
    }
    if (ImGui::BeginTable("SettingsTable", 2)) {
        ImGui::TableSetupColumn("col1", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("col2", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextColumn();

        ImGui::SeparatorText("Check Tracker");
        if (!checkTrackerPopoutState) {
            if (ImGui::BeginChild("EmbeddedCheckTrackerChild")) {
                DrawCheckTrackerList();
                ImGui::EndChild();
            }
        } else {
            ImGui::TextColored(UIWidgets::ColorValues.at(WIDGET_COLOR), "Tracker popped out");
        }

        ImGui::TableNextColumn();
        ImGui::SeparatorText("Check Settings");
        UIWidgets::CVarCheckbox("Only Show Current Level", CVAR_NAME_SHOW_CURRENT_LEVEL);

        //Logic Checkbox here

        if (UIWidgets::Button(
            "Expand All",
                UIWidgets::ButtonOptions{}.Color(WIDGET_COLOR).Size(ImVec2(ImGui::GetContentRegionAvail().x / 2, 0)))) {
            expandState = true;
        }
        ImGui::SameLine();
        if (UIWidgets::Button("Collapse All", UIWidgets::ButtonOptions{}.Color(WIDGET_COLOR))) {
            expandState = false;
        }

        //Search Checkbox and options here

        if (UIWidgets::CVarSliderFloat("", CVAR_NAME_CHECK_TRACKER_OPACITY,
            { .format = "Opacity: %.1f",
            .step = 0.01f,
            .min = 0.0f,
            .max = 1.0f,
            .defaultValue = 0.5f,
            .labelPosition = UIWidgets::LabelPositions::None,
            .color = WIDGET_COLOR,
            })) {
            checkTrackerBG.w = CVAR_CHECK_TRACKER_OPACITY;
        }

        if (UIWidgets::CVarSliderFloat(" ", CVAR_NAME_CHECK_TRACKER_SCALE,
            { .format = "Scale: %.1f",
            .step = 0.10f,
            .min = 0.7f,
            .max = 2.5f,
            .defaultValue = 1.0f,
            .labelPosition = UIWidgets::LabelPositions::None,
            .color = WIDGET_COLOR,
            })) {
            checkTrackerScale = CVAR_CHECK_TRACKER_SCALE;
        }

        int16_t checkColorIndex = 0;
        for (auto& [cvar, color, label] : defaultCheckColorList) {
            std::string cvarText = cvar;
            cvarText += ".Value";
            std::string colorText = label;
            colorText += " Color";
            std::string widgetLabel = "##";
            widgetLabel += std::to_string(checkColorIndex);

            ImGui::PushID(checkColorIndex);
            UIWidgets::CVarColorPicker(widgetLabel.c_str(), cvar, color, true);
            ImGui::SameLine();
            if (UIWidgets::Button(ICON_FA_REFRESH, { .size = ImVec2(32.0f, 32.0f), .color = WIDGET_COLOR })) {
                CVarSetColor(cvarText.c_str(), color);
                Ship::Context::GetInstance()->GetWindow()->GetGui()->SaveConsoleVariablesNextFrame();
            }
            ImGui::SameLine();
            ImGui::Text(colorText.c_str());
            ImGui::PopID();
            checkColorIndex++;
        }
        ImGui::EndTable();
    }
}

void Init() {
    checkTrackerPopoutState = CVarGetInteger("gWindows.CheckTracker", 0);
    checkTrackerBG = { 0, 0, 0, CVAR_CHECK_TRACKER_OPACITY };
    checkTrackerScale = CVAR_CHECK_TRACKER_SCALE;
}

} // namespace CheckTracker
} // namespace Rando