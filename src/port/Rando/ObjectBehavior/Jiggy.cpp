#include "ObjectBehavior.h"
#include "port/Rando/Logic/Logic.h"
#include "port/Rando/CustomObject/CustomObject.h"

#include <libultraship/bridge.h>
#include "port/ui/cvar_prefixes.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/enhancements/events/PortEnhancements.h"
#include "port/enhancements/events/hooks/Events.h"

// TODO: SWAP TO RANDO_SAVE_OPTIONS
#define CVAR_NAME Rando::StaticData::Options[RO_SHUFFLE_JIGGIES].cvar
#define CVAR CVarGetInteger(CVAR_NAME, 0)

void Rando::ObjectBehavior::InitJiggyBehavior() {
    COND_VB_SHOULD(VB_OVERRIDE_JIGGY_SPAWN, EVENT_PRIORITY_NORMAL, true, {
		jiggy_e jiggyId = va_arg(args, jiggy_e);
        f32* position = va_arg(args, f32*);

        if (CVAR) {
            RandoCheckId randoCheckId = Rando::StaticData::GetCheckByJiggyId(jiggyId);
            
            if (randoCheckId == RC_UNKNOWN) {
                return;
            }
            
            Rando::StaticData::RandoShuffledPool randoShuffledObject;
            randoShuffledObject.randoCheckId = RC_UNKNOWN;
            
            randoShuffledObject = Rando::Logic::GetShuffledObject(randoCheckId);
            if (randoShuffledObject.randoCheckId == RC_UNKNOWN) {
                return;
            }
            
            int32_t spawnPosition[3];
            spawnPosition[0] = (int32_t)position[0];
            spawnPosition[1] = (int32_t)position[1];
            spawnPosition[2] = (int32_t)position[2];
            
            Actor* newCustomActor = CustomObject::SpawnCustomActor(
                (actor_e)Rando::StaticData::Items[randoShuffledObject.randoItemId].actorId, spawnPosition);
            newCustomActor = CustomObject::SetCustomActorParameters(newCustomActor, randoCheckId);
            CustomObject::AddToCustomActorMap(randoCheckId, newCustomActor);
            
            ApplyCustomActorPhysics(randoCheckId, newCustomActor);
            
            CustomObject::InitializeSpawnQueue();
            *should = true;
        }
	})
}
