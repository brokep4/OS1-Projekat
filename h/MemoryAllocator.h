//
// Created by os on 5/23/22.
//

#ifndef OS_PROJEKAT_DEF_MEMORYALLOCATOR_H
#define OS_PROJEKAT_DEF_MEMORYALLOCATOR_H

#include "../lib/hw.h"

class MemoryAllocator
{
    struct MemBlock
    {
        MemBlock* next;
        size_t size;
    };
public:
    static MemBlock* head;
    static void init();
    static void* kmem_alloc(size_t size);
    static int kmem_free(void* p);
    static uint64 count_free_memory();
    static void kmemcpy(void* dst, void* src, size_t size);
    static bool tryToJoin(MemBlock* left);
};

#endif //OS_PROJEKAT_DEF_MEMORYALLOCATOR_H
