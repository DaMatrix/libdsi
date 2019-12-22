#ifndef LIBDSI_INTR_H
#define LIBDSI_INTR_H

namespace dsi::intr {
    enum Interrupt {
        VBLANK                          = 0,
        HBLANK                          = 1,
        VCOUNT                          = 2,
        TIMER_OVERFLOW_0                = 3,
        TIMER_OVERFLOW_1                = 4,
        TIMER_OVERFLOW_2                = 5,
        TIMER_OVERFLOW_3                = 6,
        DMA0                            = 8,
        DMA1                            = 9,
        DMA2                            = 10,
        DMA3                            = 11,
        KEYPAD                          = 12,
        IPC_SYNC                        = 16,
        IPC_SEND_FIFO_EMPTY             = 17,
        IPC_RECV_FIFO_NOT_EMPTY         = 18,
        GAMECARD_DATA_TRANSFER_COMPLETE = 19,
        GAMECARD_IREQ_MC                = 20,
        NDMA0                           = 28,
        NDMA1                           = 29,
        NDMA2                           = 30,
        NDMA3                           = 31,
#ifdef ARM9
        GEOMETRY_COMMAND_FIFO = 21,
        XPERTTEAK             = 24,
        CAMERA                = 25
#endif
#ifdef ARM7
        RTC                 = 7,
        SCREENS_UNFOLDING   = 22,
        SPI_BUS             = 23,
        WIFI                = 24,
        GPIO18_0            = 32 + 0,
        GPIO18_1            = 32 + 1,
        GPIO18_2            = 32 + 2,
        GPIO33_0            = 32 + 4,
        GPIO33_1            = 32 + 5,
        HEADPHONE_CONNECT   = 32 + 5,
        GPIO33_2            = 32 + 6,
        POWER_BUTTON        = 32 + 6,
        GPIO33_3            = 32 + 7,
        SDMMC_CONTROLLER    = 32 + 8,
        SD_SLOT_DATA1       = 32 + 9,
        SDIO_CONTROLLER     = 32 + 10,
        SDIO_DATA1          = 32 + 11,
        AES                 = 32 + 12,
        I2C                 = 32 + 13,
        MICROPHONE_EXTENDED = 32 + 14
#endif
    };

    /**
     * Initializes the interrupt handler system, clearing any currently registered interrupt handlers.
     */
    void init();

    /**
     * Sets the root interrupt handler (the function that will be called every time an interrupt occurs).
     *
     * The default value for this (as set by init()) delegates interrupts to the corresponding handlers as set by the other methods in this namespace,
     * be aware that using a custom handler may prevent these handlers from working.
     *
     * @param handler the new root handler to use
     */
    void setRootHandler(Void handler);

    /**
     * Sets the handler for the given interrupt.
     *
     * If a handler is already set for the given interrupt, it will be replaced.
     *
     * @param interrupt the interrupt to listen for
     * @param handler   the handler for the given interrupt
     */
    void set(Interrupt interrupt, Void handler);
}

#endif //LIBDSI_INTR_H
