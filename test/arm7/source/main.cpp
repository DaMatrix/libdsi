#include "main.h"

using namespace dsi;

volatile bool exitflag = false;

void powerButtonCB() {
    exitflag = true;
}

void vcountHandler() {
    inputGetAndSend();
}

int main() {
    //clear sound registers
    mem::fastClear((void*) 0x04000400, 0x100);

    irqInit();
    initClockIRQ();
    fifoInit();

    SetYtrigger(80);

    installSystemFIFO();

    irqSet(IRQ_VCOUNT, vcountHandler);

    irqEnable(IRQ_VCOUNT);

    setPowerButtonCB(powerButtonCB);

    while (!exitflag) {
        if (!(reg::KEYINPUT() & (KEY_SELECT | KEY_START | KEY_L | KEY_R))) {
            exitflag = true;
        }
        bios::vBlankIntrWait();
    }
}
