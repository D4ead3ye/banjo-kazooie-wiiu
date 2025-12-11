#include "LighthouseMenu.h"
#include "UIWidgets.hpp"
#include "LighthouseInputEditorWindow.h"
#include "Notification.h"
#include <variant>
#include <utils/StringHelper.h>
#include <spdlog/fmt/fmt.h>
#include "variables.h"
#include <variant>
#include <tuple>
#include "ResolutionEditor.h"
#include "port/build.h"

static const std::unordered_map<int32_t, const char*> menuThemeOptions = {
    { UIWidgets::Colors::Red, "Red" },
    { UIWidgets::Colors::DarkRed, "Dark Red" },
    { UIWidgets::Colors::Orange, "Orange" },
    { UIWidgets::Colors::Green, "Green" },
    { UIWidgets::Colors::DarkGreen, "Dark Green" },
    { UIWidgets::Colors::LightBlue, "Light Blue" },
    { UIWidgets::Colors::Blue, "Blue" },
    { UIWidgets::Colors::DarkBlue, "Dark Blue" },
    { UIWidgets::Colors::Indigo, "Indigo" },
    { UIWidgets::Colors::Violet, "Violet" },
    { UIWidgets::Colors::Purple, "Purple" },
    { UIWidgets::Colors::Brown, "Brown" },
    { UIWidgets::Colors::Gray, "Gray" },
    { UIWidgets::Colors::DarkGray, "Dark Gray" },
};

static const std::vector<const char*> textureFilteringOptions = {
    "Three-Point", // Fast::FILTER_THREE_POINT,
    "Linear",      // Fast::FILTER_LINEAR
    "None",        // Fast::FILTER_NONE
};

#ifdef _DEBUG
DebugLogOption defaultLogLevel = DEBUG_LOG_TRACE;
#else
DebugLogOption defaultLogLevel = DEBUG_LOG_INFO;
#endif

static const std::vector<const char*> logLevels = {
    "Trace",    // DEBUG_LOG_TRACE
    "Debug",    // DEBUG_LOG_DEBUG
    "Info",     // DEBUG_LOG_INFO
    "Warn",     // DEBUG_LOG_WARN
    "Error",    // DEBUG_LOG_ERROR
    "Critical", // DEBUG_LOG_CRITICAL
    "Off",      // DEBUG_LOG_OFF
};

static const std::vector<const char*> notificationPosition = {
    "Top Left", "Top Right", "Bottom Left", "Bottom Right", "Hidden",
};

namespace LighthouseGui {
extern std::shared_ptr<LighthouseMenu> mLighthouseMenu;

using namespace UIWidgets;

void LighthouseMenu::AddSidebarEntry(std::string sectionName, std::string sidebarName, uint32_t columnCount) {
    assert(!sectionName.empty());
    assert(!sidebarName.empty());
    menuEntries.at(sectionName).sidebars.emplace(sidebarName, SidebarEntry{ .columnCount = columnCount });
    menuEntries.at(sectionName).sidebarOrder.push_back(sidebarName);
}

WidgetInfo& LighthouseMenu::AddWidget(WidgetPath& pathInfo, std::string widgetName, WidgetType widgetType) {
    assert(!widgetName.empty());                        // Must be unique
    assert(menuEntries.contains(pathInfo.sectionName)); // Section/header must already exist
    assert(menuEntries.at(pathInfo.sectionName).sidebars.contains(pathInfo.sidebarName)); // Sidebar must already exist
    std::unordered_map<std::string, SidebarEntry>& sidebar = menuEntries.at(pathInfo.sectionName).sidebars;
    uint8_t column = pathInfo.column;
    if (sidebar.contains(pathInfo.sidebarName)) {
        while (sidebar.at(pathInfo.sidebarName).columnWidgets.size() < column + 1) {
            sidebar.at(pathInfo.sidebarName).columnWidgets.push_back({});
        }
    }
    SidebarEntry& entry = sidebar.at(pathInfo.sidebarName);
    entry.columnWidgets.at(column).push_back({ .name = widgetName, .type = widgetType });
    WidgetInfo& widget = entry.columnWidgets.at(column).back();
    switch (widgetType) {
        case WIDGET_CHECKBOX:
        case WIDGET_CVAR_CHECKBOX:
            widget.options = std::make_shared<CheckboxOptions>();
            break;
        case WIDGET_SLIDER_FLOAT:
        case WIDGET_CVAR_SLIDER_FLOAT:
            widget.options = std::make_shared<FloatSliderOptions>();
            break;
        case WIDGET_SLIDER_INT:
        case WIDGET_CVAR_SLIDER_INT:
            widget.options = std::make_shared<IntSliderOptions>();
            break;
        case WIDGET_COMBOBOX:
        case WIDGET_CVAR_COMBOBOX:
        case WIDGET_AUDIO_BACKEND:
        case WIDGET_VIDEO_BACKEND:
            widget.options = std::make_shared<ComboboxOptions>();
            break;
        case WIDGET_BUTTON:
            widget.options = std::make_shared<ButtonOptions>();
            break;
        case WIDGET_WINDOW_BUTTON:
            widget.options = std::make_shared<ButtonOptions>(ButtonOptions{ .size = Sizes::Inline });
            break;
        case WIDGET_COLOR_24:
        case WIDGET_COLOR_32:
            break;
        case WIDGET_SEARCH:
        case WIDGET_SEPARATOR:
        case WIDGET_SEPARATOR_TEXT:
        case WIDGET_TEXT:
        default:
            widget.options = std::make_shared<WidgetOptions>();
    }
    return widget;
}

// TODO: Generate
// Last generated with the following command on 10/30/2025. The gap in commits is so that cherry-picks from other repos
// do not get mixed in.
// clang-format off
// { git shortlog -sn 2332f63..558f59b ; git shortlog -sn dfcc80e..HEAD; } | sort -k2 | uniq -f1 | sed -E 's/^[[:space:]]*[0-9]+[[:space:]]+/"/' | sed -E 's/(.+)/\1",/'
// clang-format on
std::vector<std::string> contributors = {
    ""
};

void LighthouseMenu::AddSettings() {
    // Add Settings menu
    AddMenuEntry("Settings", "gSettings.Menu.SettingsSidebarSection");
    // General Settings
    AddSidebarEntry("Settings", "General", 2);
    WidgetPath path = { "Settings", "General", SECTION_COLUMN_1 };
    AddWidget(path, "Menu Theme", WIDGET_CVAR_COMBOBOX)
        .CVar("gSettings.Menu.Theme")
        .Options(ComboboxOptions()
                     .Tooltip("Changes the Theme of the Menu Widgets.")
                     .ComboMap(&menuThemeOptions)
                     .DefaultIndex(Colors::LightBlue));
#if not defined(__SWITCH__) and not defined(__WIIU__)
    AddWidget(path, "Menu Controller Navigation", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_IMGUI_CONTROLLER_NAV)
        .Options(CheckboxOptions().Tooltip(
            "Allows controller navigation of the Lighthouse menu (Settings, Enhancements,...)\nCAUTION: "
            "This will disable game inputs while the menu is visible.\n\nD-pad to move between "
            "items, A to select, B to move up in scope."));
    AddWidget(path, "Cursor Always Visible", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.CursorVisibility")
        .Callback([](WidgetInfo& info) {
            Ship::Context::GetInstance()->GetWindow()->SetForceCursorVisibility(
                CVarGetInteger("gSettings.CursorVisibility", 0));
        })
        .Options(CheckboxOptions().Tooltip("Makes the cursor always visible, even in full screen."));
#endif
    AddWidget(path, "Search In Sidebar", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.Menu.SidebarSearch")
        .Callback([](WidgetInfo& info) {
            if (CVarGetInteger("gSettings.Menu.SidebarSearch", 0)) {
                mLighthouseMenu->InsertSidebarSearch();
            } else {
                mLighthouseMenu->RemoveSidebarSearch();
            }
        })
        .Options(CheckboxOptions().Tooltip(
            "Displays the Search menu as a sidebar entry in Settings instead of in the header."));
    AddWidget(path, "Search Input Autofocus", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.Menu.SearchAutofocus")
        .Options(CheckboxOptions().Tooltip(
            "Search input box gets autofocus when visible. Does not affect using other widgets."));
    AddWidget(path, "Alt Assets Tab hotkey", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Mods.AlternateAssetsHotkey")
        .Options(
            CheckboxOptions().Tooltip("Allows pressing the Tab key to toggle alternate assets.").DefaultValue(true));
    AddWidget(path, "Open App Files Folder", WIDGET_BUTTON)
        .Callback([](WidgetInfo& info) {
            std::string filesPath = Ship::Context::GetInstance()->GetAppDirectoryPath();
            SDL_OpenURL(std::string("file:///" + std::filesystem::absolute(filesPath).string()).c_str());
        })
        .Options(ButtonOptions().Tooltip("Opens the folder that contains the save and mods folders, etc."));

    path.column = SECTION_COLUMN_2;
    AddWidget(path, "about", WIDGET_CUSTOM).CustomFunction([](WidgetInfo& info) {
        ImGui::BeginChild("about");
        ImGui::PushStyleColor(ImGuiCol_Text, ColorValues.at(Colors::Gray));
        if (gGitCommitTag[0] == 0) {
            ImGui::Text("%s | %s", (char*)gGitBranch, (char*)gGitCommitHash);
        } else {
            ImGui::Text("%s", (char*)gBuildVersion);
        }
        ImGui::PopStyleColor();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
        ImGui::SeparatorText("Thank You");
        ImGui::PopStyleColor();
        // ImGui::SameLine();
        // ImTextureID heartTextureId = Ship::Context::GetInstance()->GetWindow()->GetGui()->GetTextureByName(
        //     (const char*)gQuestIconHeartContainer2Tex);
        // ImGui::Image(heartTextureId, ImVec2(25.0f, 25.0f));
        ImGui::TextWrapped("Special thanks to our contributors, playtesters, artists, moderators, helpers, and "
                           "everyone in the larger decomp & N64 communities who make this project possible.\n\n");

        // Draw auto scrolling list of contributors in columns
        ImGui::SetNextWindowSize(ImVec2(0.0f, ImGui::GetMainViewport()->WorkSize.y / 3));
        ImGui::BeginChild("contributors");
        static double scrollSpeed = 1.5f * (ImGui::GetFontSize() / 1000.0f); // Lines to scroll per second
        double scrollPosition = fmod(GetUnixTimestamp() * scrollSpeed, ImGui::GetScrollMaxY() + 1.0f);
        ImGui::SetScrollY(scrollPosition);

        ImGui::Dummy(ImVec2(0.0f, ImGui::GetFontSize()));
        static int numColumns = 2; // Two columns seem to work best. Some names are too long for more on lower res
        for (int column = 0; column < numColumns; column++) {
            if (column > 0)
                ImGui::SameLine();

            ImGui::BeginGroup();
            for (int i = column; i < contributors.size(); i += numColumns) {
                ImGui::Text("%s", contributors.at(i).c_str());
            }
            ImGui::EndGroup();
        }
        ImGui::Dummy(ImVec2(0.0f, ImGui::GetFontSize()));
        ImGui::EndChild();

        ImGui::EndChild();
    });

    // Audio Settings
    path.sidebarName = "Audio";
    path.column = SECTION_COLUMN_1;
    AddSidebarEntry("Settings", "Audio", 3);
    // AddWidget(path, "Master Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
    //     .CVar("gSettings.Audio.MasterVolume")
    //     .Options(FloatSliderOptions()
    //                  .Tooltip("Adjust the overall sound volume.")
    //                  .ShowAdjustmentButtons(false)
    //                  .Format("")
    //                  .IsPercentage());
    // AddWidget(path, "Main Music Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
    //     .CVar("gSettings.Audio.MainMusicVolume")
    //     .Callback([](WidgetInfo& info) {
    //         AudioSeq_SetPortVolumeScale(SEQ_PLAYER_BGM_MAIN, CVarGetFloat("gSettings.Audio.MainMusicVolume", 1.0f));
    //     })
    //     .Options(FloatSliderOptions()
    //                  .Tooltip("Adjust the background music volume.")
    //                  .ShowAdjustmentButtons(false)
    //                  .Format("")
    //                  .IsPercentage());
    // AddWidget(path, "Sub Music Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
    //     .CVar("gSettings.Audio.SubMusicVolume")
    //     .Callback([](WidgetInfo& info) {
    //         AudioSeq_SetPortVolumeScale(SEQ_PLAYER_BGM_SUB, CVarGetFloat("gSettings.Audio.SubMusicVolume", 1.0f));
    //     })
    //     .Options(FloatSliderOptions()
    //                  .Tooltip("Adjust the sub music volume.")
    //                  .ShowAdjustmentButtons(false)
    //                  .Format("")
    //                  .IsPercentage());
    // AddWidget(path, "Sound Effects Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
    //     .CVar("gSettings.Audio.SoundEffectsVolume")
    //     .Callback([](WidgetInfo& info) {
    //         AudioSeq_SetPortVolumeScale(SEQ_PLAYER_SFX, CVarGetFloat("gSettings.Audio.SoundEffectsVolume", 1.0f));
    //     })
    //     .Options(FloatSliderOptions()
    //                  .Tooltip("Adjust the sound effects volume.")
    //                  .ShowAdjustmentButtons(false)
    //                  .Format("")
    //                  .IsPercentage());
    // AddWidget(path, "Fanfare Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
    //     .CVar("gSettings.Audio.FanfareVolume")
    //     .Callback([](WidgetInfo& info) {
    //         AudioSeq_SetPortVolumeScale(SEQ_PLAYER_FANFARE, CVarGetFloat("gSettings.Audio.FanfareVolume", 1.0f));
    //     })
    //     .Options(FloatSliderOptions()
    //                  .Tooltip("Adjust the fanfare volume.")
    //                  .ShowAdjustmentButtons(false)
    //                  .Format("")
    //                  .IsPercentage());
    // AddWidget(path, "Ambience Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
    //     .CVar("gSettings.Audio.AmbienceVolume")
    //     .Callback([](WidgetInfo& info) {
    //         AudioSeq_SetPortVolumeScale(SEQ_PLAYER_AMBIENCE, CVarGetFloat("gSettings.Audio.AmbienceVolume", 1.0f));
    //     })
    //     .Options(FloatSliderOptions()
    //                  .Tooltip("Adjust the ambient sound volume.")
    //                  .ShowAdjustmentButtons(false)
    //                  .Format("")
    //                  .IsPercentage());
    AddWidget(path, "Audio API", WIDGET_AUDIO_BACKEND);

    // Graphics Settings
    path.sidebarName = "Graphics";
    path.column = SECTION_COLUMN_1;
    AddSidebarEntry("Settings", "Graphics", 3);
    AddWidget(path, "Graphics Options", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Toggle Fullscreen", WIDGET_BUTTON)
        .Callback([](WidgetInfo& info) { Ship::Context::GetInstance()->GetWindow()->ToggleFullscreen(); })
        .Options(ButtonOptions().Tooltip("Toggles Fullscreen On/Off."));
    AddWidget(path, "Internal Resolution: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar(CVAR_INTERNAL_RESOLUTION)
        .Callback([](WidgetInfo& info) {
            Ship::Context::GetInstance()->GetWindow()->SetResolutionMultiplier(
                CVarGetFloat(CVAR_INTERNAL_RESOLUTION, 1));
        })
        .PreFunc([](WidgetInfo& info) {
            if (mLighthouseMenu->disabledMap.at(DISABLE_FOR_ADVANCED_RESOLUTION_ON).active &&
                mLighthouseMenu->disabledMap.at(DISABLE_FOR_VERTICAL_RES_TOGGLE_ON).active) {
                info.activeDisables.push_back(DISABLE_FOR_ADVANCED_RESOLUTION_ON);
                info.activeDisables.push_back(DISABLE_FOR_VERTICAL_RES_TOGGLE_ON);
            } else if (mLighthouseMenu->disabledMap.at(DISABLE_FOR_LOW_RES_MODE_ON).active) {
                info.activeDisables.push_back(DISABLE_FOR_LOW_RES_MODE_ON);
            }
        })
        .Options(
            FloatSliderOptions()
                .Tooltip("Multiplies your output resolution by the value inputted, as a more intensive but effective "
                         "form of anti-aliasing.")
                .ShowAdjustmentButtons(false)
                .IsPercentage()
                .Format("")
                .Min(0.5f)
                .Max(2.0f));
#ifndef __WIIU__
    AddWidget(path, "Anti-aliasing (MSAA): %d", WIDGET_CVAR_SLIDER_INT)
        .CVar(CVAR_MSAA_VALUE)
        .Callback([](WidgetInfo& info) {
            Ship::Context::GetInstance()->GetWindow()->SetMsaaLevel(CVarGetInteger(CVAR_MSAA_VALUE, 1));
        })
        .Options(
            IntSliderOptions()
                .Tooltip("Activates MSAA (multi-sample anti-aliasing) from 2x up to 8x, to smooth the edges of "
                         "rendered geometry.\n"
                         "Higher sample count will result in smoother edges on models, but may reduce performance.")
                .Min(1)
                .Max(8)
                .DefaultValue(1));
#endif

    AddWidget(path, "Current FPS: %d", WIDGET_CVAR_SLIDER_INT)
        .CVar("gInterpolationFPS")
        .Callback([](WidgetInfo& info) {
            int32_t defaultValue = std::static_pointer_cast<IntSliderOptions>(info.options)->defaultValue;
            if (CVarGetInteger(info.cVar, defaultValue) == defaultValue) {
                info.name = "Current FPS: Original (%d)";
            } else {
                info.name = "Current FPS: %d";
            }
        })
        .PreFunc([](WidgetInfo& info) {
            if (mLighthouseMenu->disabledMap.at(DISABLE_FOR_MATCH_REFRESH_RATE_ON).active)
                info.activeDisables.push_back(DISABLE_FOR_MATCH_REFRESH_RATE_ON);
        })
        .Options(IntSliderOptions().Min(20).Max(360).DefaultValue(20).Tooltip(
            "Uses Matrix Interpolation to create extra frames, resulting in smoother graphics. "
            "This is purely visual and does not impact game logic, execution of glitches etc.\n\n"
            "A higher target FPS than your monitor's refresh rate will waste resources, and might give a worse "
            "result."));
    AddWidget(path, "Match Refresh Rate", WIDGET_CVAR_CHECKBOX)
        .CVar("gMatchRefreshRate")
        .Options(CheckboxOptions().Tooltip("Matches interpolation value to the refresh rate of your display."));
    AddWidget(path, "Renderer API (Needs reload)", WIDGET_VIDEO_BACKEND);
    AddWidget(path, "Enable Vsync", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_VSYNC_ENABLED)
        .PreFunc([](WidgetInfo& info) { info.isHidden = mLighthouseMenu->disabledMap.at(DISABLE_FOR_NO_VSYNC).active; })
        .Options(CheckboxOptions()
                     .Tooltip("Removes tearing, but clamps your max FPS to your displays refresh rate.")
                     .DefaultValue(true));
    AddWidget(path, "Windowed Fullscreen", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_SDL_WINDOWED_FULLSCREEN)
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = mLighthouseMenu->disabledMap.at(DISABLE_FOR_NO_WINDOWED_FULLSCREEN).active;
        })
        .Options(CheckboxOptions().Tooltip("Enables Windowed Fullscreen Mode."));
    AddWidget(path, "Allow multi-windows", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_ENABLE_MULTI_VIEWPORTS)
        .PreFunc(
            [](WidgetInfo& info) { info.isHidden = mLighthouseMenu->disabledMap.at(DISABLE_FOR_NO_MULTI_VIEWPORT).active; })
        .Options(CheckboxOptions()
                     .Tooltip("Allows multiple windows to be opened at once. Requires a reload to take effect.")
                     .DefaultValue(true));
    AddWidget(path, "Texture Filter (Needs reload)", WIDGET_CVAR_COMBOBOX)
        .CVar(CVAR_TEXTURE_FILTER)
        .Options(ComboboxOptions().Tooltip("Sets the applied Texture Filtering.").ComboVec(&textureFilteringOptions));

    path.column = SECTION_COLUMN_2;
    AddWidget(path, "Advanced Graphics Options", WIDGET_SEPARATOR_TEXT);

    path.sidebarName = "Controls";
    AddSidebarEntry("Settings", "Controls", 1);
    AddWidget(path, "Popout Bindings Window", WIDGET_WINDOW_BUTTON)
        .CVar("gWindows.LighthouseInputEditor")
        .WindowName("Lighthouse Input Editor")
        .Options(ButtonOptions().Tooltip("Enables the separate Bindings Window.").Size(Sizes::Inline));

    path.sidebarName = "Overlay";
    AddSidebarEntry("Settings", "Overlay", 2);
    AddWidget(path, "Notifications", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Position", WIDGET_CVAR_COMBOBOX)
        .CVar("gNotifications.Position")
        .Options(ComboboxOptions()
                     .Tooltip("Which corner of the screen notifications appear in.")
                     .ComboVec(&notificationPosition)
                     .DefaultIndex(3));
    AddWidget(path, "Duration: %.1f seconds", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gNotifications.Duration")
        .Options(FloatSliderOptions()
                     .Tooltip("How long notifications are displayed for.")
                     .Format("%.1f")
                     .Step(0.1f)
                     .Min(3.0f)
                     .Max(30.0f)
                     .DefaultValue(10.0f));
    AddWidget(path, "Background Opacity: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gNotifications.BgOpacity")
        .Options(FloatSliderOptions()
                     .Tooltip("How opaque the background of notifications is.")
                     .DefaultValue(0.5f)
                     .IsPercentage());
    AddWidget(path, "Size %.1f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gNotifications.Size")
        .Options(FloatSliderOptions()
                     .Tooltip("How large notifications are.")
                     .Format("%.1f")
                     .Step(0.1f)
                     .Min(1.0f)
                     .Max(5.0f)
                     .DefaultValue(1.8f));
    AddWidget(path, "Test Notification", WIDGET_BUTTON)
        .Callback([](WidgetInfo& info) {
            Notification::Emit({
                .itemIcon = "__OTR__icon_item_24_static_yar/gQuestIconGoldSkulltulaTex",
                .prefix = "This",
                .message = "is a",
                .suffix = "test.",
            });
        })
        .Options(ButtonOptions().Tooltip("Displays a test notification."));
    path.column = SECTION_COLUMN_2;
    // AddWidget(path, "In-Game Timer", WIDGET_SEPARATOR_TEXT);
    // AddWidget(path, "Display", WIDGET_CVAR_COMBOBOX)
    //     .CVar("gWindows.DisplayOverlay")
    //     .WindowName("Display Overlay")
    //     .Options(
    //         ComboboxOptions()
    //             .Tooltip(
    //                 "How the timer should be displayed in the overlay.\n\n"
    //                 "- Off: Do not display a timer\n"
    //                 "- Real-Time: Display the time that has elapsed since creating the save file, regardless of play.\n"
    //                 "- In-Game Time: Display the time spent playing the save file")
    //             .ComboVec(&timerDisplayOptions));
    AddWidget(path, "Hide Window Background", WIDGET_CVAR_CHECKBOX)
        .CVar("gDisplayOverlay.Background")
        .Options(CheckboxOptions().Tooltip("Hides the background of the Display Overlay window."));
    AddWidget(path, "Scale: %.1fx", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gDisplayOverlay.Scale")
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the Scale for the Display Overlay window.")
                     .Min(1.0f)
                     .Max(5.0f)
                     .DefaultValue(1.0f)
                     .Format("%.1f")
                     .Step(0.1f));

    // path.column = SECTION_COLUMN_1;
    // path.sidebarName = "Presets";
    // AddSidebarEntry("Settings", "Presets", 1);
    // AddWidget(path, "Presets", WIDGET_CUSTOM).CustomFunction([](WidgetInfo& info) { PresetManager_Draw(); });

    // Input Viewer
    path.sidebarName = "Input Viewer";
    path.column = SECTION_COLUMN_1;
    AddSidebarEntry("Settings", path.sidebarName, 2);
    AddWidget(path, "Input Viewer", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Toggle Input Viewer", WIDGET_WINDOW_BUTTON)
        .CVar("gWindows.InputViewer")
        .WindowName("Input Viewer")
        .Options(ButtonOptions().Tooltip("Toggles the Input Viewer."));

    AddWidget(path, "Input Viewer Settings", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Popout Input Viewer Settings", WIDGET_WINDOW_BUTTON)
        .CVar("gWindows.InputViewerSettings")
        .WindowName("Input Viewer Settings")
        .Options(ButtonOptions().Tooltip("Enables the separate Input Viewer Settings Window."));
}
int32_t motionBlurStrength;

void LighthouseMenu::AddEnhancements() {
    AddMenuEntry("Enhancements", "gSettings.Menu.EnhancementsSidebarSection");
    
}

void LighthouseMenu::AddDevTools() {
    AddMenuEntry("Dev Tools", "gSettings.Menu.DevToolsSidebarSection");
    
}

LighthouseMenu::LighthouseMenu(const std::string& consoleVariable, const std::string& name)
    : Menu(consoleVariable, name, 0, UIWidgets::Colors::LightBlue) {
}

void LighthouseMenu::InitElement() {
    Ship::Menu::InitElement();
    AddSettings();
    AddEnhancements();
    AddDevTools();

    if (CVarGetInteger("gSettings.Menu.SidebarSearch", 0)) {
        InsertSidebarSearch();
    }

    for (auto& initFunc : MenuInit::GetInitFuncs()) {
        initFunc();
    }

    disabledMap = {
        { DISABLE_FOR_NO_VSYNC,
          { [](disabledInfo& info) -> bool {
               return !Ship::Context::GetInstance()->GetWindow()->CanDisableVerticalSync();
           },
            "Disabling VSync not supported" } },
        { DISABLE_FOR_NO_WINDOWED_FULLSCREEN,
          { [](disabledInfo& info) -> bool {
               return !Ship::Context::GetInstance()->GetWindow()->SupportsWindowedFullscreen();
           },
            "Windowed Fullscreen not supported" } },
        { DISABLE_FOR_NO_MULTI_VIEWPORT,
          { [](disabledInfo& info) -> bool {
               return !Ship::Context::GetInstance()->GetWindow()->GetGui()->SupportsViewports();
           },
            "Multi-viewports not supported" } },
        { DISABLE_FOR_NOT_DIRECTX,
          { [](disabledInfo& info) -> bool {
               return Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() !=
                      Ship::WindowBackend::FAST3D_DXGI_DX11;
           },
            "Available Only on DirectX" } },
        { DISABLE_FOR_DIRECTX,
          { [](disabledInfo& info) -> bool {
               return Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() ==
                      Ship::WindowBackend::FAST3D_DXGI_DX11;
           },
            "Not Available on DirectX" } },
        { DISABLE_FOR_MATCH_REFRESH_RATE_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger("gMatchRefreshRate", 0); },
            "Match Refresh Rate is Enabled" } },
        { DISABLE_FOR_ADVANCED_RESOLUTION_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".Enabled", 0); },
            "Advanced Resolution Enabled" } },
        { DISABLE_FOR_VERTICAL_RES_TOGGLE_ON,
          { [](disabledInfo& info) -> bool {
               return CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".VerticalResolutionToggle", 0);
           },
            "Vertical Resolution Toggle Enabled" } },
        { DISABLE_FOR_LOW_RES_MODE_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger(CVAR_LOW_RES_MODE, 0); }, "N64 Mode is enabled" } },
        { DISABLE_FOR_ADVANCED_RESOLUTION_OFF,
          { [](disabledInfo& info) -> bool { return !CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".Enabled", 0); },
            "Advanced Resolution is Disabled" } },
        { DISABLE_FOR_VERTICAL_RESOLUTION_OFF,
          { [](disabledInfo& info) -> bool {
               return !CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".VerticalResolutionToggle", 0);
           },
            "Vertical Resolution Toggle is Off" } },
    };
}

void LighthouseMenu::UpdateElement() {
    Ship::Menu::UpdateElement();
}

void LighthouseMenu::Draw() {
    Ship::Menu::Draw();
}

void LighthouseMenu::DrawElement() {
    Ship::Menu::DrawElement();
}
} // namespace LighthouseGui
