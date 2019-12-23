#ifndef LIBDSI_LIBDSI9_H
#define LIBDSI_LIBDSI9_H

#ifndef ARM9
#error Must be used for ARM9!
#endif

#include <libdsi/bios.h>
#include <libdsi/core9.h>
#include <libdsi/dma.h>
#include <libdsi/fifo.h>
#include <libdsi/intr.h>
#include <libdsi/registers.h>

#include <libdsi/arm9/video.h>
#include <libdsi/arm9/wifi9.h>

namespace dsi {
    namespace _internal {
        extern "C" u32 __dtcm_start;
        extern "C" u32 __dtcm_top;
        extern "C" u32 __dtcm_size;
    }

    /**
     * @return the bottom address of DTCM
     */
    constexpr u32 dtcmStart() { return (u32) &_internal::__dtcm_start; }

    /**
     * @return the top address of DTCM
     */
    constexpr u32 dtcmEnd() { return (u32) &_internal::__dtcm_top; }

    /**
     * @return the size of DTCM
     */
    constexpr u32 dtcmSize() { return (u32) &_internal::__dtcm_size; }
}

#endif //LIBDSI_LIBDSI9_H
