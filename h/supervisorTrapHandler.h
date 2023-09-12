//
// Created by os on 5/23/22.
//

#ifndef OS_PROJEKAT_DEF_SUPERVISORTRAPHANDLER_H
#define OS_PROJEKAT_DEF_SUPERVISORTRAPHANDLER_H

#include "../lib/console.h"
#include "../h/RiscV.h"
#include "../lib/hw.h"
#include "../h/printUtils.h"
#include "../h/MemoryAllocator.h"
#include "../h/_thread.h"
#include "../h/syscall_c.h"
#include "../h/Scheduler.h"
#include "../h/_sem.h"

class Timer
{
public:
    struct WaitingThread
    {
        _thread* thread;
        int timeFromPrev;
        WaitingThread* next;
        //WaitingThread* prev;
        static void* operator new(size_t sz) noexcept;
        static void operator delete(void* p) noexcept;
    };
    static uint64 ticks;
    static WaitingThread* head;
    static void kadd(_thread* thread, time_t time);
    static void kcheck();
    static void kDecHead();
};

extern "C" void supervisorTrap();
extern "C" void supervisorTrapHandler();

void init_supervisorTrapHandler();

#endif //OS_PROJEKAT_DEF_SUPERVISORTRAPHANDLER_H
