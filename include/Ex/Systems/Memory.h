#pragma once

#include <new>

#include "Ex/Core.h"
#include "Ex/Systems/Logger.h"

template<typename T>
T* ExAlloc(sz count)
{
    T* tmp = new (std::nothrow) T[count];
    if (!tmp) ExLog("Memory allocation failed");
    ExAssert(tmp);

    return tmp;
}

template<typename T>
T* ExAlloc()
{
    T* tmp = new (std::nothrow) T;
    if (!tmp) ExLog("Memory allocation failed");
    ExAssert(tmp);

    return tmp;
}

template<typename T>
void ExFree(T* p)
{
    if (!p) ExLog("Freeing a null pointer");
    ExAssert(p);

    delete p;
}
