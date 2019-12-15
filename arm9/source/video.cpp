#include <libdsi/arm9/video.h>

namespace dsi::video {
    void resetVRAM() {
        reg::VRAMCNT_A = reg::A_Direct;
        reg::VRAMCNT_B = reg::B_Direct;
        reg::VRAMCNT_C = reg::C_Direct;
        reg::VRAMCNT_D = reg::D_Direct;
        reg::VRAMCNT_E = reg::E_Direct;
        reg::VRAMCNT_F = reg::F_Direct;
        reg::VRAMCNT_G = reg::G_Direct;
        reg::VRAMCNT_H = reg::H_Direct;
        reg::VRAMCNT_I = reg::I_Direct;

        mem::fastClear((void*) 0x05000000, 0x800); //clear palettes
        mem::fastClear((void*) 0x07000000, 0x800); //clear OAM
        mem::fastClear((void*) 0x06000000, 0xA4000); //clear VRAM

        reg::VRAMCNT_A = reg::A_Background0;
        reg::VRAMCNT_B = reg::B_Object0;
        reg::VRAMCNT_C = reg::C_BackgroundB;
        reg::VRAMCNT_D = reg::D_ObjectB;
        reg::VRAMCNT_E = reg::E_Disable;
        reg::VRAMCNT_F = reg::F_Disable;
        reg::VRAMCNT_G = reg::G_Disable;
        reg::VRAMCNT_H = reg::H_Disable;
        reg::VRAMCNT_I = reg::I_Disable;
    }
}
