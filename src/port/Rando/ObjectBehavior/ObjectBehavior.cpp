#include "ObjectBehavior.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/Rando/Logic/Logic.h"
#include "port/enhancements/events/hooks/Events.h"
#include "port/Rando/CustomObject/CustomObject.h"

#include "spdlog/spdlog.h"

extern "C" {
int __baMarker_8028BC60(void);
void __baMarker_resolveMusicNoteCollision(Prop* arg0);
}

void LogOutSpawns(int32_t actorId, int16_t posX, int16_t posY, int16_t posZ) {
    std::string locationStr = std::to_string(posX) + ", " + std::to_string(posY) + ", " + std::to_string(posZ);
    SPDLOG_INFO("Actor ID: {} | Position: {}", actorId, locationStr);
}

void LogOutCollision(int32_t actorId, int16_t posX, int16_t posY, int16_t posZ) {
    std::string locationStr = std::to_string(posX) + ", " + std::to_string(posY) + ", " + std::to_string(posZ);
    SPDLOG_INFO("Collect ID: {} | Position: {}", actorId, locationStr);
}

bool ShouldOverrideSpawn(int16_t posX, int16_t posY, int16_t posZ) {
    RandoCheckId randoCheckId = Rando::StaticData::GetCheckByPosition({ posX, posY, posZ });
    if (randoCheckId == RC_UNKNOWN) {
        return false;
    }

    if (CustomObject::CheckSpawnQueue(posX, posY, posZ)) {
        return false;
    }

    if (Rando::Logic::IsCheckShuffled(randoCheckId)) {
        CustomObject::AddToSpawnQueue(posX, posY, posZ);
        return true;
    }

    return false;
}

// Entry point for the module, run once on game boot
void Rando::ObjectBehavior::Init() {
    REGISTER_LISTENER(OnActorSpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorSpawn* ev = (OnActorSpawn*)event;

        // if (!IS_RANDO) {
        //     return;
        // }

        if (map_get() != MAP_2_MM_MUMBOS_MOUNTAIN) {
            return;
        }

        CustomObject::InitializeSpawnQueue();

        if (ShouldOverrideSpawn(ev->posX, ev->posY, ev->posZ)) {
            CustomObject::AddToSpawnQueue(ev->posX, ev->posY, ev->posZ);
            event->cancelled = true;
            ev->result = NULL;
        }
    })

    REGISTER_LISTENER(OnPropSpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnPropSpawn* ev = (OnPropSpawn*)event;

        // if (!IS_RANDO) {
        //     return;
        // }

        if (map_get() != MAP_2_MM_MUMBOS_MOUNTAIN) {
            return;
        }

        if (ShouldOverrideSpawn(ev->posX, ev->posY, ev->posZ)) {
            CustomObject::AddToSpawnQueue(ev->posX, ev->posY, ev->posZ);
            event->cancelled = true;
        }
    })

    REGISTER_LISTENER(OnActorCollision, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorCollision* ev = (OnActorCollision*)event;

        // if (!IS_RANDO) {
        //     return;
        // }

        if (map_get() != MAP_2_MM_MUMBOS_MOUNTAIN) {
            return;
        }

        if (!ev->propId->markerFlag) {
            switch (ev->propId->spriteProp.unk0_31) {
                case RP_MUSIC_NOTE:
                    if (!__baMarker_8028BC60()) {
                        ev->propId->spriteProp.unk8_4 = 0;
                        __baMarker_resolveMusicNoteCollision(ev->propId);
                        event->cancelled = true;
                        LogOutCollision(ev->propId->spriteProp.unk0_31, ev->propId->actorProp.x,
                                        ev->propId->actorProp.y, ev->propId->actorProp.z);
                    }
                    break;
                default:
                    break;
            }
        } else {
            if (ev->propId->actorProp.marker->id == MARKER_53_EMPTY_HONEYCOMB) {
                LogOutCollision(ACTOR_47_EMPTY_HONEYCOMB, ev->propId->actorProp.x, ev->propId->actorProp.y,
                                ev->propId->actorProp.z);
            }
        }
    })
}
