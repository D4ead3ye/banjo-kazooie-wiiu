#include "ObjectBehavior.h"
#include "port/Rando/Logic/Logic.h"

#include <libultraship/bridge.h>
#include "port/ui/cvar_prefixes.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/enhancements/events/PortEnhancements.h"
#include "port/enhancements/events/hooks/Events.h"

#define MOLETABLE_SIZE 10

typedef struct {
    s16 teach_text_id;
    s16 refresher_text_id;
    s8 camera_node;
    s8 ability;
} ChMoleDescription;

extern "C" {
void func_80347A14(s32 arg0);
int ability_isUnlocked(enum ability_e uid);
void ability_unlock(enum ability_e);
extern ChMoleDescription moleTable[];
}

ChMoleDescription result;

// TODO: SWAP TO RANDO_SAVE_OPTIONS
#define CVAR_NAME Rando::StaticData::Options[RO_SHUFFLE_MOLEHILLS].cvar
#define CVAR CVarGetInteger(CVAR_NAME, 0)

ChMoleDescription GetMoleDescriptionByAbility(int16_t abilityId) {
    for (int i = 0; i < MOLETABLE_SIZE; i++) {
        if (moleTable[i].ability == abilityId) {
            result = moleTable[i];
            break;
        }
    }

    return result;
}

void Rando::ObjectBehavior::InitMolehillBehavior() {
    COND_VB_SHOULD(VB_OVERRIDE_MOLEHILL_ABILITY, EVENT_PRIORITY_NORMAL, true, {
        Actor* molehillActor = va_arg(args, Actor*);
        s32* textId = va_arg(args, s32*);
        s32* isLearned = va_arg(args, s32*);

        if (CVAR) {
            RandoCheckId randoCheckId = Rando::StaticData::GetCheckByPosition(
                molehillActor->position_x, molehillActor->position_y, molehillActor->position_z);

            if (randoCheckId == RC_UNKNOWN) {
                return;
            }

            Rando::StaticData::RandoShuffledPool shuffledMolehill = Rando::Logic::GetShuffledObject(randoCheckId);
            ChMoleDescription moleInfo = GetMoleDescriptionByAbility(shuffledMolehill.randoCollectionId);

            if (moleInfo.ability != NULL) {
                *should = true;

                if (ability_isUnlocked((ability_e)moleInfo.ability)) {
                    *textId = (s32)moleInfo.refresher_text_id;
                    *isLearned = (s32)0xf;
                } else {
                    func_80347A14(0);
                    molehillActor->has_met_before = true;
                    *textId = (s32)moleInfo.teach_text_id;
                    ability_unlock((ability_e)moleInfo.ability);
                }
                
            }
        }

    })
}
