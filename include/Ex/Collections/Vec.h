#pragma once

#include "Ex\Core.h"

template <typename T>
class Vec
{
private:
    u32 count;
    u32 size;
    T* data;
public:
    void Init();
    void Fini();

    void Push(T val);
    T Pop();
}
