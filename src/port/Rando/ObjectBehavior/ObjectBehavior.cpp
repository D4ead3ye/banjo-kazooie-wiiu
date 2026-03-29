#include "ObjectBehavior.h"
#include <libultraship/bridge/consolevariablebridge.h>
// #include "port/Rando/Logic/Logic.h"
#include "port/enhancements/events/hooks/Events.h"

#include "spdlog/spdlog.h"

void LogOutSpawns(int32_t actorId, int16_t posX, int16_t posY, int16_t posZ) {
    std::string locationStr = std::to_string(posX) + ", " + std::to_string(posY) + ", " + std::to_string(posZ);
    SPDLOG_INFO("Actor ID: {} | Position: {}", actorId, locationStr);
}

// Entry point for the module, run once on game boot
void Rando::ObjectBehavior::Init() {
    REGISTER_LISTENER(OnActorSpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorSpawn* ev = (OnActorSpawn*)event;

        if (!IS_RANDO) {
            return;
        }

        LogOutSpawns(ev->actorId, ev->posX, ev->posY, ev->posZ);

        if (ev->actorId >= ACTOR_5E_JINJO_YELLOW && ev->actorId <= ACTOR_61_JINJO_PINK) {
            event->cancelled = true;
            CustomObject::SpawnObject(ACTOR_46_JIGGY, ev->posX, ev->posY, ev->posZ, ev->rot);
        }
    })
}
