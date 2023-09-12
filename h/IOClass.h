//
// Created by os on 5/30/22.
//

#ifndef OS_PROJEKAT_DEF_IOCLASS_H
#define OS_PROJEKAT_DEF_IOCLASS_H

#include "Queue.h"
#include "_sem.h"

class IOClass
{
public:
    static _sem* inputBufferSem;
    static Queue<char>* inputBuffer;
    static Queue<char>* outputBuffer;
    static void init();
    static void kAddToInputBuffer(char c);
    static void kAddToOutputBuffer(char c);
    static int kInputBufferSize();
    static int kOutputBufferSize();
    static char kPopInputBuffer();
    static char kPopOutputBuffer();
};


#endif //OS_PROJEKAT_DEF_IOCLASS_H
