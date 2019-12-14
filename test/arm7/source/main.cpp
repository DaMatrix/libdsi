#include "main.h"

using namespace dsi;

volatile bool exitflag = false;

void powerButtonCB() {
    exitflag = true;
}

int main() {// clear sound registers
    dmaFillWords(0, (void*) 0x04000400, 0x100);

    setPowerButtonCB(powerButtonCB);

    while (!exitflag) {
        if (!(reg::KEYINPUT() & (KEY_SELECT | KEY_START | KEY_L | KEY_R))) {
            exitflag = true;
        }
        bios::vBlankIntrWait();
    }
}
