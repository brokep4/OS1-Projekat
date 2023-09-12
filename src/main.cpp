//
// Created by os on 5/23/22.
//
#include "../h/supervisorTrapHandler.h"
#include "../h/printUtils.h"
#include "../h/syscall_c.h"
#include "../h/MemoryAllocator.h"
#include "../h/Queue.h"
#include "../h/_thread.h"
#include "../h/Scheduler.h"
#include "../h/IOClass.h"
#include "../h/syscall_cpp.hpp"

void idle(void* arg)
{
    //uint64 cnt=0;
    while(1)
    {
        //for(int i = 0; i < 1000000; i++)cnt++;
        //the reason for this is timer, because if we keep dispatching on empty scheduler, timer ticks will be broken
        if(Scheduler::size() > 0)
            thread_dispatch();
    }
}


extern void userMain();

void umain(void* arg)
{
    userMain();
    sem_t sem = (sem_t)arg;
    sem_signal(sem);
}

void main()
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    MemoryAllocator::init();
    init_supervisorTrapHandler();
    Scheduler::init();
    IOClass::init();
    //main function mustn't END BEFORE ALL THREADS ARE DONE
    //i should probably add one IDLE thread, that does nothing but while(1)
    //the reason for that is just in case we later use semaphores in this main function
    _thread mainThread(nullptr,nullptr,nullptr);
    _thread::setRunning(&mainThread);
    thread_create(nullptr,&idle,nullptr);
    //RiscV::enable_interrupts();
    RiscV::changeMode(0);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///////////// PUBLIC TESTING

    sem_t semUmain;
    sem_open(&semUmain,0);
    thread_create(nullptr,umain,(void*)semUmain);

    sem_wait(semUmain);


     /////////////

     RiscV::changeMode(1);

     /*

     uint64 prev=0;
     while(1)
     {
         prev++;
         printDec(prev);
         printDec(Timer::ticks/10);
         Thread::sleep(10);
     }
      */

}
