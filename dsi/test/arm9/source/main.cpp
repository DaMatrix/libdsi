#include "main.h"

using namespace dsi;

int main() {
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
