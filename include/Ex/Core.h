#pragma once

// ----------------------------------------------------------------------------
// Types
// ----------------------------------------------------------------------------

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef size_t sz;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef i8  b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;

typedef char c8;

typedef float  f32;
typedef double f64;

// ----------------------------------------------------------------------------
// Standard Library
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Macros
// ----------------------------------------------------------------------------

#define ExUnused(x) (void)(x)

// TODO: debugbreak if debug, just crash in release
#define ExCrash() do { __debugbreak(); } while (false)

// TODO: should assertions work only in debug?
// They are here to make sure we correctly use the API and to make sure
// we don't violate function's contracts
#define ExAssert(p) do { if (!(p)) { ExCrash(); }  } while (false)

#define ExUnreachableCodePath() ExAssert(false)

#define ExArraySize(a) (sizeof(a) / sizeof(*(a)))

// ----------------------------------------------------------------------------
