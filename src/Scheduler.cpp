//
// Created by os on 5/24/22.
//
#include "../h/Scheduler.h"

Queue<_thread*>* Scheduler::q = nullptr;

void Scheduler::put(_thread *t)
{
    if(q==nullptr)
    {
        char err[]="FATAL ERROR! Scheduler's queue is uninitialized\n";
        printString2(err);
        return;
    }
    q->add(t);
}

_thread *Scheduler::get()
{
    if(q==nullptr)
    {
        char err[]="FATAL ERROR! Scheduler's queue is uninitialized\n";
        printString2(err);
        return nullptr;
    }
    return q->pop();
}

void Scheduler::init()
{
    /*Queue<_thread*> qq(50);
    Scheduler::q = (Queue<_thread*>*) mem_alloc(sizeof(Queue<_thread*>));
    MemoryAllocator::kmemcpy((void*)q,(void*)&qq,sizeof(Queue<_thread*>));
     */
    Scheduler::q = new Queue<_thread*>();
}

uint64 Scheduler::size() {
    if(q==nullptr)
    {
        char err[]="FATAL ERROR! Scheduler's queue is uninitialized\n";
        printString2(err);
        return 0;
    }
    return q->size();
}