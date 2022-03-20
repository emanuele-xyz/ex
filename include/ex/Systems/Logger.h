#pragma once

// #define ExLog(fmt, ...) ExLogInternal(__FUNCTION__ ":" __FILE__ "(" #__LINE__ "): " fmt, __VA_ARGS__)

void ExLogInternal(char* fmt, ...);

#define ExLog(fmt, ...) ExLogInternal(fmt, __VA_ARGS__)
