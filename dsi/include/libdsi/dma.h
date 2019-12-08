#ifndef LIBDSI_ROOT_H
#define LIBDSI_ROOT_H

#include <libdsi/registers.h>

//TODO: add DSi DMA
// https://problemkaputt.de/gbatek.htm#dsinewdma
namespace dsi::dma {
    constexpr u16 ENABLE      = 1 << 15;
    constexpr u16 IRQ_REQUEST = 1 << 14;
    constexpr u16 GAMEPAK_DRQ = 1 << 11; //DMA3 only
    constexpr u16 IS_32BIT    = 1 << 10;
    constexpr u16 REPEAT      = 1 << 9;

    constexpr u16 DST_INC = 0 << 5;
    constexpr u16 DST_DEC = 1 << 5;
    constexpr u16 DST_FIX = 2 << 5;
    constexpr u16 DST_RLD = 3 << 5;

    constexpr u16 SRC_INC = 0 << 7;
    constexpr u16 SRC_DEC = 1 << 7;
    constexpr u16 SRC_FIX = 2 << 7;

    constexpr u16 START_NOW = 0 << 12;
    constexpr u16 START_VBL = 1 << 12;
    #ifdef ARM9
    constexpr u16 START_HBL     = 2 << 12;
    constexpr u16 START_SPECIAL = 3 << 12; //DMA0=Prohibited, DMA1/DMA2=Sound FIFO, DMA3=Video Capture
    #endif

    /**
     * A wrapper around a DMA channel's registers.
     */
    struct Channel {
        volatile void* src;
        volatile void* dst;
        volatile u16 size;
        volatile u16 control;

        /**
         * @return whether this DMA channel is currently complete
         */
        bool complete() { return (this->control & ENABLE) == 0; }

        /**
         * @return whether this DMA channel is currently active
         */
        bool running() { return (this->control & ENABLE) != 0; }

        /**
         * Stops this DMA channel (doing nothing if it is not currently active).
         */
        void stop() { this->control = 0; }

        /**
         * Blocks until this DMA channel is finished.
         */
        void wait() { while (this->control & ENABLE) {}}

        /**
         * Simply starts this DMA channel with the given settings.
         *
         * @param src the source address
         * @param dst the destination address
         * @param size the number of words/halfwords to copy
         * @param control additional settings
         */
        void start(void* src, void* dst, u16 size, u16 control) {
            while (this->control & ENABLE) {}
            this->src     = src;
            this->dst     = dst;
            this->size    = size;
            this->control = control | ENABLE;
        }
    };

    constexpr Channel* channel(u32 i) { return (Channel*) (0x40000B0 + i * sizeof(Channel)); }

    /*Channel* anyOpenChannel() {
        for (u32 i = 0; i < 4; i++) {
            if (channel(i)->complete()) {
                return channel(i);
            }
        }
        return nullptr;
    }*/
}

#endif //LIBDSI_ROOT_H
