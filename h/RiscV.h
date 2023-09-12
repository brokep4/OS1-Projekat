//
// Created by os on 5/23/22.
//

#ifndef OS_PROJEKAT_DEF_RISCV_H
#define OS_PROJEKAT_DEF_RISCV_H

#include "../lib/hw.h"

class RiscV
{
public:
    static inline uint64 get_scause()
    {
        uint64 scause;
        asm volatile("csrr %0, scause" : "=r"(scause));
        return scause;
    }

    static inline uint64 get_sepc()
    {
        uint64 sepc;
        asm volatile("csrr %0, sepc" : "=r"(sepc));
        return sepc;
    }

    static inline uint64 get_sscratch()
    {
        uint64 sscratch;
        asm volatile("csrr %0, sscratch" : "=r"(sscratch));
        return sscratch;
    }

    static inline uint64 get_sstatus()
    {
        uint64 sstatus;
        asm volatile("csrr %0, sstatus" : "=r"(sstatus));
        return sstatus;
    }

    static inline uint64 get_sip()
    {
        uint64 sip;
        asm volatile("csrr %0, sip" : "=r"(sip));
        return sip;
    }

    static inline uint64 get_a0()
    {
        uint64 x;
        asm volatile("mv %0, a0" : "=r" (x));
        return x;
    }

    static inline uint64 get_a1()
    {
        uint64 x;
        asm volatile("mv %0, a1" : "=r" (x));
        return x;
    }

    static inline uint64 get_a2()
    {
        uint64 x;
        asm volatile("mv %0, a2" : "=r" (x));
        return x;
    }

    static inline uint64 get_a3()
    {
        uint64 x;
        asm volatile("mv %0, a3" : "=r" (x));
        return x;
    }

    static inline void clear_SSIP_sip()
    {
        asm volatile("csrc sip, 0x02");
    }


    static inline void set_sepc(uint64 sepc)
    {
        asm volatile("csrw sepc, %0" : : "r"(sepc));
    }

    static inline void set_sscratch(uint64 sscratch)
    {
        asm volatile("csrw sscratch, %0" : : "r"(sscratch));
    }

    static inline void set_sstatus(uint64 sstatus)
    {
        asm volatile("csrw sstatus, %0" : : "r"(sstatus));
    }

    static inline void set_stvec(uint64 stvec)
    {
        asm volatile("csrw stvec, %0" : : "r"(stvec));
    }

    static inline void set_sip(uint64 sip)
    {
        asm volatile("csrw sip, %0" : : "r"(sip));
    }

    static inline void set_a0(uint64 x)
    {
        asm volatile("mv a0, %0" : : "r" (x));
    }

    static inline void set_a1(uint64 x)
    {
        asm volatile("mv a1, %0" : : "r" (x));
    }

    static inline void set_a2(uint64 x)
    {
        asm volatile("mv a2, %0" : : "r" (x));
    }

    static inline void set_a3(uint64 x)
    {
        asm volatile("mv a3, %0" : : "r" (x));
    }

    static inline void enable_interrupts()
    {
        asm volatile("csrs sstatus, 0x02");
    }

    static inline void change_to_user_mode()
    {
        //asm volatile("csrc sstatus, 256");
        set_sstatus(get_sstatus() & ((~((uint64)0)) ^ 256));
    }

    static inline void change_to_kernel_mode()
    {
        //asm volatile("csrs sstatus, 256");
        set_sstatus(get_sstatus() | 256);
    }
    /*
    static inline void disable_interrupts()
    {
        asm volatile("csrc sstatus, 0x02");
    }
    */
    static inline void writeToKernelStack(uint64 offsetFromTop, uint64 what)
    {
        uint64 kernelSP = RiscV::get_sscratch()-256;
        uint64* location = (uint64*)(kernelSP + offsetFromTop);
        *location = what;
    }

    static void changeMode(uint64 newMode)
    {
        RiscV::set_a1(newMode);
        RiscV::set_a0(0x99);
        asm volatile("ecall");
    }

};
#endif //OS_PROJEKAT_DEF_RISCV_H
