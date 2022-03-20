#pragma once

#include "Ex/Core.h"

#define ExLog(fmt, ...) ExLogInternal(__FUNCTION__, __FILE__, __LINE__, fmt, __VA_ARGS__)

void ExLogInternal(const char* function, const char* file, i32 line, char* fmt, ...);
