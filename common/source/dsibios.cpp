#include <libdsi/dsibios.h>

namespace libdsi::bios {
    __attribute__((naked)) i32 div(i32 numerator, i32 denominator) { asm ("swi 0x09\n\tbx lr"); }
    __attribute__((naked)) u16 sqrt(u32 number) { asm ("swi 0x0D\n\tbx lr"); }
}
