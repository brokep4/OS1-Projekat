//
// Created by os on 5/26/22.
//

#ifndef OS_PROJEKAT_DEF__SEM_H
#define OS_PROJEKAT_DEF__SEM_H

#include "Queue.h"
#include "_thread.h"

class _sem
{
    Queue<_thread*> blockedThreads_;
    uint64 val_;
public:
    _sem(uint64 v) : val_(v) {}
    ~_sem();
    int kwait();
    int ksignal();
    static void* operator new(size_t sz) noexcept;
    static void operator delete(void* p) noexcept;
};


#endif //OS_PROJEKAT_DEF__SEM_H
