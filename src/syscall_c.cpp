//
// Created by os on 5/23/22.
//
#include "../h/syscall_c.h"

void* mem_alloc(size_t size)
{
    RiscV::set_a1((uint64)size);
    RiscV::set_a0(0x01);
    asm volatile("ecall");
    return (void*)RiscV::get_a0();
}

int mem_free(void* p)
{
    RiscV::set_a1((uint64)p);
    RiscV::set_a0(0x02);
    asm volatile("ecall");
    return (int)RiscV::get_a0();
}

int thread_create(thread_t* handle, void(*start_routine)(void*),void* arg)
{
    if(start_routine == nullptr)return -1;
    RiscV::set_a3((uint64)arg);
    RiscV::set_a2((uint64)start_routine);
    RiscV::set_a1((uint64)handle);
    RiscV::set_a0(0x11);
    asm volatile("ecall");
    return (int)RiscV::get_a0();
}

int thread_exit()
{
    RiscV::set_a0(0x12);
    asm volatile("ecall");
    return (int )RiscV::get_a0();
}


void thread_dispatch()
{
    RiscV::set_a0(0x13);
    asm volatile("ecall");
}

int thread_make(thread_t* handle, void(*start_routine)(void*),void* arg)
{
    if(start_routine == nullptr)return -1;
    RiscV::set_a3((uint64)arg);
    RiscV::set_a2((uint64)start_routine);
    RiscV::set_a1((uint64)handle);
    RiscV::set_a0(0x14);
    asm volatile("ecall");
    return (int)RiscV::get_a0();
}

int thread_start(thread_t handle)
{
    if(handle== nullptr)return -1;
    RiscV::set_a1((uint64)handle);
    RiscV::set_a0(0x15);
    asm volatile("ecall");
    return (int)RiscV::get_a0();
}


int sem_open(sem_t* handle, uint64 val)
{
    if(handle == nullptr)return -1;
    RiscV::set_a2(val);
    RiscV::set_a1((uint64)handle);
    RiscV::set_a0(0x21);
    asm volatile("ecall");
    return (int)RiscV::get_a0();
}

int sem_close(sem_t id)
{
    if(id == nullptr)return -1;
    RiscV::set_a1((uint64)id);
    RiscV::set_a0(0x22);
    asm volatile("ecall");
    return (int)RiscV::get_a0();
}

int sem_wait(sem_t id)
{
    if(id == nullptr)return -1;
    RiscV::set_a1((uint64)id);
    RiscV::set_a0(0x23);
    asm volatile("ecall");
    return (int)RiscV::get_a0();
}

int sem_signal(sem_t id)
{
    if(id == nullptr)return -1;
    RiscV::set_a1((uint64)id);
    RiscV::set_a0(0x24);
    asm volatile("ecall");
    return (int)RiscV::get_a0();
}

int time_sleep(time_t time)
{
    if(time==0)return 0;
    RiscV::set_a1(time);
    RiscV::set_a0(0x31);
    asm volatile("ecall");
    return (int)RiscV::get_a0();
}

#include "../h/IOClass.h"

void putc(char c)
{
    if(IOClass::kOutputBufferSize() >= 500)return;
    RiscV::set_a1((uint64)c);
    RiscV::set_a0(0x41);
    asm volatile("ecall");
}

char getc()
{
    //while(IOClass::kInputBufferSize()==0);
    sem_wait(IOClass::inputBufferSem);
    RiscV::set_a0(0x42);
    asm volatile("ecall");
    volatile char c = RiscV::get_a0();
    return c;//return (char)RiscV::get_a0();
}