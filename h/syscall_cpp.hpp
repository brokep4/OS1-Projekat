//
// Created by os on 5/30/22.
//

#ifndef OS_PROJEKAT_DEF_SYSCALL_CPP_H
#define OS_PROJEKAT_DEF_SYSCALL_CPP_H
#include "../h/syscall_c.h"

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
protected:
    static void wrapper(void* t);
};
class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};
class PeriodicThread : public Thread {
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
    static void wrapper2(void* arg);
};
class Console {
public:
    static char getc ();
    static void putc (char);
};

#endif //OS_PROJEKAT_DEF_SYSCALL_CPP_H
