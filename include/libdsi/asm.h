#ifndef LIBDSI_ASM_H
#define LIBDSI_ASM_H

#if !__ASSEMBLER__
#error "asm.h may only be used in assembly files!"
#endif	// !__ASSEMBLER__

    .syntax unified

#ifdef ARM9
    .arch armv5te
    .cpu arm946e-s
#elif defined(ARM7)
    .arch armv4t
    .cpu arm7tdmi
#else
#error "Neither ARM7 nor ARM9 is defined!"
#endif

.macro ASM_FUNC name
    .global \name
    .type \name, %function
\name:
.endm

.macro ASM_DATA name
    .global \name
    .type \name, %object
\name:
.endm

#define GET_CPSR(DST) mrs DST, cpsr
#define SET_CPSR(SRC) msr cpsr, SRC
#define GET_SPSR(DST) mrs DST, spsr
#define SET_SPSR(SRC) msr spsr, SRC

#endif // LIBDSI_ASM_H
