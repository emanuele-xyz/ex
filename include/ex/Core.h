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

typedef char c8;

typedef float  f32;
typedef double f64;

typedef i8  b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;

// ----------------------------------------------------------------------------
// Standard Library
// ----------------------------------------------------------------------------

#include <new>

// ----------------------------------------------------------------------------
// Macros
// ----------------------------------------------------------------------------

#define ExUnused(x) (void)(x)

#define ExCrash() do { __debugbreak(); } while (false)
#define ExCrashMsg(msg) do { *(int*)(0) = 0; } while (false)

#define ExAssert(p) do { if (!(p)) { ExCrash(); }  } while (false)
#define ExAssertMsg(p, msg) do { if (!(p)) { ExCrash(); }  } while (false)
#define ExAssertFmt(p, fmt, ...) do { if (!(p)) { ExCrash(); }  } while (false)

#define ExUnreachableCodePath() ExCrashMsg("Unreachable code path")

#define ExArraySize(a) (sizeof(a) / sizeof(*(a)))

// ----------------------------------------------------------------------------
// Templated Functions
// ----------------------------------------------------------------------------

template<typename T>
T* ExAlloc(sz count)
{
    T* tmp = new (std::nothrow) T[count];
    ExAssertMsg(tmp, "Memory allocation failed");
    return tmp;
}

template<typename T>
T* ExAlloc()
{
    T* tmp = new (std::nothrow) T;
    ExAssertMsg(tmp, "Memory allocation failed");
    return tmp;
}

template<typename T>
void ExFree(T* p)
{
    ExAssertMsg(p, "Freeing a null pointer");
    delete p;
}

// ----------------------------------------------------------------------------
