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
}

#endif //LIBDSI_DSITYPES_H
