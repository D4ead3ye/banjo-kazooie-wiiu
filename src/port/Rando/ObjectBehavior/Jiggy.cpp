#include "ObjectBehavior.h"
#include "port/Rando/Logic/Logic.h"
#include "port/Rando/CustomObject/CustomObject.h"
#include "port/enhancements/events/hooks/Events.h"

#include "spdlog/spdlog.h"

#define OPTION_ENABLED RANDO_SAVE_OPTIONS[RO_SHUFFLE_JIGGIES].optionValue

void Rando::ObjectBehavior::InitJiggyBehavior() {
    COND_VB_SHOULD(VB_OVERRIDE_JIGGY_SPAWN, EVENT_PRIORITY_NORMAL, true, {
		jiggy_e jiggyId = va_arg(args, jiggy_e);
        f32* position = va_arg(args, f32*);

        if (!IS_RANDO && !OPTION_ENABLED) {
            return;
        }

        RandoCheckId randoCheckId = Rando::StaticData::GetCheckByJiggyId(jiggyId);
        
        if (randoCheckId == RC_UNKNOWN) {
            return;
        }

        int32_t spawnPosition[3];
        spawnPosition[0] = (int32_t)position[0];
        spawnPosition[1] = (int32_t)position[1];
        spawnPosition[2] = (int32_t)position[2];

        if (jiggyId == JIGGY_26_BGS_TANKTUP) {
            spawnPosition[0] -= 300;
            spawnPosition[1] += 100;
            spawnPosition[2] -= 300;
        }

        Actor* randoCustomActor = CustomObject::ShouldCreateCustomActorEX(randoCheckId, spawnPosition, false);
        if (jiggyId != JIGGY_17_CC_CLANKER_RAISED && jiggyId != JIGGY_1B_CC_TOOTH) {
            ApplyCustomActorPhysics(randoCheckId, randoCustomActor, false);
        }
        *should = true;
	})
}
