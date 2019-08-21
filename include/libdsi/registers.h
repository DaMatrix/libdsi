#ifndef LIBDSI_REGISTERS_H
#define LIBDSI_REGISTERS_H

#include <libdsi/dsitypes.h>

/**
 * Provides constant references to the various registers used for controlling the various features of the NDS hardware.
 */
namespace libdsi::reg {
    static constexpr u32* const KEYINPUT = (u32*) 0x4000130;
}

#endif //LIBDSI_REGISTERS_H
