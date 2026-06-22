// Cutscene Skip Enhancements
//
// Moves CVar checks for cutscene skipping out of decomp files and into
// port-side event listeners.

#include <libultraship/bridge.h>
#include "port/UI/cvar_prefixes.h"
#include "port/Enhancements/Events/Hooks/Events.h"
#include "port/ShipInit.hpp"

#define CVAR_SKIP_BOOT_LOGOS CVAR_ENHANCEMENT("Cutscenes.SkipBootLogos")
#define CVAR_SKIP_INTRO CVAR_ENHANCEMENT("Cutscenes.StartSkipIntro")
#define CVAR_SKIP_MISC_CUTSCENES CVAR_ENHANCEMENT("Cutscenes.SkipMiscCutscenes")
#define CVAR_SKIP_JIGGY_DANCE CVAR_ENHANCEMENT("Cutscenes.SkipJiggyDance")
#define CVAR_SKIP_CLUCKER_CUTSCENE CVAR_ENHANCEMENT("Cutscenes.SkipCluckerCutscene")

void RegisterSkipBootLogos_Init() {
    COND_HOOK(OnBootLogosCheck, EVENT_PRIORITY_NORMAL, CVarGetInteger(CVAR_SKIP_BOOT_LOGOS, 0), [](IEvent* event) {
        auto* ev = reinterpret_cast<OnBootLogosCheck*>(event);
        *ev->skipLogos = true;
    });
}

void RegisterSkipIntroCutscene_Init() {
    COND_HOOK(OnIntroCutsceneCheck, EVENT_PRIORITY_NORMAL, CVarGetInteger(CVAR_SKIP_INTRO, 0), [](IEvent* event) {
        auto* ev = reinterpret_cast<OnIntroCutsceneCheck*>(event);
        *ev->skipIntro = true;
    });
}

void RegisterSkipMiscCutscenes_Init() {
    COND_HOOK(OnMiscCutscenesCheck, EVENT_PRIORITY_NORMAL, CVarGetInteger(CVAR_SKIP_MISC_CUTSCENES, 0),
              [](IEvent* event) {
                  auto* ev = reinterpret_cast<OnMiscCutscenesCheck*>(event);
                  *ev->skipMiscCutscenes = true;
              });
}

void RegisterSkipJiggyDance_Init() {
    COND_VB_SHOULD(VB_PLAY_JIGGY_DANCE, EVENT_PRIORITY_NORMAL, CVarGetInteger(CVAR_SKIP_JIGGY_DANCE, 0),
                   { *should = false; });
}

void RegisterSkipCluckerCutscene_Init() {
    COND_HOOK(OnGetLevelSpecificFlag, EVENT_PRIORITY_NORMAL, CVarGetInteger(CVAR_SKIP_CLUCKER_CUTSCENE, 0),
              [](IEvent* event) {
                  auto* ev = reinterpret_cast<OnGetLevelSpecificFlag*>(event);
                  if (ev->flagId == LEVEL_FLAG_14_TTC_UNKNOWN) {
                      ev->result = 1;
                      event->Cancelled = true;
                  }
              });
}

static RegisterShipInitFunc initBootLogosFunc(RegisterSkipBootLogos_Init, { CVAR_SKIP_BOOT_LOGOS });
static RegisterShipInitFunc initSkipIntroFunc(RegisterSkipIntroCutscene_Init, { CVAR_SKIP_INTRO });
static RegisterShipInitFunc initSkipMiscCutscenesFunc(RegisterSkipMiscCutscenes_Init, { CVAR_SKIP_MISC_CUTSCENES });
static RegisterShipInitFunc initSkipJiggyDanceFunc(RegisterSkipJiggyDance_Init, { CVAR_SKIP_JIGGY_DANCE });
static RegisterShipInitFunc initSkipCluckerCutsceneFunc(RegisterSkipCluckerCutscene_Init,
                                                        { CVAR_SKIP_CLUCKER_CUTSCENE });
