//
// Created by os on 5/23/22.
//

#include "../h/printUtils.h"
#include "../h/syscall_c.h"

void printString2(const char* s)
{
    while(*s != '\0')
    {
        putc(*s);
        s++;
    }
}

void print(const char* s)
{
    printString2(s);
}

void printDec(uint64 x)
{
    if(x==0)
    {
        putc('0');
        putc('\n');
        return;
    }
    char s[22];
    int cnt = 0;
    uint64 y = x;
    while(y > 0)cnt++,y/=10;
    s[cnt+1]='\0';
    s[cnt]='\n';
    while(x > 0)
    {
        s[--cnt] = '0' + x%10;
        x/=10;
    }
    printString2(s);
}