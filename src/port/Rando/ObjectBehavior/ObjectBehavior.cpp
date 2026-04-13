#include "ObjectBehavior.h"
#include "port/ui/UIWidgets.hpp"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/ui/Notification.h"
#include "port/Rando/Logic/Logic.h"
#include "port/enhancements/events/hooks/Events.h"
#include "port/Rando/CustomObject/CustomObject.h"

#include "spdlog/spdlog.h"

#define WIDGET_TEXT_COLOR(id) UIWidgets::ColorValues.at(id)

extern "C" {
void chjiggy_setJiggyId(Actor* thisx, u32 id);

typedef struct {
    enum mumbotoken_e uid;
} ActorLocal_MumboToken;

typedef struct {
    enum honeycomb_e uid;
    s32 unk4;
} ActorLocal_EmptyHoneycomb;

int __baMarker_8028BC60(void);
void __baMarker_resolveMusicNoteCollision(Prop* arg0);
enum level_e map_getLevel(enum map_e map);
}

// clang-format off
std::vector<int32_t> actorSpawnWhitelist = {
    ACTOR_2D_MUMBO_TOKEN,
    ACTOR_46_JIGGY,
    ACTOR_47_EMPTY_HONEYCOMB,
    ACTOR_51_MUSIC_NOTE,
    ACTOR_5E_JINJO_YELLOW,
    ACTOR_5F_JINJO_ORANGE,
    ACTOR_60_JINJO_BLUE,
    ACTOR_61_JINJO_PINK,
    ACTOR_62_JINJO_GREEN,
};

std::map<int32_t, UIWidgets::Colors> randoItemColors = {
    { RI_EMPTY_HONEYCOMB,   UIWidgets::Colors::Yellow },
    { RI_JIGGY,             UIWidgets::Colors::Yellow },
    { RI_JINJO_BLUE,        UIWidgets::Colors::SkyBlue },
    { RI_JINJO_GREEN,       UIWidgets::Colors::Green },
    { RI_JINJO_ORANGE,      UIWidgets::Colors::Orange },
    { RI_JINJO_PINK,        UIWidgets::Colors::Pink },
    { RI_JINJO_YELLOW,      UIWidgets::Colors::Yellow },
    { RI_MUMBO_TOKEN,       UIWidgets::Colors::Gray },
    { RI_MUSIC_NOTE,        UIWidgets::Colors::Yellow },
};

std::map<int32_t, actor_e> jinjoMarkerMap = {
    { MARKER_5A_JINJO_BLUE, 	ACTOR_60_JINJO_BLUE },
    { MARKER_5B_JINJO_GREEN, 	ACTOR_62_JINJO_GREEN },
    { MARKER_5C_JINJO_ORANGE, 	ACTOR_5F_JINJO_ORANGE },
    { MARKER_5D_JINJO_PINK, 	ACTOR_61_JINJO_PINK },
    { MARKER_5E_JINJO_YELLOW, 	ACTOR_5E_JINJO_YELLOW },
};
// clang-format on

void LogOutSpawns(int32_t actorId, int16_t posX, int16_t posY, int16_t posZ) {
    std::string locationStr = std::to_string(posX) + ", " + std::to_string(posY) + ", " + std::to_string(posZ);
    SPDLOG_INFO("Actor ID: {} | Position: {}", actorId, locationStr);
}

void LogOutCollision(int32_t actorId, int16_t posX, int16_t posY, int16_t posZ) {
    std::string locationStr = std::to_string(posX) + ", " + std::to_string(posY) + ", " + std::to_string(posZ);
    SPDLOG_INFO("Collect ID: {} | Position: {}", actorId, locationStr);
}

bool IsActorWhitelisted(int32_t actorId) {
    for (auto& entry : actorSpawnWhitelist) {
        if (entry == actorId) {
            return true;
        }
    }
    return false;
}

void SendCollisionNotification(RandoItemId randoItemId) {
    std::string prefix = "You collected ";
    prefix += Rando::StaticData::Items[randoItemId].article;
    std::string message = Rando::StaticData::Items[randoItemId].name;
    
    Notification::Emit({ .prefix = prefix,
                         .prefixColor = WIDGET_TEXT_COLOR(UIWidgets::Colors::White),
                         .message = message,
                         .messageColor = WIDGET_TEXT_COLOR(randoItemColors.at(randoItemId)) });
};

bool ShouldOverrideSpawn(int32_t posX, int32_t posY, int32_t posZ) {
    RandoCheckId randoCheckId = Rando::StaticData::GetCheckByPosition({ posX, posY, posZ });
    if (randoCheckId == RC_UNKNOWN) {
        return false;
    }

    if (CustomObject::CheckSpawnQueue(randoCheckId)) {
        return false;
    }

    if (Rando::Logic::IsCheckShuffled(randoCheckId)) {
        int32_t position[3];
        position[0] = posX;
        position[1] = posY;
        position[2] = posZ;
        CustomObject::AddToSpawnQueue(randoCheckId, position);
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

        if (map_getLevel(map_get()) != LEVEL_1_MUMBOS_MOUNTAIN) {
            return;
        }

        CustomObject::InitializeSpawnQueue();

        if (!IsActorWhitelisted(ev->actorId)) {
            return;
        }

        if (ShouldOverrideSpawn(ev->posX, ev->posY, ev->posZ)) {
            event->cancelled = true;
            ev->result = NULL;
        }
    })

    REGISTER_LISTENER(OnPropSpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnPropSpawn* ev = (OnPropSpawn*)event;

        // if (!IS_RANDO) {
        //     return;
        // }

        if (map_getLevel(map_get()) != LEVEL_1_MUMBOS_MOUNTAIN) {
            return;
        }

        if (ShouldOverrideSpawn(ev->posX, ev->posY, ev->posZ)) {
            event->cancelled = true;
        }
    })

    REGISTER_LISTENER(OnBundleSpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnBundleSpawn* ev = (OnBundleSpawn*)event;

        // if (!IS_RANDO) {
        //     return;
        // }

        if (map_getLevel(map_get()) != LEVEL_1_MUMBOS_MOUNTAIN) {
            return;
        }

        int32_t position[3];
        position[0] = (int32_t)ev->posX;
        position[1] = (int32_t)ev->posY;
        position[2] = (int32_t)ev->posZ;

        Rando::StaticData::RandoShuffledPool randoShuffledObject;
        randoShuffledObject.randoCheckId = RC_UNKNOWN;

        switch (ev->bundle_id) {
            case BUNDLE_3_MM_HUT_JINJO_GREEN:
                randoShuffledObject = Rando::Logic::GetShuffledObject(RC_MM_JINJO_GREEN);
                break;
            case BUNDLE_4_MM_HUT_JIGGY:
                if (position[1] < 2000) {
                    randoShuffledObject = Rando::Logic::GetShuffledObject(RC_MM_JIGGY_ORANGE_PADS);
                    
                } else {
                    randoShuffledObject = Rando::Logic::GetShuffledObject(RC_MM_JIGGY_HUTS);
                }
                break;
            default:
                return;
        }

        if (randoShuffledObject.randoCheckId == RC_UNKNOWN) {
            return;
        }

        *ev->result = CustomObject::SpawnCustomActor(
            (actor_e)Rando::StaticData::Items[randoShuffledObject.randoItemId].actorId, position);
        if (*ev->result == NULL) {
            return;
        }

        *ev->result = CustomObject::SetCustomActorParameters(*ev->result, randoShuffledObject.randoCheckId);
        CustomObject::AddToCustomActorMap(randoShuffledObject.randoCheckId, *ev->result);

        event->cancelled = true;
    })

    REGISTER_LISTENER(OnJiggySpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnJiggySpawn* ev = (OnJiggySpawn*)event;

        // if (!IS_RANDO) {
        //     return;
        // }

        if (map_getLevel(map_get()) != LEVEL_1_MUMBOS_MOUNTAIN) {
            return;
        }

        RandoCheckId randoCheckId = Rando::StaticData::GetCheckByJiggyId(ev->jiggyId);

        if (randoCheckId == RC_UNKNOWN) {
            return;
        }

        Rando::StaticData::RandoShuffledPool randoShuffledObject;
        randoShuffledObject.randoCheckId = RC_UNKNOWN;

        randoShuffledObject = Rando::Logic::GetShuffledObject(randoCheckId);
        if (randoShuffledObject.randoCheckId == RC_UNKNOWN) {
            return;
        }

        int32_t position[3];
        position[0] = (int32_t)ev->posX;
        position[1] = (int32_t)ev->posY;
        position[2] = (int32_t)ev->posZ;

        Actor* newCustomActor = CustomObject::SpawnCustomActor(
            (actor_e)Rando::StaticData::Items[randoShuffledObject.randoItemId].actorId, position);
        newCustomActor = CustomObject::SetCustomActorParameters(newCustomActor, randoCheckId);
        CustomObject::AddToCustomActorMap(randoCheckId, newCustomActor);

        CustomObject::InitializeSpawnQueue();
        event->cancelled = true;
    })

    REGISTER_LISTENER(OnActorCollision, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorCollision* ev = (OnActorCollision*)event;

        // if (!IS_RANDO) {
        //     return;
        // }

        if (map_getLevel(map_get()) != LEVEL_1_MUMBOS_MOUNTAIN) {
            return;
        }
        RandoItemId randoItemId = RI_UNKNOWN;

        if (!ev->propId->markerFlag) {
            switch (ev->propId->spriteProp.unk0_31) {
                case RP_MUSIC_NOTE:
                    LogOutCollision(ev->propId->spriteProp.unk0_31, ev->propId->actorProp.x, ev->propId->actorProp.y,
                                    ev->propId->actorProp.z);
                    randoItemId = RI_MUSIC_NOTE;
                    break;
                default:
                    break;
            }
        } else {
            switch (ev->propId->actorProp.marker->id) {
                case MARKER_39_MUMBO_TOKEN:
                    LogOutCollision(ACTOR_2D_MUMBO_TOKEN, ev->propId->actorProp.x, ev->propId->actorProp.y,
                                    ev->propId->actorProp.z);
                    randoItemId = RI_MUMBO_TOKEN;
                    break;
                case MARKER_52_JIGGY:
                    LogOutCollision(ACTOR_46_JIGGY, ev->propId->actorProp.x, ev->propId->actorProp.y,
                                    ev->propId->actorProp.z);
                    randoItemId = RI_JIGGY;
                    break;
                case MARKER_53_EMPTY_HONEYCOMB:
                    LogOutCollision(ACTOR_47_EMPTY_HONEYCOMB, ev->propId->actorProp.x, ev->propId->actorProp.y,
                                    ev->propId->actorProp.z);
                    randoItemId = RI_EMPTY_HONEYCOMB;
                    break;
                case MARKER_5A_JINJO_BLUE:
                case MARKER_5B_JINJO_GREEN:
                case MARKER_5C_JINJO_ORANGE:
                case MARKER_5D_JINJO_PINK:
                case MARKER_5E_JINJO_YELLOW:
                    LogOutCollision(jinjoMarkerMap.at(ev->propId->actorProp.marker->id), ev->propId->actorProp.x,
                                    ev->propId->actorProp.y,
                                    ev->propId->actorProp.z);
                    randoItemId = Rando::StaticData::GetRandoItemByActorId(jinjoMarkerMap.at(ev->propId->actorProp.marker->id));
                    break;
                default:
                    break;
            }
        }

        if (randoItemId != RI_UNKNOWN) {
            CustomObject::ObjectCollected(ev->propId);
            SendCollisionNotification(randoItemId);
        }
    })
}
