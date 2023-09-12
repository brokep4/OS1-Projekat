//
// Created by os on 5/24/22.
//

#ifndef OS_PROJEKAT_DEF__THREAD_H
#define OS_PROJEKAT_DEF__THREAD_H

#include "../h/MemoryAllocator.h"
#include "../h/printUtils.h"

typedef void(*PF)(void*);

class _thread
{
public:
    enum class ThreadState
    {
        NOTSTARTED, RUNNING, FINISHED, BLOCKED
    };
private:
    struct PCB
    {
        uint64 /*zero*/x0;//0
        uint64 /*ra*/x1;//8
        uint64 /*sp*/x2;//16
        uint64 x3;//24
        uint64 x4;//32
        uint64 x5;//40
        uint64 x6;//48
        uint64 x7;//56
        uint64 x8;//64
        uint64 x9;//72
        uint64 /*a0*/x10;//80
        uint64 x11;//88
        uint64 x12;//96
        uint64 x13;//104
        uint64 x14;//112
        uint64 x15;//120
        uint64 x16;//128
        uint64 /*a7*/x17;//136
        uint64 x18;//144
        uint64 x19;//152
        uint64 x20;//160
        uint64 x21;//168
        uint64 x22;//176
        uint64 x23;//184
        uint64 x24;//192
        uint64 x25;//200
        uint64 x26;//208
        uint64 x27;//216
        uint64 x28;//224
        uint64 x29;//232
        uint64 x30;//240
        uint64 x31;//248
        PCB()
        {

            x0=0;
            x1=0; x2=0; x3=0; x4=0; x5=0; x6=0; x7=0; x8=0;
            x9=0; x10=0; x11=0; x12=0; x13=0; x14=0; x15=0; x16=0;
            x17=0; x18=0; x19=0; x20=0; x21=0; x22=0; x23=0; x24=0;
            x25=0; x26=0; x27=0; x28=0; x29=0; x30=0; x31=0;

        }
    };
    PCB context_;
    uint64 pc_;
    //typedef void(*PF)(void*);
    PF body_;
    void* arg_;
    void* stack_;
    static const uint64 DEFAULT_STACK_SIZE = 4096;
    static time_t timeLeft;

    ThreadState threadState_=ThreadState::NOTSTARTED;


public:
    static _thread* running;
    //constructors should be executed with interrupts disabled
    _thread(PF body, void* arg, void* stack) : body_(body), arg_(arg), stack_(stack) {
        pc_ = (uint64)&wrapper;
    }
    _thread(PF body, void* arg) : body_(body), arg_(arg)
    {
        stack_ = MemoryAllocator::kmem_alloc(DEFAULT_STACK_SIZE);
        if(stack_ == nullptr)
        {
            //printString("ERROR NO AVAILABLE SPACE FOR NEW STACK!\n");
            setState(ThreadState::FINISHED);//cheat around, if there is no space for stack of this thread
            //when we call kstart, this thread won't start, because its ThreadState is set to FINISHED
        }
        else {
            context_.x2 = (uint64) ((char *) stack_ + DEFAULT_STACK_SIZE - 1);
            context_.x2 -= context_.x2 % 16;
            pc_ = (uint64) &wrapper;
        }
    }

    static void wrapper();
    static void setRunning(_thread* t);
    void setState(ThreadState state);
    static void kdispatch();
    ThreadState getState();
    static void kyield(_thread* oldT, _thread* newT);
    int kstart();
    static _thread* kmakeDynamic(PF body, void* arg, void* stack);
    static _thread* kmakeDynamic(PF body, void* arg);
    static void* operator new(size_t sz) noexcept;
    static void operator delete(void* p) noexcept;
    static int kthread_exit();
    void set_a0(uint64 val);
    static int ksleep(time_t time);
    static time_t getTimeLeft();
    static void setTimeLeft(time_t val);
};


#endif //OS_PROJEKAT_DEF__THREAD_H
