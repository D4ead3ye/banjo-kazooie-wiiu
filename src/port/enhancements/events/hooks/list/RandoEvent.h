#pragma once

#include "port/enhancements/events/hooks/EventSystem.h"
#include <stdarg.h>

#include "enums.h"
#include "prop.h"

// clang-format off
DEFINE_EVENT(OnSaveCreate,
	void* saveData;
)

DEFINE_EVENT(OnActorSpawn,
	int32_t posX;
	int32_t posY;
	int32_t posZ;
	Actor* result;
)

DEFINE_EVENT(OnPropSpawn,
	int32_t propId;
	int32_t posX;
	int32_t posY;
	int32_t posZ;
)

DEFINE_EVENT(OnActorCollision,
	Prop* propId;
)
// clang-format on