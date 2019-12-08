#ifndef LIBDSI_ROOT_H
#define LIBDSI_ROOT_H

#include <libdsi/registers.h>

//TODO: add DSi DMA
// https://problemkaputt.de/gbatek.htm#dsinewdma
namespace dsi::dma   {
    constexpr u16 ENABLE = 1 << 15;
    constexpr u16 IRQ_REQUEST = 1 << 14;
    constexpr u16 GAMEPAK_DRQ = 1 << 11; //DMA3 only
    constexpr u16 IS_32BIT = 1 << 10;
    constexpr u16 REPEAT = 1 << 9;

    constexpr u16 DST_CTRL_INCREMENT = 0 << 5;
    constexpr u16 DST_CTRL_DECREMENT = 1 << 5;
    constexpr u16 DST_FIX     = 2 << 5;
    constexpr u16 DST_CTRL_RELOAD    = 3 << 5;

    constexpr u16 SRC_CTRL_INCREMENT  = 0 << 7;
    constexpr u16 SRC_CTRL_DECREMENT  = 1 << 7;
    constexpr u16 SRC_FIX      = 2 << 7;

    constexpr u16 START_IMMEDIATE = 0 << 12;
    constexpr u16 START_VBL    = 1 << 12;
    constexpr u16 START_HBLANK    = 2 << 12;
    constexpr u16 START_SPECIAL   = 3 << 12; //DMA0=Prohibited, DMA1/DMA2=Sound FIFO, DMA3=Video Capture

    /**
     * A wrapper around a DMA channel's registers.
     */
    struct Channel {
        void* src;
        void* dst;
        u16 size;
        u16 control;

        /**
         * @return whether this DMA channel is currently complete
         */
        constexpr bool complete() { return (this->control & ENABLE) == 0; }

        /**
         * @return whether this DMA channel is currently active
         */
        constexpr bool running() { return (this->control & ENABLE) != 0; }

        /**
         * Stops this DMA channel (doing nothing if it is not currently active).
         */
        constexpr void stop() { this->control = 0; }
    };

    constexpr Channel* channel(u32 i) { return (Channel*) (0x40000B0 + i * sizeof(Channel)); }
}

#endif //LIBDSI_ROOT_H
