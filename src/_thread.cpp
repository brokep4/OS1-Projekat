//
// Created by os on 5/24/22.
//
#include "../h/_thread.h"
#include "../h/Scheduler.h"

_thread* _thread::running= nullptr;
time_t _thread::timeLeft = 2;

void _thread::setRunning(_thread *t)
{
    running = t;
}

void _thread::setState(ThreadState state)
{
    this->threadState_ = state;
}

void _thread::wrapper()
{
    if(running == nullptr)
    {
        printString2("FATAL ERROR! RUNNING IS NULLPTR\n");
        return;
    }
    if(running->stack_ == nullptr)
    {
        //unsuccessful allocation of thread's stack...not enough memory.
        //we also shouldn't save context of thread that is finished, to avoid conflict
        //main thread will have stack_==nullptr, but will never execute this wrapper function
        printString2("\nFATAL ERROR! THREAD IS IN WRAPPER, BUT WITHOUT STACK\n");
        running->setState(ThreadState::FINISHED);
        thread_dispatch();
        return;
    }
    if(running->getState() == ThreadState::FINISHED)
    {
        printString2("FATAL ERROR! THIS SHOULD NEVER HAPPEN! THREAD THAT IF FINISHED IS IN WRAPPER! LIKELY STACK COULDN'T BE ALLOCATED, BUT SOMEHOW WRAPPER WAS CALLED(PROTECTION FROM IT IS IN KSTART)\n");
        return;
    }
    running->setState(ThreadState::RUNNING);
    running->body_(running->arg_);
    running->setState(ThreadState::FINISHED);
    //we should add SYSTEM CALL DISPATCH BELOW
    thread_dispatch();
}

_thread::ThreadState _thread::getState()
{
    return this->threadState_;
}

void _thread::kyield(_thread *oldT, _thread *newT)
{
    if(oldT == nullptr || newT == nullptr)
    {
        char err[]="FATAL ERROR! oldT or newT was nullptr when calling yield\n";
        printString2(err);
        return;
    }
    char* kernelSP = (char*)RiscV::get_sscratch() - 256;

    if(oldT->getState() != ThreadState::FINISHED)
    {
        //if thread has finished, there is no point to save its context
        //also, this helps us to free stack from thread that is no longer running
        oldT->context_.x0 = *((uint64*)(kernelSP+0));
        oldT->context_.x1 = *((uint64*)(kernelSP+8));
        oldT->context_.x2 = *((uint64*)(kernelSP+16));
        oldT->context_.x3 = *((uint64*)(kernelSP+24));
        oldT->context_.x4 = *((uint64*)(kernelSP+32));
        oldT->context_.x5 = *((uint64*)(kernelSP+40));
        oldT->context_.x6 = *((uint64*)(kernelSP+48));
        oldT->context_.x7 = *((uint64*)(kernelSP+56));
        oldT->context_.x8 = *((uint64*)(kernelSP+64));
        oldT->context_.x9 = *((uint64*)(kernelSP+72));
        oldT->context_.x10 = *((uint64*)(kernelSP+80));
        oldT->context_.x11 = *((uint64*)(kernelSP+88));
        oldT->context_.x12 = *((uint64*)(kernelSP+96));
        oldT->context_.x13 = *((uint64*)(kernelSP+104));
        oldT->context_.x14 = *((uint64*)(kernelSP+112));
        oldT->context_.x15 = *((uint64*)(kernelSP+120));
        oldT->context_.x16 = *((uint64*)(kernelSP+128));
        oldT->context_.x17 = *((uint64*)(kernelSP+136));
        oldT->context_.x18 = *((uint64*)(kernelSP+144));
        oldT->context_.x19 = *((uint64*)(kernelSP+152));
        oldT->context_.x20 = *((uint64*)(kernelSP+160));
        oldT->context_.x21 = *((uint64*)(kernelSP+168));
        oldT->context_.x22 = *((uint64*)(kernelSP+176));
        oldT->context_.x23 = *((uint64*)(kernelSP+184));
        oldT->context_.x24 = *((uint64*)(kernelSP+192));
        oldT->context_.x25 = *((uint64*)(kernelSP+200));
        oldT->context_.x26 = *((uint64*)(kernelSP+208));
        oldT->context_.x27 = *((uint64*)(kernelSP+216));
        oldT->context_.x28 = *((uint64*)(kernelSP+224));
        oldT->context_.x29 = *((uint64*)(kernelSP+232));
        oldT->context_.x30 = *((uint64*)(kernelSP+240));
        oldT->context_.x31 = *((uint64*)(kernelSP+248));
        oldT->pc_ = RiscV::get_sepc();
    }
    else
    {
        //we know for a fact that this thread will never become active again, therefore we can free its stack
        //since we are still in trap free should be called as kernel function, not sys call
        MemoryAllocator::kmem_free(oldT->stack_);
        oldT->stack_=nullptr;
    }

    *((uint64*)(kernelSP+0)) = newT->context_.x0;
    *((uint64*)(kernelSP+8)) = newT->context_.x1;
    *((uint64*)(kernelSP+16)) = newT->context_.x2;
    *((uint64*)(kernelSP+24)) = newT->context_.x3;
    *((uint64*)(kernelSP+32)) = newT->context_.x4;
    *((uint64*)(kernelSP+40)) = newT->context_.x5;
    *((uint64*)(kernelSP+48)) = newT->context_.x6;
    *((uint64*)(kernelSP+56)) = newT->context_.x7;
    *((uint64*)(kernelSP+64)) = newT->context_.x8;
    *((uint64*)(kernelSP+72)) = newT->context_.x9;
    *((uint64*)(kernelSP+80)) = newT->context_.x10;
    *((uint64*)(kernelSP+88)) = newT->context_.x11;
    *((uint64*)(kernelSP+96)) = newT->context_.x12;
    *((uint64*)(kernelSP+104)) = newT->context_.x13;
    *((uint64*)(kernelSP+112)) = newT->context_.x14;
    *((uint64*)(kernelSP+120)) = newT->context_.x15;
    *((uint64*)(kernelSP+128)) = newT->context_.x16;
    *((uint64*)(kernelSP+136)) = newT->context_.x17;
    *((uint64*)(kernelSP+144)) = newT->context_.x18;
    *((uint64*)(kernelSP+152)) = newT->context_.x19;
    *((uint64*)(kernelSP+160)) = newT->context_.x20;
    *((uint64*)(kernelSP+168)) = newT->context_.x21;
    *((uint64*)(kernelSP+176)) = newT->context_.x22;
    *((uint64*)(kernelSP+184)) = newT->context_.x23;
    *((uint64*)(kernelSP+192)) = newT->context_.x24;
    *((uint64*)(kernelSP+200)) = newT->context_.x25;
    *((uint64*)(kernelSP+208)) = newT->context_.x26;
    *((uint64*)(kernelSP+216)) = newT->context_.x27;
    *((uint64*)(kernelSP+224)) = newT->context_.x28;
    *((uint64*)(kernelSP+232)) = newT->context_.x29;
    *((uint64*)(kernelSP+240)) = newT->context_.x30;
    *((uint64*)(kernelSP+248)) = newT->context_.x31;
    RiscV::set_sepc(newT->pc_);
}

void _thread::kdispatch()
{
    //this function should be called only during kernel mode/interrupts disabled
    //actually this function can be called ONLY during trap routine
    //in that way, all threads that should swap will be blocked during trap routine
    //so we don't have to worry about ra register, as their PC in _thread will point to instruction threads were interrupted
    //and we will continue right from there
    _thread* oldT = running;
    if(oldT == nullptr)
    {
        char err[]="FATAL ERROR! Running was nullptr, but kdispatch was called!\n";
        printString2(err);
        return;
    }
    if(oldT->getState() != ThreadState::FINISHED && oldT->getState() != ThreadState::BLOCKED)
    {
        Scheduler::put(oldT);
    }
    _thread* newT = Scheduler::get();
    if(newT == nullptr)
    {
        char err[]="FATAL ERROR! Empty scheduler during kdispatch! We are currently in interrupts disabled mode, likely in trap. Everything might crash\n";
        printString2(err);
        return;
    }
    //we have to change running manually
    running = newT;
    if(oldT != newT)
    {
        kyield(oldT,newT);
    }
    _thread::setTimeLeft(DEFAULT_TIME_SLICE);
    //after this function we should go back to where it was called
    //therefore, yield will actually change kernel stack to state it was when newT was in trap
    //so after we exit trap routine, context_ of newT will be loaded in supervisorTrap
}

int _thread::kstart()
{
    if(this->threadState_ == ThreadState::FINISHED)
    {
        //this means that we weren't able to allocate stack for thread
        return -1;
    }
    this->threadState_ = ThreadState::RUNNING;
    Scheduler::put(this);
    return 0;
}

int _thread::kthread_exit()
{
    running->setState(ThreadState::FINISHED);
    kdispatch();
    if(running==nullptr)return -1;
    return 0;
}

_thread* _thread::kmakeDynamic(PF body, void* arg, void* stack)
{
    _thread t(body,arg,stack);
    _thread* tt = (_thread*)MemoryAllocator::kmem_alloc(sizeof(_thread));
    if(tt==nullptr)return tt;
    if(t.stack_ == nullptr)
    {
        mem_free(tt);
        return nullptr;
    }
    MemoryAllocator::kmemcpy((void*)tt,&t,sizeof(_thread));
    return tt;
}

_thread* _thread::kmakeDynamic(PF body, void* arg)
{
    _thread t(body,arg);
    _thread* tt = (_thread*)MemoryAllocator::kmem_alloc(sizeof(_thread));
    if(tt==nullptr)return tt;
    if(t.stack_ == nullptr)
    {
        mem_free(tt);
        return nullptr;
    }
    MemoryAllocator::kmemcpy((void*)tt,&t,sizeof(_thread));
    return tt;
}

void _thread::set_a0(uint64 val)
{
    context_.x10 = val;
}

time_t _thread::getTimeLeft()
{
    return timeLeft;
}

void _thread::setTimeLeft(time_t val)
{
    timeLeft = val;
}

//class Timer;

#include "../h/supervisorTrapHandler.h"

int _thread::ksleep(time_t time)
{
    if(running== nullptr)return -1;
    running->setState(ThreadState::BLOCKED);
    Timer::kadd(running, time);
    kdispatch();
    return 0;
}

void *_thread::operator new(size_t sz) noexcept
{
    uint64 freeMem = MemoryAllocator::count_free_memory();
    void* p = MemoryAllocator::kmem_alloc(sz);
    if(freeMem < sz && p != nullptr)
    {
        printString2("FATAL ERROR IN NEW OPERATOR FOR _THREAD! MEMORY ALLOCATOR RETURNED SPACE IN MEMORY, BUT IT SHOULD BE NO SPACE LEFT!\n");
        return nullptr;
    }
    return p;
}

void _thread::operator delete(void *p)
noexcept {
    MemoryAllocator::kmem_free(p);
}