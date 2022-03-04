#pragma once

// ----------------------------------------------------------------------------
// Types
// ----------------------------------------------------------------------------

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef char c8;

typedef float  f32;
typedef double f64;

typedef u32 b32;

#define true  1
#define false 0

// ----------------------------------------------------------------------------
// Standard Library
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
// Macros
// ----------------------------------------------------------------------------

#define ExUnused(x) (void)(x)

#define ExCrash() do { *(int*)(0) = 0; } while (false)
#define ExCrashMsg(msg) do { *(int*)(0) = 0; } while (false)

#define ExAssert(p) do { if (!(p)) { ExCrash(); }  } while (false)
#define ExAssertMsg(p, msg) do { if (!(p)) { ExCrash(); }  } while (false)

#define ExUnreachableCodePath() ExCrashMsg("Unreachable code path")

// ----------------------------------------------------------------------------
