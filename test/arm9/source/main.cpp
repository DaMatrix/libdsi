#include "main.h"

#include <nds.h>
#include <stdio.h>
#include <stdlib.h>

using namespace dsi;

extern "C" void doMain(void* buf);

int main() {
    consoleDemoInit();
    iprintf("%08x %08x\n", mem::fastCopy, mem::fastClear);
    iprintf(isDSiMode() ? "DSi mode!\n" : "NTR mode...\n");

    powerOn(POWER_ALL_2D);
    videoSetMode(MODE_3_2D);

    //auto buf = (u16*) 0x06000000;
    auto buf = bgGetGfxPtr(bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0));
    //auto buf = DISPLAY_TOP;

    setBrightness(1, -15);

    if (true) doMain(buf);
}
