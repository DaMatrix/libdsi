#ifndef LIBDSI_DSIBIOS_H
#define LIBDSI_DSIBIOS_H

#include <libdsi/dsitypes.h>

namespace libdsi::bios {
    void SoftReset(); //0x00
    void Halt(); //0x06
    void IntrWait(); //0x04
    void VBlankIntrWait(); //0x05

    /**
     * Signed Division, r0/r1.
     *
     * r0  signed 32bit Number
     * r1  signed 32bit Denom
     *
     * Return:
     *
     * r0  Number DIV Denom ;signed
     * r1  Number MOD Denom ;signed
     * r3  ABS (Number DIV Denom) ;unsigned
     *
     * For example, incoming -1234, 10 should return -123, -4, +123.
     * The function usually gets caught in an endless loop upon division by zero.
     * Note: The NDS9 and DSi9 additionally support hardware division, by math coprocessor, accessed via I/O Ports, however, the SWI function is
     * a raw software division.
     */
    extern "C" i32 div(i32 numerator, i32 denominator);

    /**
     * Calculate square root.
     *
     * r0   unsigned 32bit number
     *
     * Return:
     *
     * r0   unsigned 16bit number
     *
     * The result is an integer value, so Sqrt(2) would return 1, to avoid this inaccuracy, shift left incoming number by 2*N as much as possible
     * (the result is then shifted left by 1*N). Ie. Sqrt(2 shl 30) would return 1.41421 shl 15.
     *
     * Note: The NDS9 and DSi9 additionally support hardware square root calculation, by math coprocessor, accessed via I/O Ports, however, the
     * SWI function is a raw software calculation.
     */
    u16 sqrt(u32 number);

    void CpuSet(); //0x0B
    void CpuFastSet(); //0x0C

    #ifdef ARM7
    void Sleep(); //0x07
    #endif
}

#endif //LIBDSI_DSIBIOS_H
