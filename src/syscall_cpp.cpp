//
// Created by os on 5/30/22.
//
#include "../h/syscall_cpp.hpp"
#include "../h/_thread.h"

Thread::Thread(void (*body)(void *), void *arg)
{
    //thread_create(&myHandle,body,arg);
    //myHandle = new _thread(body,arg);
    thread_make(&myHandle,body,arg);
}

Thread::~Thread()
{
    delete myHandle;
}

int Thread::start()
{
    return thread_start(myHandle);//thread_create(&myHandle,wrapper,this);
}

void Thread::dispatch()
{
    thread_dispatch();
}

int Thread::sleep(time_t t)
{
    return time_sleep(t);
}

Thread::Thread() {
    thread_make(&myHandle,wrapper,this);
}

void Thread::wrapper(void *t)
{
    Thread* tr = (Thread*)t;
    tr->run();
}

Semaphore::Semaphore(unsigned int init)
{
    sem_open(&myHandle, init);
}
Semaphore::~Semaphore()
{
    sem_close(myHandle);
}

int Semaphore::wait()
{
    return sem_wait(myHandle);
}

int Semaphore::signal()
{
    return sem_signal(myHandle);
}

struct PeriodicData
{
    PeriodicThread* thread;
    time_t time;
    PeriodicData(PeriodicThread* t, time_t tm) : thread(t), time(tm) {}
};

PeriodicThread::PeriodicThread(time_t period)  : Thread(&PeriodicThread::wrapper2,new PeriodicData(this,period))
{}

void PeriodicThread::wrapper2(void *arg)
{
    PeriodicData* data = (PeriodicData*)arg;
    PeriodicThread* thread = data->thread;
    time_t time = data->time;
    while(1)
    {
        thread->periodicActivation();
        Thread::sleep(time);
    }
}

void Console::putc(char c)
{
    ::putc(c);
}

char Console::getc()
{
    return ::getc();
}