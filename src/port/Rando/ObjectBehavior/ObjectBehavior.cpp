#include "ObjectBehavior.h"
#include "port/ui/UIWidgets.hpp"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/ui/Notification.h"
#include "port/Rando/Logic/Logic.h"
#include "port/enhancements/events/hooks/Events.h"
#include "port/Rando/CustomObject/CustomObject.h"

#define WIDGET_TEXT_COLOR(id) UIWidgets::ColorValues.at(id)

extern "C" {
Actor* marker_getActor(ActorMarker* thisx);
bool func_802C9C14(Actor* actor);

void coMusicPlayer_playMusic(enum comusic_e track_id, s32 volume);
void marker_despawn(ActorMarker* marker);
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
    //ACTOR_12C_MOLEHILL,
};

std::map<int32_t, UIWidgets::Colors> randoItemColors = {
    { RI_EMPTY_HONEYCOMB,   UIWidgets::Colors::Yellow },
    { RI_JIGGY,             UIWidgets::Colors::Yellow },
    { RI_JINJO_BLUE,        UIWidgets::Colors::SkyBlue },
    { RI_JINJO_GREEN,       UIWidgets::Colors::Green },
    { RI_JINJO_ORANGE,      UIWidgets::Colors::Orange },
    { RI_JINJO_PINK,        UIWidgets::Colors::Pink },
    { RI_JINJO_YELLOW,      UIWidgets::Colors::Yellow },
    { RI_MOLEHILL,          UIWidgets::Colors::Purple },
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

bool nextActorSaveState = false;

void LogOutSpawns(int32_t actorId, int16_t posX, int16_t posY, int16_t posZ) {
    std::string locationStr = std::to_string(posX) + ", " + std::to_string(posY) + ", " + std::to_string(posZ);
    BK_LOG_INFO("Actor ID: %i | Position: %s", actorId, locationStr.c_str());
}

void LogOutCollision(int32_t actorId, int16_t posX, int16_t posY, int16_t posZ) {
    std::string locationStr = std::to_string(posX) + ", " + std::to_string(posY) + ", " + std::to_string(posZ);
    BK_LOG_INFO("Collect ID: %i | Position: %s", actorId, locationStr);
}

bool IsActorWhitelisted(int32_t actorId) {
    for (auto& entry : actorSpawnWhitelist) {
        if (entry == actorId) {
            return true;
        }
    }

    if (CVarGetInteger(Rando::StaticData::Options[RO_SPAWN_JUNK].cvar, 0) == RO_GENERIC_ON) {
        for (auto& junk : junkItemList) {
            if (junk == actorId) {
                return true;
            }
        }
    }

    return false;
}

int32_t GetJinjoActorMarkerId(actor_e actorId) {
    for (auto& [marker, actor] : jinjoMarkerMap) {
        if (actor == actorId) {
            return marker;
        }
    }

    return NULL;
}

void Rando::StaticData::SendCollisionNotification(RandoItemId randoItemId) {
    std::string prefix = randoItemId == RI_MOLEHILL ? "You learned " : "You collected ";
    prefix += Rando::StaticData::Items[randoItemId].article;
    std::string message = Rando::StaticData::Items[randoItemId].name;
    
    Notification::Emit({ .prefix = prefix,
                         .prefixColor = WIDGET_TEXT_COLOR(UIWidgets::Colors::White),
                         .message = message,
                         .messageColor = WIDGET_TEXT_COLOR(randoItemColors.at(randoItemId)) });
};

bool ShouldOverrideSpawn(RandoCheckId randoCheckId) {
    if (randoCheckId == RC_UNKNOWN) {
        return false;
    }

    if (Rando::Logic::IsCheckShuffled(randoCheckId)) {
        return true;
    }

    return false;
}

// Entry point for the module, run once on game boot
void Rando::ObjectBehavior::Init() {
    InitBundleBehavior();
    InitJiggyBehavior();
    InitJinjoBehavior();
    InitMolehillBehavior();
    InitMusicNoteBehavior();
    InitPropBehavior();

    UpdateJunkList();

    REGISTER_LISTENER(OnActorSpawn, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorSpawn* ev = (OnActorSpawn*)event;

        if (!IS_RANDO) {
            return;
        }

        CustomObject::FlushRandoSpawnQueue();

        if (!IsActorWhitelisted(ev->actorId)) {
            return;
        }

        if (ev->actorId != ACTOR_2D_MUMBO_TOKEN && (ev->posX == 9823, ev->posY == 4225, ev->posZ == -19)) {
            return;
        }

        int32_t position[3];
        position[0] = ev->posX;
        position[1] = ev->posY;
        position[2] = ev->posZ;

        RandoCheckId randoCheckId = Rando::StaticData::GetCheckByPosition(ev->posX, ev->posY, ev->posZ);

        Actor* randoCustomActor = CustomObject::ShouldCreateCustomActorEX(randoCheckId, position, false);
        if (randoCustomActor == NULL) {
            return;
        }

        event->Cancelled = true;
        ev->result = randoCustomActor;
    })

    REGISTER_LISTENER(OnActorSaveState, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorSaveState* ev = (OnActorSaveState*)event;

        if (!IS_RANDO) {
            return;
        }

        if (!IsActorWhitelisted(ev->actorId)) {
            return;
        }

        event->Cancelled = true;
    })

    REGISTER_LISTENER(OnActorCollision, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnActorCollision* ev = (OnActorCollision*)event;

        if (!IS_RANDO) {
            return;
        }

        SPDLOG_INFO("Collected {} at {}, {}, {}", std::to_string(ev->propId->actorProp.marker->id),
                    std::to_string(ev->propId->actorProp.x), std::to_string(ev->propId->actorProp.y),
                    std::to_string(ev->propId->actorProp.z));

        RandoCheckId randoCheckId = Rando::StaticData::GetCheckByPosition(
            ev->propId->actorProp.x, ev->propId->actorProp.y, ev->propId->actorProp.z);

        if (randoCheckId == RC_UNKNOWN) {
            level_e currentLevel = map_getLevel(gsworld_getMap());

            switch (currentLevel) {
                case LEVEL_1_MUMBOS_MOUNTAIN:
                    break;
                case LEVEL_2_TREASURE_TROVE_COVE:
                    break;
                case LEVEL_3_CLANKERS_CAVERN:
                    break;
                case LEVEL_4_BUBBLEGLOOP_SWAMP:
                    break;
                case LEVEL_5_FREEZEEZY_PEAK:
                    break;
                case LEVEL_6_LAIR:
                    break;
                case LEVEL_7_GOBIS_VALLEY:
                    break;
                case LEVEL_8_CLICK_CLOCK_WOOD:
                    break;
                case LEVEL_9_RUSTY_BUCKET_BAY:
                    break;
                case LEVEL_A_MAD_MONSTER_MANSION:
                    break;
                case LEVEL_B_SPIRAL_MOUNTAIN:
                    if (ev->propId->actorProp.y >= 400 && ev->propId->actorProp.y <= 700) {
                        randoCheckId = RC_SM_EMPTY_HONEYCOMB_COLLIWOBBLE;
                    } else {
                        randoCheckId = RC_SM_EMPTY_HONEYCOMB_QUARRIES;
                    }
                    break;
                default:
                    return;
            }
        }

        if (randoCheckId == RC_UNKNOWN) {
            return;
        }

        RandoItemId randoItemId = RI_UNKNOWN;
        if (ev->propId->markerFlag) {
            Actor* markerActor = marker_getActor(ev->propId->actorProp.marker);

            if (markerActor->is_bundle && func_802C9C14(markerActor)) {
                return;
            }
            switch (ev->propId->actorProp.marker->id) {
                case MARKER_39_MUMBO_TOKEN:
                    randoItemId = RI_MUMBO_TOKEN;
                    break;
                case MARKER_52_JIGGY:
                    randoItemId = RI_JIGGY;
                    break;
                case MARKER_53_EMPTY_HONEYCOMB:
                    randoItemId = RI_EMPTY_HONEYCOMB;
                    break;
                case MARKER_5A_JINJO_BLUE:
                case MARKER_5B_JINJO_GREEN:
                case MARKER_5C_JINJO_ORANGE:
                case MARKER_5D_JINJO_PINK:
                case MARKER_5E_JINJO_YELLOW:
                    randoItemId = Rando::StaticData::GetRandoItemByActorId(
                        jinjoMarkerMap.at(ev->propId->actorProp.marker->id));
                    break;
                case MARKER_5F_MUSIC_NOTE:
                    randoItemId = RI_MUSIC_NOTE;
                    marker_despawn(ev->propId->actorProp.marker);
                    coMusicPlayer_playMusic(COMUSIC_9_NOTE_COLLECTED, 16000);
                    break;
                default:
                    return;
            }
        }

        if (randoItemId != RI_UNKNOWN) {
            if (randoItemId == RI_MUSIC_NOTE) {
                event->Cancelled = true;
            }
            CustomObject::ObjectCollectedEX(randoCheckId);
        }
    })

    REGISTER_LISTENER(OnSetJiggyList, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnSetJiggyList* ev = (OnSetJiggyList*)event;

        if (!IS_RANDO) {
            return;
        }

        CustomObject::ClearRandoActorListEX();
    })

    REGISTER_LISTENER(OnFindActorFromActorId, EVENT_PRIORITY_NORMAL, [](IEvent* event) {
        OnFindActorFromActorId* ev = (OnFindActorFromActorId*)event;
        int32_t position[3];
        Rando::StaticData::RandoStaticCheck randoStaticCheck;
        RandoSaveCheck randoShuffledObject;
        Actor* refActor;

        if (!IS_RANDO) {
            return;
        }

        switch (ev->actorId) {
            case ACTOR_46_JIGGY:
                // TODO: Reimplement using new system...
                // if (gsworld_getMap() == MAP_26_MMM_NAPPERS_ROOM) {
                //     randoStaticCheck = Rando::StaticData::Checks[RC_MMM_JIGGY_MANSION_TABLE];
                //     randoShuffledObject = Rando::Logic::GetShuffledObject(RC_MMM_JIGGY_MANSION_TABLE);
                // 
                //     refActor = CustomObject::GetCustomActor(RC_MMM_JIGGY_MANSION_TABLE);
                //     if (!refActor) {
                //         position[0] = randoStaticCheck.posX;
                //         position[1] = randoStaticCheck.posY;
                //         position[2] = randoStaticCheck.posZ;
                // 
                //         refActor = CustomObject::SpawnCustomActor(
                //             (actor_e)Rando::StaticData::Checks[randoShuffledObject.shuffledCheckId].actorId, position);
                //     }
                //     if (refActor) {
                //         CustomObject::AddToCustomActorMap(RC_MMM_JIGGY_MANSION_TABLE, refActor);
                //         event->Cancelled = true;
                //         ev->result = refActor;
                //     }
                // }
                break;
            default:
                break;
        }
    })
}
