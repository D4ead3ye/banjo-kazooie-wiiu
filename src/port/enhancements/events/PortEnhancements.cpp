#include "PortEnhancements.h"
#include "port/save/SaveManager.h"
#include "port/rando/Rando.h"

#define INIT_EVENT_IDS

#include "port/enhancements/events/hooks/Events.h"

void PortEnhancements_Init() {
    PortEnhancements_Register();
}

void PortEnhancements_Register() {
    // Register engine events
    REGISTER_EVENT(GameFrameUpdate);
    REGISTER_EVENT(VanillaBehavior);

    // Register game events
    REGISTER_EVENT(OnGameLoad);
    REGISTER_EVENT(OnGameSave);
    REGISTER_EVENT(OnSaveFileLoad);
    REGISTER_EVENT(OnSaveFileSave);

    // Register rando events
    REGISTER_EVENT(OnSaveCreate);
    REGISTER_EVENT(OnActorSpawn);
    REGISTER_EVENT(OnPropSpawn);
    REGISTER_EVENT(OnActorCollision);

    SaveManager_Init();
    Rando::Init();
}

void PortEnhancements_Exit() {
    // @port TODO
}
