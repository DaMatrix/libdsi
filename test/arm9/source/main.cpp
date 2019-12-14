#include "main.h"

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>

using namespace dsi;

extern "C" void doMain(void* buf);

int main() {
    consoleDemoInit();
    iprintf("%08x %08x\n", memory::fastCopy, memory::fastClear);

    //reg::BG3CNT_A(((1 << 14) | (1 << 7) | (1 << 2) | (4 << 16)));
    //reg::DISPCNT_A(reg::DISPCNT_A() | (1 << (8 + 3)));

    powerOn(POWER_ALL_2D);
    videoSetMode(MODE_3_2D);

    //auto buf = (u16*) 0x06000000;
    auto buf = bgGetGfxPtr(bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0));
    //auto buf = DISPLAY_TOP;

    if (true) doMain(buf);

    u32 color = 1;
    while (true)    {
        color ^= color << 13;
        color ^= color >> 17;
        color ^= color << 5;

        dmaFillHalfWords((color & 0x7FFF) | 0x8000, buf, SCREEN_WIDTH * sizeof(u16));
        memory::fastCopy(buf, buf + SCREEN_WIDTH, SCREEN_WIDTH * sizeof(u16));
        bios::vBlankIntrWait();
    }

    /*memory::fastCopy(nullptr, nullptr, 32);
    while (true) bios::vBlankIntrWait();*/
}
