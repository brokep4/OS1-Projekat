//
// Created by os on 5/23/22.
//
#include "../h/MemoryAllocator.h"

MemoryAllocator::MemBlock* MemoryAllocator::head=nullptr;

void MemoryAllocator::init()
{
    MemoryAllocator::head = (MemBlock*)HEAP_START_ADDR;
    MemoryAllocator::head->size = ((uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR) - sizeof(MemBlock);
    MemoryAllocator::head->next = 0;
}

void *MemoryAllocator::kmem_alloc(size_t size)
{
    if(size == 0)return nullptr;
    if(size % MEM_BLOCK_SIZE != 0)size += MEM_BLOCK_SIZE - size%MEM_BLOCK_SIZE;
    MemBlock* prev = nullptr;
    MemBlock* cur;
    for(cur = MemoryAllocator::head; cur != nullptr; prev=cur, cur = cur->next)
    {
        if(cur->size >= size)break;
    }
    if(cur == nullptr || cur->size < size)return nullptr;
    size_t sizeLeft = cur->size - size;
    if(sizeLeft >= sizeof(MemBlock))
    {
        sizeLeft -= sizeof(MemBlock);
        MemBlock* newB = (MemBlock*)((char*)cur + size + sizeof(MemBlock));
        newB->size = sizeLeft;
        if(prev==nullptr)head=newB;
        else prev->next=newB;
        newB->next=cur->next;
    }
    else
    {
        if(prev==nullptr)head=cur->next;
        else prev->next = cur->next;
    }
    cur->next=nullptr;
    cur->size = size;
    return (void*)((char*)cur + sizeof(MemBlock));
}

int MemoryAllocator::kmem_free(void *p)
{
    if(p==nullptr)return 0;
    MemBlock* memBlock = (MemBlock*)((char*)p - sizeof(MemBlock));
    if(memBlock->size % MEM_BLOCK_SIZE != 0 || memBlock->size == 0)return -1;
    if(memBlock->next != nullptr)return -1;
    MemBlock* cur;
    MemBlock* prev=nullptr;
    for(cur = head; cur != nullptr; prev=cur, cur=cur->next)
    {
        if(cur > memBlock)break;
    }
    if(prev==nullptr)head=memBlock;
    else prev->next=memBlock;
    memBlock->next = cur;
    //should add tryToJoin?
    while(tryToJoin(memBlock));
    if(prev)
        while (tryToJoin(prev));
    return 0;
}

uint64 MemoryAllocator::count_free_memory()
{
    uint64 s=0;
    //uint64 c=0;
    for(MemBlock* cur = head; cur != 0; cur=cur->next)
    {
        //s++;
        s += cur->size + sizeof(MemBlock);
    }
    return s;
}

void MemoryAllocator::kmemcpy(void *dst, void *src, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        *((char*)dst + i) = *((char*)src + i);
    }
}

bool MemoryAllocator::tryToJoin(MemBlock *left)
{
    if(left== nullptr)return false;
    MemBlock* right = left->next;
    if(right== nullptr)return false;
    if((char*)left + left->size + sizeof(MemBlock) != (char*)right)
    {
        return false;
    }
    MemBlock* rightright = right->next;
    left->size += right->size + sizeof(MemBlock);
    left->next = rightright;
    return true;

}