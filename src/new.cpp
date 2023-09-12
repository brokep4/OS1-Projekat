//
// Created by os on 5/24/22.
//

#include "../h/syscall_c.h"

using size_t = decltype(sizeof(0));

void *operator new(size_t n) noexcept
{
    return mem_alloc(n);
}

void *operator new[](size_t n) noexcept
{
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
mem_free(p);
}

void operator delete[](void *p) noexcept
{
mem_free(p);
}