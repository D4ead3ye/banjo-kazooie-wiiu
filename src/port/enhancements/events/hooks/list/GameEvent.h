#pragma once

#include "port/enhancements/events/hooks/EventSystem.h"
#include <stdarg.h>

DEFINE_EVENT(OnGameFileSave,
	int32_t fileNum;	
)

DEFINE_EVENT(OnGameFileLoad,
	int32_t fileNum;	
)