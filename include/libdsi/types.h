#ifndef LIBDSI_DSITYPES_H
#define LIBDSI_DSITYPES_H

#ifndef ARM7
#ifndef ARM9
#error Either ARM7 or ARM9 must be set!
#endif
#endif

#ifdef ARM7
#ifdef ARM9
#error ARM7 and ARM9 may not be set together!
#endif
#endif

#ifndef DSI
#define DSI
#endif

#include <cstdint>
#include <libdsi/version.h>

namespace dsi {
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

    typedef void (* Void)();

    template<typename T> constexpr T mask(T bits) { return (1 << bits) - 1; }
    template<typename T> constexpr T mask(T bits, T offset) { return ((1 << bits) - 1) << offset; }
    template<typename T> constexpr T bit(T offset) { return 1 << offset; }

    /**
     * An ARGB16 color.
     *
     * Technically should be called ABGR16, but it's the only color format used and argb is easier to type :P
     *
     * Bit Meaning
     * 0-4   Red channel (0-31)
     * 5-9   Green channel (0-31)
     * 10-14 Blue channel (0-31)
     * 15    Alpha channel (0: transparent, 1: opaque)
     */
    struct argb16 {
        const u16 color;

        argb16(u16 color): color(color) {}

        explicit argb16(u8 alpha, u8 red, u8 green, u8 blue): color((alpha << 15) | (blue << 10) | (green << 5) | red) {}

        inline u8 red() { return this->color & 0x1F; }
        inline u8 green() { return (this->color >> 5) & 0x1F; }
        inline u8 blue() { return (this->color >> 10) & 0x1F; }
        inline u8 alpha() { return this->color >> 15; }
        inline bool opaque() { return this->color >> 15; }

        inline operator u16() const { return this->color; }
    };
}

#endif //LIBDSI_DSITYPES_H
