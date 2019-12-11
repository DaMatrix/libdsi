#include "main.h"

int main() {
    dsi::memory::fastCopy(nullptr, nullptr, 32);
    while (true) { dsi::bios::vBlankIntrWait(); }
    return 0;
}
