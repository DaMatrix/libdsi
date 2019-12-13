#include <dsi.h>

using namespace dsi;

extern "C" void initSystem(void) {
    //reset all DMA channels
    for (register u32 i = 0; i < 4; i++) {
        dma::channel(i)->stop();
    }

    //clear video display registers
    memory::fastClear((void*) 0x04000000, 0x56);
    memory::fastClear((void*) 0x04001008, 0x56);

    reg::DISPCNT_B(0);

    video::resetVRAM();

    reg::VRAMCNT_E(reg::E_Disable);
    reg::VRAMCNT_F(reg::F_Disable);
    reg::VRAMCNT_G(reg::G_Disable);
    reg::VRAMCNT_H(reg::H_Disable);
    reg::VRAMCNT_I(reg::I_Disable);
}
