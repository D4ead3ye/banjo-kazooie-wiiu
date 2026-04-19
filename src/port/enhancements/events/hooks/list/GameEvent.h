#pragma once

#include "port/enhancements/events/hooks/EventSystem.h"
#include <stdarg.h>

// clang-format off
DEFINE_EVENT(OnGameSave, 
	int32_t fileNum;
)

DEFINE_EVENT(OnGameLoad,
	int32_t fileNum;
)

DEFINE_EVENT(OnSaveFileLoad,
	int32_t fileNum;
	void* saveBuffer;
	int32_t result;
)

DEFINE_EVENT(OnSaveFileSave,
	void* saveBuffer;
	int32_t fileNum;
	int32_t * result;
)

DEFINE_EVENT(OnWarpDispatch,
	int32_t warpId;
	int32_t warpDest;
)
// clang-format on