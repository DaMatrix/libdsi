#include <dsi.h>

bool __dsimode; //set in crt0

namespace dsi {
    extern "C" void initSystem() {
        reg::IME = 0;

        for (u32 i = 0; i < 4; i++) { dma::channel(0)->erase(); }

        //clear sound registers
        mem::fastClear((void*) 0x04000400, 0x100);
    }
}
