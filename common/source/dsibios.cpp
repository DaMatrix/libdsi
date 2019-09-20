#include <libdsi/dsibios.h>

extern "C" libdsi::i32 asm_div(libdsi::i32 numerator, libdsi::i32 denominator);
extern "C" libdsi::u16 asm_sqrt(libdsi::u32 value);

namespace libdsi::bios {
    //__attribute__((naked)) i32 div(i32 numerator, i32 denominator) { asm ("swi 0x09\n\tbx lr"); }
    //__attribute__((naked)) u16 sqrt(u32 number) { asm ("swi 0x0D\n\tbx lr"); }

    /*i32 div(i32 numerator, i32 denominator) {
        return asm_div(numerator, denominator);
    }*/

    /*__attribute__((naked,target("thumb"))) inline i32 div(i32 numerator, i32 denominator)    {
        //asm("swi 0x09");
        //asm("bx lr");
        return asm_div(numerator, denominator);
    }*/
}
