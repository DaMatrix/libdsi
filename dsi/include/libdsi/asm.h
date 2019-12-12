#ifndef LIBDSI_ASM_H
#define LIBDSI_ASM_H

#if !__ASSEMBLER__
#error "asm.h may only be used in assembly files!"
#endif	// !__ASSEMBLER__

    .syntax unified

#if defined(ARM9)
    .arch armv5te
    .cpu arm946e-s
#elif defined(ARM7)
    .arch armv4t
    .cpu arm7tdmi
#else
#error "Neither ARM7 nor ARM9 is defined!"
#endif


.macro ASM_FUNC name
    .text
    .global \name
    .type \name, %function
\name:
.endm

#endif // LIBDSI_ASM_H
