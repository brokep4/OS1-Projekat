//
// Created by os on 5/23/22.
//
#include "../h/supervisorTrapHandler.h"
#include "../h/IOClass.h"

uint64 Timer::ticks=0;
Timer::WaitingThread* Timer::head= nullptr;

void Timer::kadd(_thread *thread, time_t time)
{
    if(time==0)return;
    WaitingThread* cur;
    WaitingThread* prev= nullptr;
    time_t curTime=0;
    for(cur = head; cur != nullptr; prev=cur,cur = cur->next)
    {
        curTime += cur->timeFromPrev;
        if(curTime >= time)break;
    }
    WaitingThread* newWaitingThread = new WaitingThread();
    newWaitingThread->thread = thread;
    if(!prev)head=newWaitingThread;
    else prev->next=newWaitingThread;
    newWaitingThread->next=cur;
    if(cur)curTime-=cur->timeFromPrev;
    newWaitingThread->timeFromPrev = time - curTime;
    if(cur)cur->timeFromPrev -= newWaitingThread->timeFromPrev;
}

void Timer::kcheck()
{
    //this should be called only as check, no decrements will be performed.
    while(head)
    {
        if(head->timeFromPrev > 0)break;
        head->thread->setState(_thread::ThreadState::RUNNING);
        Scheduler::put(head->thread);
        WaitingThread* str = head;
        str->thread->set_a0(0);
        head=head->next;
        delete str;
    }
}

void Timer::kDecHead()
{
    if(head && head->timeFromPrev != 0)head->timeFromPrev--;
}

void *Timer::WaitingThread::operator new(size_t sz) noexcept
{
    void* p = MemoryAllocator::kmem_alloc(sz);
    /*
    uint64 freeMem = MemoryAllocator::count_free_memory();
    if(freeMem < sz && p != nullptr)
    {
        printString2("FATAL ERROR IN NEW OPERATOR FOR _THREAD! MEMORY ALLOCATOR RETURNED SPACE IN MEMORY, BUT IT SHOULD BE NO SPACE LEFT!\n");
        return nullptr;
    }
     */
    return p;
}

void Timer::WaitingThread::operator delete(void *p)
noexcept {
    MemoryAllocator::kmem_free(p);
}

void handle_console()
{
    uint8* pStatusReg = (uint8*)CONSOLE_STATUS;
    uint8* pOutData = (uint8*)CONSOLE_TX_DATA;
    uint8* pInData = (uint8*)CONSOLE_RX_DATA;
    //int i = 3;

    while( ((*pStatusReg & CONSOLE_TX_STATUS_BIT) && IOClass::kOutputBufferSize() > 0 && IOClass::kInputBufferSize() == 0) )
    {
        if((*pStatusReg & CONSOLE_TX_STATUS_BIT) && IOClass::kOutputBufferSize() > 0)
        {
            *pOutData = IOClass::kPopOutputBuffer();
        }
    }

    while( ((*pStatusReg & CONSOLE_RX_STATUS_BIT) && IOClass::kInputBufferSize() < 500) )
    {
        if((*pStatusReg & CONSOLE_RX_STATUS_BIT) && IOClass::kInputBufferSize() < 500)
        {
            IOClass::kAddToInputBuffer((char)*pInData);
            IOClass::inputBufferSem->ksignal();
        }
        //i--;
    }
}


extern "C" void supervisorTrapHandler()
{
    volatile uint64 scause = RiscV::get_scause();
    volatile uint64 syscalla0 = RiscV::get_a0();
    volatile uint64 syscalla1 = RiscV::get_a1();
    volatile uint64 syscalla2 = RiscV::get_a2();
    volatile uint64 syscalla3 = RiscV::get_a3();
    if(scause == (1UL << 63 | 0x01))
    {
        //timer
        Timer::ticks++;
        RiscV::clear_SSIP_sip();
        Timer::kDecHead();//decrease sleep time
        Timer::kcheck();//check if some threads should be awakened
        if(_thread::getTimeLeft() == 0 || _thread::getTimeLeft() == 1) {
            _thread::kdispatch();
            _thread::setTimeLeft(DEFAULT_TIME_SLICE);
        }
        else _thread::setTimeLeft(_thread::getTimeLeft()-1);
    }
    else if(scause == 8 || scause == 9)
    {
        //software interrupt
        RiscV::clear_SSIP_sip();
        RiscV::set_sepc(RiscV::get_sepc()+4);

        if(syscalla0 == 0x01)
        {
            //void* mem_alloc(size_t size)


            //char s[]="mem_alloc\n";
            //print(s);
            //printDec(syscalla1);

            //a0 is x10
            uint64 ret = (uint64)MemoryAllocator::kmem_alloc((size_t)syscalla1);//__mem_alloc((size_t)syscalla1);
            RiscV::writeToKernelStack(80,ret);
        }
        if(syscalla0 == 0x02)
        {
            //int mem_free(void* p)


            //char s[]="mem_free\n";
            //print(s);
            //printDec(syscalla1);


            //a0 is x10
            uint64 ret = (uint64)MemoryAllocator::kmem_free((void*)syscalla1);//__mem_free((void*)syscalla1);
            RiscV::writeToKernelStack(80,ret);
        }
        if(syscalla0 == 0x11)
        {
            //int thread_create(thread_t* handle, void(*start_routine)(void*),void* arg);
            //char s[]="thread_create\n";
            //printDec(syscalla1);
            //printDec(syscalla2);
            //printDec(syscalla3);
            //printString2(s);

            /*
            _thread* t = _thread::kmakeDynamic((PF)syscalla2,(void*)syscalla3);
            thread_t* handle = (thread_t*)syscalla1;
            if(handle != nullptr)*handle = t;
            if(t != nullptr)
            {
                t->kstart();
                RiscV::writeToKernelStack(80, 0);
            }
            else
            {
                RiscV::writeToKernelStack(80,1);
            }
             */

            /*
            void* p = MemoryAllocator::kmem_alloc(6000);
            if(p == nullptr)
            {
                printString2("\nHEREIAM\n");
                RiscV::writeToKernelStack(80,1);
                return;
            }
            MemoryAllocator::kmem_free(p);
             */



            _thread* t = new _thread((PF)syscalla2,(void*)syscalla3);
            thread_t* handle = (thread_t*)syscalla1;
            if(handle != nullptr)*handle = t;
            if(t != nullptr)
            {
                uint64 ret = t->kstart();
                RiscV::writeToKernelStack(80, ret);
            }
            else
            {
                //printString2("\nHEREIAM\n");
                RiscV::writeToKernelStack(80,-1);
            }
        }
        if(syscalla0 == 0x12)
        {
            //printString2("thread_exit\n");
            uint64 ret = _thread::kthread_exit();
            RiscV::writeToKernelStack(80,ret);
        }

        if(syscalla0 == 0x13)
        {
            //char s[]="thread_dispatch\n";
            //printString2(s);
            _thread::kdispatch();
        }

        if(syscalla0 == 0x14)
        {
            _thread* t = new _thread((PF)syscalla2,(void*)syscalla3);
            thread_t* handle = (thread_t*)syscalla1;
            if(handle != nullptr)*handle = t;
            if(t != nullptr)
            {
                //uint64 ret = t->kstart();
                RiscV::writeToKernelStack(80, 0);
            }
            else
            {
                //printString2("\nHEREIAM\n");
                RiscV::writeToKernelStack(80,-1);
            }
        }

        if(syscalla0 == 0x15)
        {
            thread_t handle = (thread_t)syscalla1;
            if(handle == nullptr)
            {
                RiscV::writeToKernelStack(80,-1);
            }
            else
            {
                RiscV::writeToKernelStack(80,handle->kstart());
            }
        }

        if(syscalla0 == 0x21)
        {
            //int sem_open(sem_t* handle, uint64 val)
            sem_t* handle = (sem_t*)syscalla1;
            uint64 val = syscalla2;
            sem_t sem = new _sem(val);
            if(handle!=nullptr)*handle=sem;
            if(sem != nullptr)
            {
                RiscV::writeToKernelStack(80,0);
            }
            else RiscV::writeToKernelStack(80,-1);
        }
        if(syscalla0 == 0x22)
        {
            sem_t id = (sem_t)syscalla1;
            delete id;
            RiscV::writeToKernelStack(80,0);
        }
        if(syscalla0 == 0x23)
        {
            //int sem_wait (sem_t id)
            sem_t id = (sem_t)syscalla1;
            id->kwait();
            //RiscV::writeToKernelStack(80,0);
            //nothing should be written here actually because it will be written when unblocked or immediately if not even blocked
            //actually we can write to stack here, because if we are here, then our thread wasn't blocked by wait,
            //otherwise if it was blocked, it will later go back to sys call wait, and unblocker thread will write to a0
            RiscV::writeToKernelStack(80,0);
        }
        if(syscalla0 == 0x24)
        {
            //int sem_signal (sem_t id)
            sem_t id = (sem_t)syscalla1;
            id->ksignal();
            RiscV::writeToKernelStack(80,0);
        }
        if(syscalla0 == 0x31)
        {
            time_t time = syscalla1;
            _thread::ksleep(time);
        }
        if(syscalla0 == 0x41)
        {
            char c = (char)syscalla1;
            IOClass::kAddToOutputBuffer(c);
            handle_console();
        }
        if(syscalla0 == 0x42)
        {
            handle_console();
            char ret = -1;
            if(IOClass::kInputBufferSize() != 0)ret = IOClass::kPopInputBuffer();
            RiscV::writeToKernelStack(80,(uint64)ret);
        }
        if(syscalla0 == 0x99)
        {
            //mode change
            if(syscalla1 == 0)
            {
                RiscV::change_to_user_mode();
            }
            else RiscV::change_to_kernel_mode();
        }
    }
    else if(scause == (1UL << 63 | 0x09))
    {
        //console_handler();
        if(plic_claim() == CONSOLE_IRQ)
        {
            handle_console();
            plic_complete(CONSOLE_IRQ);
        }
    }
    else
    {
        char s[]="UNKNOWN REASON\n";
        printString2(s);
        printDec(scause);
        printDec(RiscV::get_sepc());
        printDec(Scheduler::size());
        printDec(MemoryAllocator::count_free_memory());
    }

}

void init_supervisorTrapHandler()
{
    char* stack = (char*)MemoryAllocator::kmem_alloc(4096);//__mem_alloc(4096);
    stack += 4095;
    stack -= ((uint64)stack) % 16;
    RiscV::set_sscratch((uint64)stack);
    RiscV::set_stvec((uint64)&supervisorTrap);
    //printDec((uint64)stack);
}
