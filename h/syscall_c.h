//
// Created by os on 5/23/22.
//

#ifndef OS_PROJEKAT_DEF_SYSCALL_C_H
#define OS_PROJEKAT_DEF_SYSCALL_C_H
#include "../lib/hw.h"
#include "../h/RiscV.h"
#include "../h/_thread.h"

class _sem;

void* mem_alloc(size_t size);
int mem_free(void* p);

typedef _thread* thread_t;

int thread_create(thread_t* handle, void(*start_routine)(void*),void* arg);
int thread_exit();
void thread_dispatch();
int thread_make(thread_t* handle, void(*start_routine)(void*),void* arg);
int thread_start(thread_t handle);

typedef _sem* sem_t;
int sem_open(sem_t* handle, uint64 val);
int sem_close(sem_t id);
int sem_wait(sem_t id);
int sem_signal(sem_t id);


typedef unsigned long time_t;
int time_sleep(time_t time);

void putc(char c);
char getc();

#endif //OS_PROJEKAT_DEF_SYSCALL_C_H
