#include "ObjectBehavior.h"
#include "port/Rando/Logic/Logic.h"
#include "port/Rando/CustomObject/CustomObject.h"

#include <libultraship/bridge.h>
#include "port/ui/cvar_prefixes.h"
#include <libultraship/bridge/consolevariablebridge.h>
#include "port/enhancements/events/PortEnhancements.h"
#include "port/enhancements/events/hooks/Events.h"

extern "C" {
extern f32 gBundle_yaw;
}

void Rando::ObjectBehavior::InitBundleBehavior() {
    COND_VB_SHOULD(VB_OVERRIDE_BUNDLE_SPAWN, EVENT_PRIORITY_NORMAL, true, {
        bundle_e bundleId = va_arg(args, bundle_e);
        BundleInfo* bundleInfo = va_arg(args, BundleInfo*);
        s32 bundleCount = va_arg(args, s32);
		f32* position = va_arg(args, f32*);
        Actor* actor = va_arg(args, Actor*);

        int32_t spawnPosition[3];
        spawnPosition[0] = (int32_t)position[0];
        spawnPosition[1] = (int32_t)position[1];
        spawnPosition[2] = (int32_t)position[2];

        Rando::StaticData::RandoShuffledPool randoShuffledObject;
        randoShuffledObject.randoCheckId = RC_UNKNOWN;

        switch (bundleId) {
            case BUNDLE_3_MM_HUT_JINJO_GREEN:
                randoShuffledObject = Rando::Logic::GetShuffledObject(RC_MM_JINJO_GREEN);
                break;
            case BUNDLE_4_MM_HUT_JIGGY:
                if (spawnPosition[1] < 2000) {
                    randoShuffledObject = Rando::Logic::GetShuffledObject(RC_MM_JIGGY_ORANGE_PADS);
                } else {
                    randoShuffledObject = Rando::Logic::GetShuffledObject(RC_MM_JIGGY_HUTS);
                }
                break;
            case BUNDLE_0_MM_HUT_MUSIC_NOTE:
                randoShuffledObject =
                    Rando::Logic::GetShuffledObject((RandoCheckId)((int32_t)RC_MM_NOTE_HUT_BUNDLE_1 + bundleCount));
                break;
            default:
                return;
        }

        if (randoShuffledObject.randoCheckId == RC_UNKNOWN) {
            return;
        }

        actor = CustomObject::SpawnCustomActor(
            (actor_e)Rando::StaticData::Items[randoShuffledObject.randoItemId].actorId, spawnPosition);
        if (actor == NULL) {
            return;
        }

        actor = CustomObject::SetCustomActorParameters(actor, randoShuffledObject.randoCheckId);
        CustomObject::AddToCustomActorMap(randoShuffledObject.randoCheckId, actor);
        if (randoShuffledObject.randoCheckId == RC_MM_JIGGY_HUTS) {
            ApplyCustomActorPhysics(randoShuffledObject.randoCheckId, actor);
        } else {
            ApplyBundleActorPhysics(actor, bundleId, (BundleInfo*)bundleInfo, gBundle_yaw);
        }

        *should = true;
	})
}
