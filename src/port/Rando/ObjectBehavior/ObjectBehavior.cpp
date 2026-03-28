#include "ObjectBehavior.h"
#include <libultraship/bridge/consolevariablebridge.h>
// #include "port/Rando/Logic/Logic.h"
// #include "port/hooks/list/PlayerEvent.h"
#include "port/enhancements/events/hooks/Events.h"

// Entry point for the module, run once on game boot
void Rando::ObjectBehavior::Init() {
    REGISTER_LISTENER(OnActorSpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorSpawn* ev = (OnActorSpawn*)event;

        if (!IS_RANDO) {
            return;
        }

        if (ev->arg == ACTOR_61_JINJO_PINK) {
            event->cancelled = true;
            CustomObject::SpawnObject(ACTOR_46_JIGGY, ev->posX, ev->posY, ev->posZ, ev->rot);
        }
    })
}
