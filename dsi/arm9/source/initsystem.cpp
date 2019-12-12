#include <dsi.h>

extern "C" void initSystem(void) {
//---------------------------------------------------------------------------------
    register int i;
    // stop timers and dma
    for (i=0; i<4; i++) {
        dsi::dma::Channel* channel = dsi::dma::channel(i);
        channel->control = 0;
        channel->size = 0;
        channel->src = nullptr;
        channel->dst = nullptr;
    }

    // clear video display registers
    /*dmaFillWords(0, (void*)0x04000000, 0x56);
    dmaFillWords(0, (void*)0x04001008, 0x56);

    videoSetModeSub(0);

    vramDefault();

    VRAM_E_CR = 0;
    VRAM_F_CR = 0;
    VRAM_G_CR = 0;
    VRAM_H_CR = 0;
    VRAM_I_CR = 0;*/
}
