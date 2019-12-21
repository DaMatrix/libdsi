#include "main.h"

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>

using namespace dsi;

extern "C" void doMain(void* buf);

int main() {
    crashSystem("Debug crash!");

    consoleDemoInit();
    iprintf("%08x %08x\n", mem::fastCopy, mem::fastClear);
    iprintf(isDSiMode() ? "DSi mode!\n" : "NTR mode...\n");

    sys::powerOn(sys::POWER_2D_A | sys::POWER_2D_B);
    iprintf("%08x\n", reg::DISPCNT_A);
    //videoSetMode(MODE_3_2D);
    reg::DISPCNT_A = MODE_3_2D;
    iprintf("%08x\n", REG_DISPCNT);

    //auto buf = (u16*) 0x06000000;
    auto buf = bgGetGfxPtr(bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0));
    //auto buf = DISPLAY_TOP;

    video::setBrightness(video::DISPLAY_A, -15);

    if (true) doMain(buf);

    crashSystem("Debug crash!");
}
