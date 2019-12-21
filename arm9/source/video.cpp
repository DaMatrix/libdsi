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

        mem::fastClear(&reg::BG_PALETTE_A, reg::SIZE_PALETTE);
        mem::fastClear(&reg::OAM_A, reg::SIZE_OAM);
        mem::fastClear(&reg::VRAM_A, reg::SIZE_VRAM);

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

    void swapDisplays() {
        reg::POWCNT1 ^= sys::POWER_SWAP_DISPLAYS;
    }

    void topDisplay(Display display)     {
        if (display == DISPLAY_A)   {
            reg::POWCNT1 |= sys::POWER_SWAP_DISPLAYS;
        } else if (display == DISPLAY_B)    {
            reg::POWCNT1 &= ~sys::POWER_SWAP_DISPLAYS;
        }
    }

    void resetBrightness(Display display) {
        if (display & DISPLAY_A)  {
            reg::MASTER_BRIGHT_A = 0;
        }
        if (display & DISPLAY_B)   {
            reg::MASTER_BRIGHT_B = 0;
        }
    }

    void setBrightness(Display display, i32 brightness)  {
        u32 value;
        if (brightness == 0)    {
            return resetBrightness(display);
        } else if (brightness < 0)  {
            value = (2 << 14) | (u32) (brightness < -16 ? 16 : -brightness);
        } else if (brightness > 0)  {
            value = (1 << 14) | (u32) (brightness > 16 ? 16 : brightness);
        }

        if (display & DISPLAY_A)  {
            reg::MASTER_BRIGHT_A = value;
        }
        if (display & DISPLAY_B)   {
            reg::MASTER_BRIGHT_B = value;
        }
    }

    void setBackgroundMode(Display display, BackgroundMode mode)    {
        if (display & DISPLAY_A)    {
            reg::DISPCNT_A = (reg::DISPCNT_A & ~mask(3)) | mode;
        }
        if (display & DISPLAY_B)    {
            reg::DISPCNT_B = (reg::DISPCNT_B & ~mask(3)) | mode;
        }
    }

    void setDisplayMode(Display display, DisplayMode mode)  {
        if (display & DISPLAY_A)    {
            reg::DISPCNT_A = (reg::DISPCNT_A & ~mask(2, 16)) | (mode << 16);
        }
        if (display & DISPLAY_B)    {
            reg::DISPCNT_B = (reg::DISPCNT_B & ~mask(2, 16)) | (mode << 16);
        }
    }

    void setBackdrop(Display display, argb16 color) {
        if (display & DISPLAY_A)    {
            reg::BG_PALETTE_A[0] = color;
        }
        if (display & DISPLAY_B)    {
            reg::BG_PALETTE_B[0] = color;
        }
    }
}
