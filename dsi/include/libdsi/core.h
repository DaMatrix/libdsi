#ifndef LIBDSI_CORE_H
#define LIBDSI_CORE_H

#include <libdsi/types.h>
#include <libdsi/version.h>

namespace dsi {
    namespace memory {
        /**
         * Copies the specified memory region to the given destination.
         *
         * Source and destination addresses MUST be word-aligned, otherwise behavior is undefined!
         *
         * @param src  the base address of the source data
         * @param dst  the base address of the destination data
         * @param size the number of bytes to copy. If not word-aligned, will be rounded up!
         */
        extern "C" void fastCopy(const void* src, void* dst, u32 size);

        /**
         * Clears (zeroes out) the specified memory region.
         *
         * Destination address MUST be word-aligned, otherwise behavior is undefined!
         *
         * @param dst  the base address to start clearing from
         * @param size the number of bytes to clear. If not word-aligned, will be rounded up!
         */
        extern "C" void fastClear(void* dst, u32 size);
    }
}

#endif //LIBDSI_CORE_H
