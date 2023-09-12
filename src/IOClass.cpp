//
// Created by os on 5/30/22.
//
#include "../h/IOClass.h"

Queue<char>* IOClass::inputBuffer = nullptr;
Queue<char>* IOClass::outputBuffer = nullptr;
_sem* IOClass::inputBufferSem = nullptr;

void IOClass::init()
{
    inputBuffer = new Queue<char>(500);
    outputBuffer = new Queue<char>(500);
    if(inputBuffer == nullptr || outputBuffer == nullptr)
    {
        printString2("Error while IOClass buffers initialization!\n");
    }
    inputBufferSem = new _sem(0);
}

void IOClass::kAddToInputBuffer(char c)
{
    if(inputBuffer==nullptr)return;
    inputBuffer->add(c);
}

void IOClass::kAddToOutputBuffer(char c)
{
    if(outputBuffer==nullptr)return;
    outputBuffer->add(c);
}

int IOClass::kInputBufferSize()
{
    if(inputBuffer == nullptr)return 0;
    return inputBuffer->size();
}

int IOClass::kOutputBufferSize()
{
    if(outputBuffer == nullptr)return 0;
    return outputBuffer->size();
}

char IOClass::kPopInputBuffer()
{
    if(inputBuffer == nullptr)return char();
    return inputBuffer->pop();
}

char IOClass::kPopOutputBuffer()
{
    if(outputBuffer == nullptr)return char();
    return outputBuffer->pop();
}