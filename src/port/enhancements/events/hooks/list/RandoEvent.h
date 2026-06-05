#pragma once

#include <libultraship/bridge/eventsbridge.h>
#include "port/Rando/Types.h"

#include "enums.h"
#include "prop.h"

// clang-format off
DEFINE_EVENT(OnSaveLoad,
	void* saveData;
)

DEFINE_EVENT(OnActorSpawn,
	int32_t actorId;
	int32_t posX;
	int32_t posY;
	int32_t posZ;
	Actor* result;
)

DEFINE_EVENT(OnActorSaveState)

DEFINE_EVENT(OnActorCollision,
	Prop* propId;
)

DEFINE_EVENT(OnFindActorFromActorId,
	int32_t actorId;
	Actor* result;
)

DEFINE_EVENT(OnIsJiggyScoreCollected,
	int32_t jiggyId;
	int32_t result;
)

DEFINE_EVENT(OnIsJiggyScoreSpawned,
	int32_t jiggyId;
	int32_t result;
)

DEFINE_EVENT(SetRandoInfFlag,
	int32_t flagId;
	int32_t flagState;
)
// clang-format on