#include "main.h"

using namespace dsi;

int main() {
    reg::DISPCNT_A(0x10003);
    reg::DISPCNT_B(0x10003);

    reg::BG3CNT_A(((1 << 14) | (1 << 7) | (1 << 2) | (4 << 16)));
    reg::DISPCNT_A(reg::DISPCNT_A() | (1 << (8 + 3)));

    auto buf = (u16*) 0x06000000;
    u16 color = 0;
    while (true)    {
        color = (color + 1) & 0x7FFF;
        for (u32 i = 0; i < 256 * 8; i++)   {
            buf[i] = color | 0x8000;
        }
    }

    /*memory::fastCopy(nullptr, nullptr, 32);
    while (true) { bios::vBlankIntrWait(); }
    return 0;*/
}
