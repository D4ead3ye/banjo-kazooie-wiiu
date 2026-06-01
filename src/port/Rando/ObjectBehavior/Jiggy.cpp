#include "ObjectBehavior.h"
#include "port/Rando/Logic/Logic.h"
#include "port/Rando/CustomObject/CustomObject.h"

#include <libultraship/bridge.h>
#include "port/ui/cvar_prefixes.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/enhancements/events/PortEnhancements.h"
#include "port/enhancements/events/hooks/Events.h"

#define OPTION_ENABLED RANDO_SAVE_OPTIONS[RO_SHUFFLE_JIGGIES].optionValue

void Rando::ObjectBehavior::InitJiggyBehavior() {
    COND_VB_SHOULD(VB_OVERRIDE_JIGGY_SPAWN, EVENT_PRIORITY_NORMAL, true, {
		jiggy_e jiggyId = va_arg(args, jiggy_e);
        f32* position = va_arg(args, f32*);

        LogOutSpawns(jiggyId, position[0], position[1], position[2]);

        if (!IS_RANDO && !OPTION_ENABLED) {
            return;
        }

        RandoCheckId randoCheckId = Rando::StaticData::GetCheckByJiggyId(jiggyId);
        
        if (randoCheckId == RC_UNKNOWN) {
            return;
        }
        
        RandoSaveCheck shuffledObject;
        shuffledObject.randoCheckId = RC_UNKNOWN;
        
        shuffledObject = Rando::Logic::GetShuffledObject(randoCheckId);
        if (shuffledObject.randoCheckId == RC_UNKNOWN) {
            return;
        }

        actor_e randoActorId = GetActorIdByShuffledObjectState(shuffledObject);

        if (randoActorId == ACTOR_1_UNKNOWN) {
            *should = true;
            return;
        }

        int32_t spawnPosition[3];
        spawnPosition[0] = (int32_t)position[0];
        spawnPosition[1] = (int32_t)position[1];
        spawnPosition[2] = (int32_t)position[2];
        
        Actor* newCustomActor = CustomObject::SpawnCustomActor(randoActorId, spawnPosition);
        newCustomActor = CustomObject::SetCustomActorParameters(newCustomActor, randoCheckId);
        CustomObject::AddToCustomActorMap(randoCheckId, newCustomActor);
        
        if (jiggyId != JIGGY_17_CC_CLANKER_RAISED) {
            ApplyCustomActorPhysics(randoCheckId, newCustomActor, false);
        }
        
        CustomObject::InitializeSpawnQueue();
        *should = true;
	})
}
