#include <libdsi/dsibios.h>

using namespace libdsi;

extern "C" i32 _asm_div(i32 numerator, i32 denominator);

namespace libdsi::bios {
   i32 div(i32 numerator, i32 denominator) { return _asm_div(numerator, denominator); }
}
