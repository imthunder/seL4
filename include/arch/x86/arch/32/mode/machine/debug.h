/*
 * Copyright 2016, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(DATA61_GPL)
 */

#ifndef __ARCH_MODE_MACHINE_DEBUG_H_
#define __ARCH_MODE_MACHINE_DEBUG_H_

#include <assert.h>

#ifdef CONFIG_HARDWARE_DEBUG_API

#define X86_DEBUG_BP_N_REGS                 (4)

static inline word_t
readDr6Reg(void)
{
    word_t ret;

    asm volatile(
        "movl %%dr6, %0 \n\t"
        : "=r" (ret));
    return ret;
}

static inline void
writeDr6Reg(word_t val)
{
    asm volatile(
        "movl %0, %%dr6 \n\t"
        :
        : "r" (val));
}

static inline word_t
readDr7Reg(void)
{
    word_t ret;

    asm volatile(
        "movl %%dr7, %0 \n\t"
        : "=r" (ret));
    return ret;
}

static inline void
writeDr7Reg(word_t val)
{
    asm volatile(
        "movl %0, %%dr7 \n\t"
        :
        : "r" (val));
}

static inline word_t
readDrReg(uint8_t reg)
{
    word_t ret;

    assert(reg < X86_DEBUG_BP_N_REGS);
    switch (reg) {
    case 0:
        asm volatile("movl %%dr0, %0 \n\t" : "=r" (ret));
        break;
    case 1:
        asm volatile("movl %%dr1, %0 \n\t" : "=r" (ret));
        break;
    case 2:
        asm volatile("movl %%dr2, %0 \n\t" : "=r" (ret));
        break;
    default:
        asm volatile("movl %%dr3, %0 \n\t" : "=r" (ret));
        break;
    }
    return ret;
}

static inline void
writeDrReg(uint8_t reg, word_t val)
{
    assert(reg < X86_DEBUG_BP_N_REGS);
    switch (reg) {
    case 0:
        asm volatile("movl %0, %%dr0 \n\t" :: "r" (val));
        break;
    case 1:
        asm volatile("movl %0, %%dr1 \n\t" :: "r" (val));
        break;
    case 2:
        asm volatile("movl %0, %%dr2 \n\t" :: "r" (val));
        break;
    default:
        asm volatile("movl %0, %%dr3 \n\t" :: "r" (val));
        break;
    }
}

/** Restore debug register context from a block of memory.
 *@param source The memory block from which to load the register values.
 */
static inline void
loadBreakpointState(tcb_t *source)
{
    /* Order does matter when restoring the registers: we want to restore the
     * breakpoint control register (DR7) last since it is what "activates" the
     * effects of the configuration described by the other registers.
     */
    asm volatile (
        "movl %0, %%edx \n\t"
        "movl (%%edx), %%ecx \n\t"
        "movl %%ecx, %%dr0 \n\t"
        "addl $4, %%edx \n\t"
        "movl (%%edx), %%ecx \n\t"
        "movl %%ecx, %%dr1 \n\t"
        "addl $4, %%edx \n\t"
        "movl (%%edx), %%ecx \n\t"
        "movl %%ecx, %%dr2 \n\t"
        "addl $4, %%edx \n\t"
        "movl (%%edx), %%ecx \n\t"
        "movl %%ecx, %%dr3 \n\t"
        "addl $4, %%edx \n\t"
        "movl (%%edx), %%ecx \n\t"
        "movl %%ecx, %%dr6 \n\t"
        "addl $4, %%edx \n\t"
        "movl (%%edx), %%ecx \n\t"
        "movl %%ecx, %%dr7 \n\t"
        :
        : "r" (source->tcbArch.tcbContext.breakpointState.dr)
        : "edx", "ecx");
}

#endif /* CONFIG_HARDWARE_DEBUG_API */
#endif /* __ARCH_MODE_MACHINE_DEBUG_H_ */
