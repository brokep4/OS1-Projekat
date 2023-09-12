//
// Created by os on 5/24/22.
//

#ifndef OS_PROJEKAT_DEF_QUEUE_H
#define OS_PROJEKAT_DEF_QUEUE_H

#include "../h/syscall_c.h"
#include "../h/_thread.h"

template<typename T>
class Queue
{
    int head_;
    int tail_;
    T* array_;
    int size_;
    int capacity_;
    bool isKernel;
public:
    Queue(int startingCapacity = 50,bool isKernel=true)
    {
        if(!isKernel)
        {
            array_ = (T *) mem_alloc(startingCapacity * sizeof(T));
        }
        else array_ = (T*) MemoryAllocator::kmem_alloc(startingCapacity*sizeof (T));
        head_=tail_=0;
        capacity_ = startingCapacity;
        size_=0;
        this->isKernel=isKernel;
    }
    void add(T data)
    {
        if(size_ == capacity_)
        {
            T* newArray;
            if(!isKernel)
            {
                newArray = (T*) mem_alloc((capacity_+50)*sizeof(T));
            }
            else
            {
                newArray = (T*) MemoryAllocator::kmem_alloc((capacity_+50)*sizeof(T));
            }

            if(newArray== nullptr)return;//no available size to shrink Queue
            for(int i = 0; i < size_; i++)
            {
                newArray[i] = array_[(i+head_)%capacity_];
            }
            head_=0;
            tail_=size_;
            if(!isKernel)
            {
                mem_free(array_);
            }
            else MemoryAllocator::kmem_free(array_);
            array_ = newArray;
            capacity_ += 50;
        }
        array_[tail_] = data;
        tail_ = (tail_+1)%capacity_;
        size_++;
    }
    T pop()
    {
        if(size_==0)return T();
        T ret = array_[head_];
        head_=(head_+1)%capacity_;
        size_--;
        return ret;
    }
    int size()
    {
        return size_;
    }

    ~Queue()
    {
        if(!isKernel)
            mem_free(array_);
        else
            MemoryAllocator::kmem_free(array_);
    }
};


#endif //OS_PROJEKAT_DEF_QUEUE_H
