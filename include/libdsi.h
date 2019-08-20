#ifndef DSIWIFI_LIBDSI_H
#define DSIWIFI_LIBDSI_H

#include <cstdint>

/**
 * libnds is too complicated so let's simplify things a bit
 */

namespace libdsi {
    typedef std::int8_t   i8;
    typedef std::int8_t   s8;
    typedef std::uint8_t  u8;
    typedef std::int16_t  i16;
    typedef std::int16_t  s16;
    typedef std::uint16_t u16;
    typedef std::int32_t  i32;
    typedef std::int32_t  s32;
    typedef std::uint32_t u32;
    typedef std::int64_t  i64;
    typedef std::int64_t  s64;
    typedef std::uint64_t u64;

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
    /**
     * Prepares for setting up WRAM regions on the ARM9 by allowing ARM9 write access to all WRAM slot allocation registers.
     /
    inline void wram_prepare()  {
        *WRAM_MBK9 = 0x00FFFF0F;
    }
#endif*/
}

#endif //DSIWIFI_LIBDSI_H
