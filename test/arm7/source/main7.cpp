#include "main7.h"

using namespace dsi;

volatile bool exitflag = false;

void powerButtonCB() {
    exitflag = true;
}

void vblankHandler() {
}

void vcountHandler() {
    inputGetAndSend();
}

int main() {
    REG_SOUNDCNT |= SOUND_ENABLE;
    writePowerManagement(PM_CONTROL_REG, (readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_MUTE) | PM_SOUND_AMP);
    powerOn(POWER_SOUND);

    readUserSettings();
    ledBlink(0);

    irqInit();
    initClockIRQ();
    fifoInit();

    SetYtrigger(80);

    installSystemFIFO();

    irqSet(IRQ_VCOUNT, vcountHandler);
    irqSet(IRQ_VBLANK, vblankHandler);

    irqEnable(IRQ_VBLANK | IRQ_VCOUNT);

    setPowerButtonCB(powerButtonCB);

    while (!exitflag) {
        if (!(reg::KEYINPUT & (KEY_SELECT | KEY_START | KEY_L | KEY_R))) {
            exitflag = true;
        }
        bios::vBlankIntrWait();
    }
}
