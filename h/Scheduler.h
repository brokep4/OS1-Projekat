//
// Created by os on 5/24/22.
//

#ifndef OS_PROJEKAT_DEF_SCHEDULER_H
#define OS_PROJEKAT_DEF_SCHEDULER_H
#include "../h/Queue.h"
#include "../h/_thread.h"
#include "../h/MemoryAllocator.h"
#include "../h/printUtils.h"

class _thread;

class Scheduler
{
public:
    static Queue<_thread*> *q;
    static void put(_thread* t);
    static _thread* get();
    static void init();
    static uint64 size();
};



#endif //OS_PROJEKAT_DEF_SCHEDULER_H
