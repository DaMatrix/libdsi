#include "main.h"

using namespace dsi;

void setBrightness( int screen, int level) {
//---------------------------------------------------------------------------------
    int mode = 1<<14;

    if ( level < 0){
        level = -level;
        mode = 2<<14;
    }

    if (level>16) level =16;

    if (screen & 1) reg::MASTER_BRIGHT_A(mode | level);
    if (screen & 2) reg::MASTER_BRIGHT_B(mode | level);
}

int main() {
    setBrightness(3, -16);

    reg::POWCNT1(1);

    reg::DISPCNT_A(0);
    reg::DISPCNT_B(0);

    reg::VRAMCNT_A(0);
    reg::VRAMCNT_B(0);
    reg::VRAMCNT_C(0);
    reg::VRAMCNT_D(0);
    reg::VRAMCNT_E(0);
    reg::VRAMCNT_F(0);
    reg::VRAMCNT_G(0);
    reg::VRAMCNT_H(0);
    reg::VRAMCNT_I(0);

    dsi::memory::fastCopy(nullptr, nullptr, 32);
    while (true) { dsi::bios::vBlankIntrWait(); }
    return 0;
}
