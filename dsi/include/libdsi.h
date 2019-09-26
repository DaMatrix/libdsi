#ifndef LIBDSI_H
#define LIBDSI_H

#include <libdsi/core.h>

#ifdef ARM9
#include <libdsi/arm9/dsi9.h>
#else
#include <libdsi/arm7/dsi7.h>
#endif

/**
 * libnds is too complicated so let's simplify things a bit
 */
namespace dsi {
    /*constexpr u32* WRAM_MBK1 = (u32*) 0x04004040;
    constexpr u32* WRAM_MBK2 = (u32*) 0x04004044;
    constexpr u32* WRAM_MBK3 = (u32*) 0x04004048;
    constexpr u32* WRAM_MBK4 = (u32*) 0x0400404C;
    constexpr u32* WRAM_MBK5 = (u32*) 0x04004050;
    constexpr u32* WRAM_MBK6 = (u32*) 0x04004054;
    constexpr u32* WRAM_MBK7 = (u32*) 0x04004058;
    constexpr u32* WRAM_MBK8 = (u32*) 0x0400405C;
#ifdef ARM7
    constexpr u32* WRAM_MBK9 = (u32*) 0x04004060;
#endif

#ifdef ARM7
    /
     * Prepares for setting up WRAM regions on the ARM9 by allowing ARM9 write access to all WRAM slot allocation registers.
     /
    inline void wram_prepare()  {
        *WRAM_MBK9 = 0x00FFFF0F;
    }
#endif*/
}

#endif //LIBDSI_H
