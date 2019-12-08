#ifndef LIBDSI_CORE_H
#define LIBDSI_CORE_H

#include <libdsi/types.h>
#include <libdsi/version.h>

namespace dsi {
    namespace memory {
        /**
         * Copies the specified memory region to the given destination in 32-byte chunks.
         *
         * Source and destination addresses MUST be word-aligned, otherwise behavior is undefined!
         *
         * @param src   the base address of the source data
         * @param dst   the base address of the destination data
         * @param bytes the number of chunks to copy
         */
        extern "C" void copyChunks32(const void* src, void* dst, u32 chunks);
    }
}

#endif //LIBDSI_CORE_H
