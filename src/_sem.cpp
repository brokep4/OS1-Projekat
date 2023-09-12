//
// Created by os on 5/26/22.
//
#include "../h/_sem.h"
#include "../h/Scheduler.h"

//missing part is on sem destroy, then all blocked should return 0 in wait

_sem::~_sem()
{
    while(blockedThreads_.size())
    {
        _thread* t = blockedThreads_.pop();
        t->setState(_thread::ThreadState::RUNNING);
        t->set_a0(-1);
        Scheduler::put(t);
    }
}

int _sem::kwait()
{
    if(val_ == 0)
    {
        this->blockedThreads_.add(_thread::running);
        _thread::running->setState(_thread::ThreadState::BLOCKED);
        RiscV::writeToKernelStack(80,0);//writing 0 to a0 as return value of this sys call
        //if later sem_destroy is called, this value will be overwritten, as unblocker thread can write to it
        //actually this can because of that be also written as running->set_a0(0)
        _thread::kdispatch();
    }
    else {
        val_--;
        RiscV::writeToKernelStack(80, 0);
    }
    return 0;

    /*
    val_--;
    if(val_ < 0)
    {
        this->blockedThreads_.add(_thread::running);
        _thread::running->setState(_thread::ThreadState::BLOCKED);
        RiscV::writeToKernelStack(80,0);//writing 0 to a0 as return value of this sys call
        //if later sem_destroy is called, this value will be overwritten, as unblocker thread can write to it
        //actually this can because of that be also written as running->set_a0(0)
        _thread::kdispatch();
    }
    RiscV::writeToKernelStack(80,0);
    return 0;
     */
}

int _sem::ksignal()
{
    //val_++;
    if(blockedThreads_.size() > 0)
    {
        _thread* t = blockedThreads_.pop();
        t->setState(_thread::ThreadState::RUNNING);
        Scheduler::put(t);
        t->set_a0(0);
    }
    else val_++;
    //completely non necessary, as we are going to write same in supervisorTrapHandler
    RiscV::writeToKernelStack(80,0);
    return 0;

    /*
    val_++;
    if(val_ <= 0)
    {
        _thread* t = blockedThreads_.pop();
        t->setState(_thread::ThreadState::RUNNING);
        Scheduler::put(t);
        t->set_a0(0);
    }
    return 0;
     */
}

void *_sem::operator new(size_t sz) noexcept
{
    uint64 freeMem = MemoryAllocator::count_free_memory();
    void* p = MemoryAllocator::kmem_alloc(sz);
    if(freeMem < sz && p != nullptr)
    {
        printString2("FATAL ERROR IN NEW OPERATOR FOR _SEM! MEMORY ALLOCATOR RETURNED SPACE IN MEMORY, BUT IT SHOULD BE NO SPACE LEFT!\n");
        return nullptr;
    }
    return p;
}

void _sem::operator delete(void *p)
noexcept {
    MemoryAllocator::kmem_free(p);
}