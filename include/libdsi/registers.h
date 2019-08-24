#ifndef LIBDSI_REGISTERS_H
#define LIBDSI_REGISTERS_H

#include <libdsi/dsitypes.h>

/**
 * Provides constant references to the various registers used for controlling the various features of the NDS hardware.
 */
namespace libdsi::reg {
    #define REG_R(ADDRESS, TYPE, NAME) constexpr TYPE NAME() { return *((TYPE* ) (ADDRESS)); }
    #define REG_W(ADDRESS, TYPE, NAME) constexpr TYPE NAME(TYPE value) { return *((TYPE* ) (ADDRESS)) = value; }
    #define REG_RW(ADDRESS, TYPE, NAME) REG_R(ADDRESS, TYPE, NAME) REG_W(ADDRESS, TYPE, NAME)

    #ifdef ARM9
    REG_RW(0x4000000, u32, DISPCNT)
    #endif

    REG_R(0x4000130, u16, KEYINPUT)

    #undef REG_RW
    #undef REG_W
    #undef REG_R
}

#endif //LIBDSI_REGISTERS_H
