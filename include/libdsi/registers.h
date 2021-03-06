#ifndef LIBDSI_REGISTERS_H
#define LIBDSI_REGISTERS_H

#include <libdsi/types.h>

#ifdef __JETBRAINS_IDE__
#define _ENUM_VOLATILE volatile
#else
#define _ENUM_VOLATILE
#endif

#define REG_R(ADDRESS, TYPE, NAME) static volatile const TYPE& NAME = *((volatile TYPE*) (ADDRESS));
#define REG_W(ADDRESS, TYPE, NAME) static volatile TYPE& NAME = *((volatile TYPE*) (ADDRESS));
#define REG_RW(ADDRESS, TYPE, NAME) static volatile TYPE& NAME = *((volatile TYPE*) (ADDRESS));

#ifdef __JETBRAINS_IDE__
//basically just hides some erroneous compile errors from CLion
#define BLOCK_R(ADDRESS, SIZE, TYPE, NAME) static const TYPE (&NAME)[SIZE / sizeof(TYPE)] = nullptr;
#define BLOCK_W(ADDRESS, SIZE, TYPE, NAME) static TYPE (&NAME)[SIZE / sizeof(TYPE)] = nullptr;
#define BLOCK_RW(ADDRESS, SIZE, TYPE, NAME) static TYPE (&NAME)[SIZE / sizeof(TYPE)] = nullptr;
#else
#define BLOCK_R(ADDRESS, SIZE, TYPE, NAME) static const TYPE (&NAME)[SIZE / sizeof(TYPE)] = (TYPE (&)[SIZE / sizeof(TYPE)]) *((TYPE*) (ADDRESS));
#define BLOCK_W(ADDRESS, SIZE, TYPE, NAME) static TYPE (&NAME)[SIZE / sizeof(TYPE)] = (TYPE (&)[SIZE / sizeof(TYPE)]) *((TYPE*) (ADDRESS));
#define BLOCK_RW(ADDRESS, SIZE, TYPE, NAME) static TYPE (&NAME)[SIZE / sizeof(TYPE)] = (TYPE (&)[SIZE / sizeof(TYPE)]) *((TYPE*) (ADDRESS));
#endif

/**
 * Definitions of all NDS registers, along with some basic documentation.
 *
 * Mostly copied from gbatek (https://problemkaputt.de/gbatek.htm), with some adaptations.
 *
 * I sincerely hope that copying so much stuff from gbatek won't cause issues, I just want to be able to see documentation of the registers directly
 * in my IDE while I'm coding, without having to have multiple browser tabs open on a second screen to see register info. Martin Korth - if you want
 * me to get rid of all the comments, just tell me.
 *
 * All the macro calls have a fully namespaced type because CLion shows up with errors otherwise.
 */
namespace dsi::reg {
    /**
     * Extended Palettes
     * When allocating extended palettes, the allocated memory is not mapped to the CPU bus, so the CPU can access extended palette only when
     * temporarily de-allocating it.
     *
     * Color 0 of all standard/extended palettes is transparent, color 0 of BG standard palette 0 is used as backdrop. extended palette memory must
     * be allocated to VRAM.
     *
     * BG Extended Palette enabled in DISPCNT Bit 30, when enabled,
     *  standard palette --> 16-color tiles (with 16bit bgmap entries) (text)
     *                       256-color tiles (with 8bit bgmap entries) (rot/scal)
     *                       256-color bitmaps
     *                       backdrop-color (color 0)
     *  extended palette --> 256-color tiles (with 16bit bgmap entries)(text,rot/scal)
     *
     * Allocated VRAM is split into 4 slots of 8K each (32K used in total), normally BG0..3 are using Slot 0..3, however BG0 and BG1 can be optionally
     * changed to BG0=Slot2, and BG1=Slot3 via BG0CNT and BG1CNT.
     *
     * OBJ Extended Palette enabled in DISPCNT Bit 31, when enabled,
     *
     * 16 colors x 16 palettes --> standard palette memory (=256 colors)
     * 256 colors x 16 palettes --> extended palette memory (=4096 colors)
     *
     * Extended OBJ palette memory must be allocated to VRAM F, G, or I (which are 16K) of which only the first 8K are used for extended
     * palettes (=1000h 16bit entries).
     */
    BLOCK_RW(0x05000000, 0x200, u16, BG_PALETTE_A)
    BLOCK_RW(0x05000200, 0x200, u16, OBJ_PALETTE_A)
    BLOCK_RW(0x05000400, 0x200, u16, BG_PALETTE_B)
    BLOCK_RW(0x05000600, 0x200, u16, OBJ_PALETTE_B)

    /**
     * The total size of all palette memory.
     */
    constexpr u32 SIZE_PALETTE = sizeof(BG_PALETTE_A) + sizeof(OBJ_PALETTE_A) + sizeof(BG_PALETTE_B) + sizeof(OBJ_PALETTE_B);

    BLOCK_RW(0x07000000, 0x400, u16, OAM_A)
    BLOCK_RW(0x07000400, 0x400, u16, OAM_B)

    /**
     * The total size of all OAM.
     */
    constexpr u32 SIZE_OAM = sizeof(OAM_A) + sizeof(OAM_B);

    BLOCK_RW(0x06800000, 0x20000, u16, VRAM_A)
    BLOCK_RW(0x06820000, 0x20000, u16, VRAM_B)
    BLOCK_RW(0x06840000, 0x20000, u16, VRAM_C)
    BLOCK_RW(0x06860000, 0x20000, u16, VRAM_D)
    BLOCK_RW(0x06880000, 0x10000, u16, VRAM_E)
    BLOCK_RW(0x06890000, 0x4000, u16, VRAM_F)
    BLOCK_RW(0x06894000, 0x4000, u16, VRAM_G)
    BLOCK_RW(0x06898000, 0x8000, u16, VRAM_H)
    BLOCK_RW(0x068A0000, 0x4000, u16, VRAM_I)

    /**
     * The total size of all VRAM banks.
     */
    constexpr u32 SIZE_VRAM = sizeof(VRAM_A) + sizeof(VRAM_B) + sizeof(VRAM_C) + sizeof(VRAM_D) + sizeof(VRAM_E)
                              + sizeof(VRAM_F) + sizeof(VRAM_G) + sizeof(VRAM_H) + sizeof(VRAM_I);

    /**
     * 2D Engine A+B - DISPSTAT - General LCD Status (Read/Write)
     */
    REG_RW(0x04000004, u16, DISPSTAT)

    /**
     * 2D Engine A+B - VCOUNT - Vertical Counter (Read only)
     */
    REG_R(0x04000006, u16, VCOUNT)

    // ARM9 Display Engine A
    #ifdef ARM9
    /**
     * 2D Engine A - DISPCNT - LCD Control (Read/Write)
     */
    REG_RW(0x04000000, u32, DISPCNT_A)

    /**
     * BG0 Control
     */
    REG_RW(0x04000008, u16, BG0CNT_A)

    /**
     * BG1 Control
     */
    REG_RW(0x040000A0, u16, BG1CNT_A)

    /**
     * BG2 Control
     */
    REG_RW(0x040000C0, u16, BG2CNT_A)

    /**
     * BG3 Control
     */
    REG_RW(0x040000E0, u16, BG3CNT_A)

    /**
     * BG0 X-Offset
     */
    REG_W(0x04000010, u16, BG0HOFS_A)

    /**
     * BG0 Y-Offset
     */
    REG_W(0x04000012, u16, BG0VOFS_A)

    /**
     * BG1 X-Offset
     */
    REG_W(0x04000014, u16, BG1HOFS_A)

    /**
     * BG1 Y-Offset
     */
    REG_W(0x04000016, u16, BG1VOFS_A)

    /**
     * BG2 X-Offset
     */
    REG_W(0x04000018, u16, BG2HOFS_A)

    /**
     * BG2 Y-Offset
     */
    REG_W(0x0400001A, u16, BG2VOFS_A)

    /**
     * BG3 X-Offset
     */
    REG_W(0x0400001C, u16, BG3HOFS_A)

    /**
     * BG3 Y-Offset
     */
    REG_W(0x0400001E, u16, BG3VOFS_A)

    /**
     * BG2 Rotation/Scaling Parameter A (dx)
     */
    REG_W(0x04000020, u16, BG2PA_A)

    /**
     * BG2 Rotation/Scaling Parameter B (dmx)
     */
    REG_W(0x04000022, u16, BG2PB_A)

    /**
     * BG2 Rotation/Scaling Parameter C (dy)
     */
    REG_W(0x04000024, u16, BG2PC_A)

    /**
     * BG2 Rotation/Scaling Parameter D (dmy)
     */
    REG_W(0x04000026, u16, BG2PD_A)

    /**
     * BG2 Reference Point X-Coordinate
     */
    REG_W(0x04000028, u32, BG2X_A)

    /**
     * BG2 Reference Point Y-Coordinate
     */
    REG_W(0x0400002C, u32, BG2Y_A)

    /**
     * BG3 Rotation/Scaling Parameter A (dx)
     */
    REG_W(0x04000030, u16, BG3PA_A)

    /**
     * BG3 Rotation/Scaling Parameter B (dmx)
     */
    REG_W(0x04000032, u16, BG3PB_A)

    /**
     * BG3 Rotation/Scaling Parameter C (dy)
     */
    REG_W(0x04000034, u16, BG3PC_A)

    /**
     * BG3 Rotation/Scaling Parameter D (dmy)
     */
    REG_W(0x04000036, u16, BG3PD_A)

    /**
     * BG3 Reference Point X-Coordinate
     */
    REG_W(0x04000038, u32, BG3X_A)

    /**
     * BG3 Reference Point Y-Coordinate
     */
    REG_W(0x0400003C, u32, BG3Y_A)

    /**
     * Window 0 Horizontal Dimensions
     */
    REG_W(0x04000040, u16, WIN0H_A)

    /**
     * Window 1 Horizontal Dimensions
     */
    REG_W(0x04000042, u16, WIN1H_A)

    /**
     * Window 0 Vertical Dimensions
     */
    REG_W(0x04000044, u16, WIN0V_A)

    /**
     * Window 1 Vertical Dimensions
     */
    REG_W(0x04000046, u16, WIN1V_A)

    /**
     * Inside of Window 0 and 1
     */
    REG_RW(0x04000048, u16, WININ_A)

    /**
     * Inside of OBJ Window & Outside of Windows
     */
    REG_RW(0x0400004A, u16, WINOUT_A)

    /**
     * Mosaic Size
     */
    REG_W(0x0400004C, u16, MOSAIC_A)

    /**
     * Color Special Effects Selection
     */
    REG_RW(0x04000050, u16, BLDCNT_A)

    /**
     * Alpha Blending Coefficients
     */
    REG_RW(0x04000052, u16, BLDALPHA_A)

    /**
     * Brightness (Fade-In/Out) Coefficient
     */
    REG_W(0x04000054, u16, BLDY_A)

    /**
     * 3D Display Control Register
     */
    REG_RW(0x04000060, u16, DISP3DCNT_A)

    /**
     * Display Capture Control Register
     */
    REG_RW(0x04000064, u32, DISPCAPCNT_A)

    /**
     * Main Memory Display FIFO
     */
    REG_RW(0x04000068, u32, DISP_MMEM_FIFO_A)

    /**
     * 2D Engine A - Master Brightness Up/Down
     */
    REG_RW(0x0400006C, u16, MASTER_BRIGHT_A)
    #endif

    // ARM9 Display Engine B
    #ifdef ARM9
    //TODO: i'm not quite certain about these
    /**
     * 2D Engine B - DISPCNT - LCD Control (Read/Write)
     */
    REG_RW(0x04000000 + 0x1000, u32, DISPCNT_B)

    /**
     * BG0 Control
     */
    REG_RW(0x04000008 + 0x1008, u16, BG0CNT_B)

    /**
     * BG1 Control
     */
    REG_RW(0x040000A0 + 0x1008, u16, BG1CNT_B)

    /**
     * BG2 Control
     */
    REG_RW(0x040000C0 + 0x1008, u16, BG2CNT_B)

    /**
     * BG3 Control
     */
    REG_RW(0x040000E0 + 0x1008, u16, BG3CNT_B)

    /**
     * BG0 X-Offset
     */
    REG_W(0x04000010 + 0x1008, u16, BG0HOFS_B)

    /**
     * BG0 Y-Offset
     */
    REG_W(0x04000012 + 0x1008, u16, BG0VOFS_B)

    /**
     * BG1 X-Offset
     */
    REG_W(0x04000014 + 0x1008, u16, BG1HOFS_B)

    /**
     * BG1 Y-Offset
     */
    REG_W(0x04000016 + 0x1008, u16, BG1VOFS_B)

    /**
     * BG2 X-Offset
     */
    REG_W(0x04000018 + 0x1008, u16, BG2HOFS_B)

    /**
     * BG2 Y-Offset
     */
    REG_W(0x0400001A + 0x1008, u16, BG2VOFS_B)

    /**
     * BG3 X-Offset
     */
    REG_W(0x0400001C + 0x1008, u16, BG3HOFS_B)

    /**
     * BG3 Y-Offset
     */
    REG_W(0x0400001E + 0x1008, u16, BG3VOFS_B)

    /**
     * BG2 Rotation/Scaling Parameter A (dx)
     */
    REG_W(0x04000020 + 0x1008, u16, BG2PA_B)

    /**
     * BG2 Rotation/Scaling Parameter B (dmx)
     */
    REG_W(0x04000022 + 0x1008, u16, BG2PB_B)

    /**
     * BG2 Rotation/Scaling Parameter C (dy)
     */
    REG_W(0x04000024 + 0x1008, u16, BG2PC_B)

    /**
     * BG2 Rotation/Scaling Parameter D (dmy)
     */
    REG_W(0x04000026 + 0x1008, u16, BG2PD_B)

    /**
     * BG2 Reference Point X-Coordinate
     */
    REG_W(0x04000028 + 0x1008, u32, BG2X_B)

    /**
     * BG2 Reference Point Y-Coordinate
     */
    REG_W(0x0400002C + 0x1008, u32, BG2Y_B)

    /**
     * BG3 Rotation/Scaling Parameter A (dx)
     */
    REG_W(0x04000030 + 0x1008, u16, BG3PA_B)

    /**
     * BG3 Rotation/Scaling Parameter B (dmx)
     */
    REG_W(0x04000032 + 0x1008, u16, BG3PB_B)

    /**
     * BG3 Rotation/Scaling Parameter C (dy)
     */
    REG_W(0x04000034 + 0x1008, u16, BG3PC_B)

    /**
     * BG3 Rotation/Scaling Parameter D (dmy)
     */
    REG_W(0x04000036 + 0x1008, u16, BG3PD_B)

    /**
     * BG3 Reference Point X-Coordinate
     */
    REG_W(0x04000038 + 0x1008, u32, BG3X_B)

    /**
     * BG3 Reference Point Y-Coordinate
     */
    REG_W(0x0400003C + 0x1008, u32, BG3Y_B)

    /**
     * Window 0 Horizontal Dimensions
     */
    REG_W(0x04000040 + 0x1008, u16, WIN0H_B)

    /**
     * Window 1 Horizontal Dimensions
     */
    REG_W(0x04000042 + 0x1008, u16, WIN1H_B)

    /**
     * Window 0 Vertical Dimensions
     */
    REG_W(0x04000044 + 0x1008, u16, WIN0V_B)

    /**
     * Window 1 Vertical Dimensions
     */
    REG_W(0x04000046 + 0x1008, u16, WIN1V_B)

    /**
     * Inside of Window 0 and 1
     */
    REG_RW(0x04000048 + 0x1008, u16, WININ_B)

    /**
     * Inside of OBJ Window & Outside of Windows
     */
    REG_RW(0x0400004A + 0x1008, u16, WINOUT_B)

    /**
     * Mosaic Size
     */
    REG_W(0x0400004C + 0x1008, u16, MOSAIC_B)

    /**
     * Color Special Effects Selection
     */
    REG_RW(0x04000050 + 0x1008, u16, BLDCNT_B)

    /**
     * Alpha Blending Coefficients
     */
    REG_RW(0x04000052 + 0x1008, u16, BLDALPHA_B)

    /**
     * Brightness (Fade-In/Out) Coefficient
     */
    REG_W(0x04000054 + 0x1008, u16, BLDY_B)

    /**
     * 2D Engine B - Master Brightness Up/Down
     */
    REG_RW(0x0400006C + 0x1000, u16, MASTER_BRIGHT_B)
    #endif

    // ARM9 DMA and Timers
    #ifdef ARM9
    /**
     * DMA 0 Source Address (internal memory)
     * DMA 1-3 Source Address (any memory)
     *
     * The most significant address bits are ignored, only the least significant 27 or 28 bits are used (max 07FFFFFFh internal memory, or max
     * 0FFFFFFFh any memory - except SRAM ?!).
     */
    REG_W(0x040000B0, void*, DMA0SAD)
    REG_W(0x040000BC, void*, DMA1SAD)
    REG_W(0x040000C8, void*, DMA2SAD)
    REG_W(0x040000D4, void*, DMA3SAD)

    /**
     * DMA 0-2 Destination Address (internal memory)
     * DMA 3 Destination Address (any memory)
     *
     * The most significant address bits are ignored, only the least significant 27 or 28 bits are used (max. 07FFFFFFh internal memory or
     * 0FFFFFFFh any memory - except SRAM ?!).
     */
    REG_W(0x040000B4, void*, DMA0DAD);
    REG_W(0x040000C0, void*, DMA1DAD);
    REG_W(0x040000CC, void*, DMA2DAD);
    REG_W(0x040000D8, void*, DMA3DAD);

    /**
     * DMA 0-2 Word Count (14-bit)
     * DMA 3 Word Count (16-bit)
     *
     * Specifies the number of data units to be transferred, each unit is 16bit or 32bit depending on the transfer type, a value of zero is
     * treated as max length (ie. 4000h, or 10000h for DMA3).
     */
    REG_W(0x040000B8, u16, DMA0CNT_L)
    REG_W(0x040000C4, u16, DMA1CNT_L)
    REG_W(0x040000D0, u16, DMA2CNT_L)
    REG_RW(0x040000DC, u16, DMA3CNT_L)

    /**
     * DMA 0-3 Control
     *
     * Bit   Expl.
     * 0-4   Not used
     * 5-6   Dest Addr Control  (0=Increment,1=Decrement,2=Fixed,3=Increment/Reload)
     * 7-8   Source Adr Control (0=Increment,1=Decrement,2=Fixed,3=Prohibited)
     * 9     DMA Repeat                   (0=Off, 1=On) (Must be zero if Bit 11 set)
     * 10    DMA Transfer Type            (0=16bit, 1=32bit)
     * 11    Game Pak DRQ  - DMA3 only -  (0=Normal, 1=DRQ <from> Game Pak, DMA3)
     * 12-13 DMA Start Timing  (0=Immediately, 1=VBlank, 2=HBlank, 3=Special)
     *         The 'Special' setting (Start Timing=3) depends on the DMA channel:
     *         DMA0=Prohibited, DMA1/DMA2=Sound FIFO, DMA3=Video Capture
     * 14    IRQ upon end of Word Count   (0=Disable, 1=Enable)
     * 15    DMA Enable                   (0=Off, 1=On)
     *
     * After changing the Enable bit from 0 to 1, wait 2 clock cycles before accessing any DMA related registers.
     *
     * When accessing OAM (7000000h) or OBJ VRAM (6010000h) by HBlank Timing, then the "H-Blank Interval Free" bit in DISPCNT register must be set.
     */
    REG_W(0x040000BA, u16, DMA0CNT_H)
    REG_W(0x040000C6, u16, DMA1CNT_H)
    REG_W(0x040000D2, u16, DMA2CNT_H)
    REG_W(0x040000DE, u16, DMA3CNT_H)
    #endif

    // Keypad (ARM7 and ARM9)
    #if defined(ARM7) || defined(ARM9)

    /**
     * Key Status
     *
     * Bit   Expl.
     * 0     Button A        (0=Pressed, 1=Released)
     * 1     Button B        (etc.)
     * 2     Select          (etc.)
     * 3     Start           (etc.)
     * 4     Right           (etc.)
     * 5     Left            (etc.)
     * 6     Up              (etc.)
     * 7     Down            (etc.)
     * 8     Button R        (etc.)
     * 9     Button L        (etc.)
     * 10-15 Not used
     *
     * It'd be usually recommended to read-out this register only once per frame, and to store the current state in memory. As a side effect,
     * this method avoids problems caused by switch bounce when a key is newly released or pressed.
     */
    REG_R(0x04000130, u16, KEYINPUT)

    /**
     * Key Interrupt Control
     *
     * The keypad IRQ function is intended to terminate the very-low-power Stop mode, it is not suitable for processing normal user input, to
     * do this, most programs are invoking their keypad handlers from within VBlank IRQ.
     *
     * Bit   Expl.
     * 0     Button A        (0=Ignore, 1=Select)
     * 1     Button B        (etc.)
     * 2     Select          (etc.)
     * 3     Start           (etc.)
     * 4     Right           (etc.)
     * 5     Left            (etc.)
     * 6     Up              (etc.)
     * 7     Down            (etc.)
     * 8     Button R        (etc.)
     * 9     Button L        (etc.)
     * 10-13 Not used
     * 14    IRQ Enable Flag (0=Disable, 1=Enable)
     * 15    IRQ Condition   (0=Logical OR, 1=Logical AND)
     *
     * In logical OR mode, an interrupt is requested when at least one of the selected buttons is pressed.
     * In logical AND mode, an interrupt is requested when ALL of the selected buttons are pressed.
     */
    REG_RW(0x04000132, u16, KEYCNT)

    #ifdef ARM7
    /**
     * Key X/Y Input
     *
     * Bit  Expl.
     * 0      Button X     (0=Pressed, 1=Released)
     * 1      Button Y     (0=Pressed, 1=Released)
     * 3      DEBUG button (0=Pressed, 1=Released/None such)
     * 6      Pen down     (0=Pressed, 1=Released/Disabled) (always 0 in DSi mode)
     * 7      Hinge/folded (0=Open, 1=Closed)
     * 2,4,5  Unknown / set
     * 8..15  Unknown / zero
     *
     * The Hinge stuff is a magnetic sensor somewhere underneath of the Start/Select buttons (NDS) or between A/B/X/Y buttons (DSi), it will be
     * triggered by the magnet field from the right speaker when the console is closed. The hinge generates an interrupt request (there seems to
     * be no way to disable this, unlike as for all other IRQ sources), however, the interrupt execution can be disabled in IE register (as for
     * other IRQ sources).
     *
     * The Pen Down is the /PENIRQ signal from the Touch Screen Controller (TSC), if it is enabled in the TSC control register, then it will
     * notify the program when the screen pressed, the program should then read data from the TSC (if there's no /PENIRQ then doing unneccassary
     * TSC reads would just waste CPU power). However, the user may release the screen before the program performs the TSC read, so treat the
     * screen as not pressed if you get invalid TSC values (even if /PENIRQ was LOW).
     *
     * Not sure if the TSC /PENIRQ is actually triggering an IRQ in the NDS?
     * The Debug Button should be connected to R03 and GND (on original NDS, R03 is the large soldering point between the SL1 jumper and the VR1
     * potentiometer) (there is no R03 signal visible on the NDS-Lite board).
     *
     * Interrupts are reportedly not supported for X,Y buttons.
     */
    REG_R(0x04000136, u16, EXTKEYIN)
    #endif
    #endif

    // IPC/ROM
    #if defined(ARM7) || defined(ARM9)

    /**
     * IPC Synchronize Register
     *
     * Bit   Dir  Expl.
     * 0-3   R    Data input from IPCSYNC Bit8-11 of remote CPU (00h..0Fh)
     * 4-7   -    Not used
     * 8-11  R/W  Data output to IPCSYNC Bit0-3 of remote CPU   (00h..0Fh)
     * 12    -    Not used
     * 13    W    Send IRQ to remote CPU      (0=None, 1=Send IRQ)
     * 14    R/W  Enable IRQ from remote CPU  (0=Disable, 1=Enable)
     * 15-31 -    Not used
     */
    REG_RW(0x04000180, u32, IPCSYNC)

    /**
     * IPC Fifo Control Register
     *
     * Bit   Dir  Expl.
     * 0     R    Send Fifo Empty Status      (0=Not Empty, 1=Empty)
     * 1     R    Send Fifo Full Status       (0=Not Full, 1=Full)
     * 2     R/W  Send Fifo Empty IRQ         (0=Disable, 1=Enable)
     * 3     W    Send Fifo Clear             (0=Nothing, 1=Flush Send Fifo)
     * 4-7   -    Not used
     * 8     R    Receive Fifo Empty          (0=Not Empty, 1=Empty)
     * 9     R    Receive Fifo Full           (0=Not Full, 1=Full)
     * 10    R/W  Receive Fifo Not Empty IRQ  (0=Disable, 1=Enable)
     * 11-13 -    Not used
     * 14    R/W  Error, Read Empty/Send Full (0=No Error, 1=Error/Acknowledge)
     * 15    R/W  Enable Send/Receive Fifo    (0=Disable, 1=Enable)
     * 16-31 -    Not used
     */
    REG_RW(0x04000184, u32, IPCFIFOCOUNT)

    /**
     * IPC Send Fifo
     *
     * Bit0-31  Send Fifo Data (max 16 words; 64bytes)
     */
    REG_W(0x04000188, u32, IPCFIFOSEND)

    /**
     * IPC Receive Fifo
     *
     * Bit0-31  Receive Fifo Data (max 16 words; 64bytes)
     */
    REG_R(0x04100000, u32, IPCFIFORECV)

    /**
     * Gamecard ROM and SPI Control
     */
    REG_RW(0x040001A0, u16, AUXSPICNT)

    /**
     * Gamecard SPI Bus Data/Strobe
     */
    REG_RW(0x040001A2, u16, AUXSPIDATA)

    /**
     * Gamecard bus timing/control
     */
    REG_RW(0x040001A4, u32, GAMECARD_BUS_TIMING_CONTROL)

    /**
     * Gamecard bus 8-byte command out
     */
    REG_RW(0x040001A8, u64, GAMECARD_BUS_CMD_OUT)

    /**
     * Gamecard Encryption Seed 0 Lower 32bit
     */
    REG_RW(0x040001B0, u32, GAMECARD_ENCRYPTION_SEED_0_LOWER)

    /**
     * Gamecard Encryption Seed 1 Lower 32bit
     */
    REG_RW(0x040001B4, u32, GAMECARD_ENCRYPTION_SEED_1_LOWER)

    /**
     * Gamecard Encryption Seed 0 Upper 7bit (bit7-15 unused)
     */
    REG_RW(0x040001B8, u16, GAMECARD_ENCRYPTION_SEED_0_UPPER)

    /**
     * Gamecard Encryption Seed 1 Upper 7bit (bit7-15 unused)
     */
    REG_RW(0x040001BA, u16, GAMECARD_ENCRYPTION_SEED_1_UPPER)
    #endif

    // ARM9 Math
    #ifdef ARM9
    /**
     * Division Control
     */
    REG_RW(0x04000280, u8, DIVCNT)

    /**
     * Division Numerator
     */
    REG_RW(0x04000290, u64, DIV_NUMER)

    /**
     * Division Denominator
     */
    REG_RW(0x04000298, u64, DIV_DENOM)

    /**
     * Division Quotient (=Numer/Denom)
     */
    REG_R(0x040002A0, u64, DIV_RESULT)

    /**
     * Division Remainder (=Numer MOD Denom)
     */
    REG_R(0x040002A8, u64, DIVREM_RESULT)

    /**
     * Square Root Control
     */
    REG_RW(0x040002B0, u8, SQRTCNT)

    /**
     * Square Root Result
     */
    REG_R(0x040002B4, u32, SQRT_RESULT)

    /**
     * Square Root Parameter Input
     */
    REG_RW(0x040002B8, u64, SQRT_PARAM)

    /**
     * Graphics Power Control Register
     *
     * 0     Enable Flag for both LCDs (0=Disable) (Prohibited, see notes)
     * 1     2D Graphics Engine A      (0=Disable) (Ports 008h-05Fh, Pal 5000000h)
     * 2     3D Rendering Engine       (0=Disable) (Ports 320h-3FFh)
     * 3     3D Geometry Engine        (0=Disable) (Ports 400h-6FFh)
     * 4-8   Not used
     * 9     2D Graphics Engine B      (0=Disable) (Ports 1008h-105Fh, Pal 5000400h)
     * 10-14 Not used
     * 15    Display Swap (0=Send Display A to Lower Screen, 1=To Upper Screen)
     * 16-31 Not used
     *
     * Use SwapBuffers command once after enabling Rendering/Geometry Engine.
     * Improper use of Bit0 may damage the hardware?
     * When disabled, corresponding ports become read-only, corresponding (palette-) memory becomes read-only-zero-filled.
     */
    REG_RW(0x04000304, u32, POWCNT1)
    #endif

    // ARM9 3D Display Engine
    #ifdef ARM9
    //TODO
    #endif

    // ARM7 Misc Registers
    #ifdef ARM7
    /**
     * Real Time Clock Register
     *
     * Bit  Expl.
     * 0    Data I/O   (0=Low, 1=High)
     * 1    Clock Out  (0=Low, 1=High)
     * 2    Select Out (0=Low, 1=High/Select)
     * 4    Data  Direction  (0=Read, 1=Write)
     * 5    Clock Direction  (should be 1=Write)
     * 6    Select Direction (should be 1=Write)
     * 3,8-11   Unused I/O Lines
     * 7,12-15  Direction for Bit3,8-11 (usually 0)
     * 16-31    Not used
     */
    REG_RW(0x04000138, u32, RTC)

    /**
     * SPI Bus Control/Status Register
     *
     * Bit  Expl.
     * 0-1   Baudrate (0=4MHz/Firmware, 1=2MHz/Touchscr, 2=1MHz/Powerman., 3=512KHz)
     * 2-6   Not used            (Zero)
     * 7     Busy Flag           (0=Ready, 1=Busy) (presumably Read-only)
     * 8-9   Device Select       (0=Powerman., 1=Firmware, 2=Touchscr, 3=Reserved)
     * 10    Transfer Size       (0=8bit/Normal, 1=16bit/Bugged)
     * 11    Chipselect Hold     (0=Deselect after transfer, 1=Keep selected)
     * 12-13 Not used            (Zero)
     * 14    Interrupt Request   (0=Disable, 1=Enable)
     * 15    SPI Bus Enable      (0=Disable, 1=Enable)
     */
    REG_RW(0x040001C0, u16, SPICNT)

    /**
     * SPI Bus Data/Strobe Register
     *
     * Bit  Expl.
     * 0-7   Data
     * 8-15  Not used (always zero, even in bugged-16bit mode)
     *
     * The SPI transfer is started on writing to this register, so one must <write> a dummy value (should be zero) even when intending to <read>
     * from SPI bus.
     *
     * During transfer, the Busy flag in SPICNT is set, and the written SPIDATA value is transferred to the device (via output line),
     * simultaneously data is received (via input line). Upon transfer completion, the Busy flag goes off (with optional IRQ), and the received
     * value can be then read from SPIDATA, if desired.
     */
    REG_RW(0x040001C2, u16, SPIDATA)
    #endif

    // Memory and Interrupt control (ARM7 and ARM9)
    #ifdef DSI

    /**
     * External Memory Control/Status
     *
     * 0-1   32-pin GBA Slot SRAM Access Time    (0-3 = 10, 8, 6, 18 cycles)
     * 2-3   32-pin GBA Slot ROM 1st Access Time (0-3 = 10, 8, 6, 18 cycles)
     * 4     32-pin GBA Slot ROM 2nd Access Time (0-1 = 6, 4 cycles)
     * 5-6   32-pin GBA Slot PHI-pin out   (0-3 = Low, 4.19MHz, 8.38MHz, 16.76MHz)
     * 7     32-pin GBA Slot Access Rights     (0=ARM9, 1=ARM7)
     * 8-10  Not used (always zero)
     * 11    17-pin NDS Slot Access Rights     (0=ARM9, 1=ARM7)
     * 12    Not used (always zero)
     * 13    NDS:Always set?  ;set/tested by DSi bootcode: Main RAM enable, CE2 pin?
     * 14    Main Memory Interface Mode Switch (0=Async/GBA/Reserved, 1=Synchronous)
     * 15    Main Memory Access Priority       (0=ARM9 Priority, 1=ARM7 Priority)
     *
     * Bit0-6 can be changed by both NDS9 and NDS7, changing these bits affects the local EXMEM register only, not that of the other CPU.
     * Bit7-15 can be changed by NDS9 only, changing these bits affects both EXMEM registers, ie. both NDS9 and NDS7 can read the current NDS9 setting.
     * Bit14=0 is intended for GBA mode, however, writes to this bit appear to be ignored?
     */
    #ifdef ARM7
    REG_RW(0x04000206, u16, EXMEMSTAT)
    #else
    REG_RW(0x04000206, u16, EXMEMCNT)
    #endif

    /**
     * Interrupt Master Enable
     */
    REG_RW(0x04000208, u32, IME)

    /**
     * Interrupt Enable
     * 
     * Bits in the IE register are 0=Disable, 1=Enable.
     * Reading IF returns 0=No request, 1=Interrupt Request.
     * Writing IF acts as 0=No change, 1=Acknowledge (clears that bit).
     * 
     * 0     LCD V-Blank
     * 1     LCD H-Blank
     * 2     LCD V-Counter Match
     * 3     Timer 0 Overflow
     * 4     Timer 1 Overflow
     * 5     Timer 2 Overflow
     * 6     Timer 3 Overflow
     * 7     NDS7 only: SIO/RCNT/RTC (Real Time Clock)
     * 8     DMA 0
     * 9     DMA 1
     * 10    DMA 2
     * 11    DMA 3
     * 12    Keypad
     * 13    GBA-Slot (external IRQ source) / DSi: None such
     * 14    Not used                       / DSi9: NDS-Slot Card change?
     * 15    Not used                       / DSi: dito for 2nd NDS-Slot?
     * 16    IPC Sync
     * 17    IPC Send FIFO Empty
     * 18    IPC Recv FIFO Not Empty
     * 19    NDS-Slot Game Card Data Transfer Completion
     * 20    NDS-Slot Game Card IREQ_MC
     * 21    NDS9 only: Geometry Command FIFO
     * 22    NDS7 only: Screens unfolding
     * 23    NDS7 only: SPI bus
     * 24    NDS7 only: Wifi    / DSi9: XpertTeak DSP
     * 25    Not used           / DSi9: Camera
     * 26    Not used           / DSi9: Undoc, IF.26 set on FFh-filling 40021Axh
     * 27    Not used           / DSi:  Maybe IREQ_MC for 2nd gamecard?
     * 28    Not used           / DSi: NewDMA0
     * 29    Not used           / DSi: NewDMA1
     * 30    Not used           / DSi: NewDMA2
     * 31    Not used           / DSi: NewDMA3
     */
    REG_RW(0x04000210, u32, IE)

    /**
     * Interrupt Request Flags
     */
    REG_RW(0x04000214, u32, IF)

    #ifdef ARM7
    /**
     * Interrupt Enable
     * DSi only (additional ARM7 interrupt sources)
     */
    REG_RW(0x04000218, u32, IE2)

    /**
     * Interrupt Request Flags
     * DSi only (additional ARM7 interrupt sources)
     */
    REG_RW(0x0400021C, u32, IF2)
    #endif
    #endif

    // ARM7 Memory and IRQ Control
    #ifdef ARM7
    /**
     * Wifi Waitstate Control
     *
     * Bit   Expl.
     * 0-2   Wifi WS0 Control (0-7) (Ports 4800000h-4807FFFh)
     * 3-5   Wifi WS1 Control (0-7) (Ports 4808000h-480FFFFh)
     * 4-15  Not used (zero)
     *
     * This register is initialized by firmware on power-up, don't change.
     * Note: WIFIWAITCNT can be accessed only when enabled in POWCNT2.
     */
    REG_RW(0x04000206, u16, WIFIWAITCNT)

    /**
     * Low Power Mode Control
     *
     * Bit   Expl.
     * 0-5   Not used (zero)
     * 6-7   Power Down Mode  (0=No function, 1=Enter GBA Mode, 2=Halt, 3=Sleep)
     *
     * In Halt mode, the CPU is paused as long as (IE AND IF)=0.
     * In Sleep mode, most of the hardware including sound and video are paused, this very-low-power mode could be used much like a screensaver.
     * The HALTCNT register should not be accessed directly. Instead, the BIOS Halt, Sleep, CustomHalt, IntrWait, or VBlankIntrWait SWI functions
     * should be used.
     */
    REG_RW(0x04000301, u8, HALTCNT)

    /**
     * Sound/Wifi Power Control Register
     *
     * Bit   Expl.
     * 0     Sound Speakers (0=Disable, 1=Enable) (Initial setting = 1)
     * 1     Wifi           (0=Disable, 1=Enable) (Initial setting = 0)
     * 2-31  Not used
     *
     * Note: Bit0 disables the internal Speaker only, headphones are not disabled.
     * Bit1 disables Port 4000206h, and Ports 4800000h-480FFFFh.
     */
    REG_RW(0x04000304, u16, POWCNT2)

    /**
     * Bios-data-read-protection address
     *
     * Opcodes at...      Can read from      Expl.
     * 0..[BIOSPROT]-1    0..3FFFh           Double-protected (when BIOSPROT is set)
     * [BIOSPROT]..3FFFh  [BIOSPROT]..3FFFh  Normal-protected (always active)
     *
     * Used to double-protect the first some KBytes of the NDS7 BIOS. The BIOS is split into two protection regions, one always active, one
     * controlled by the BIOSPROT register. The overall idea is that only the BIOS can read from itself, any other attempts to read from that
     * regions return FFh-bytes.
     *
     * The initial BIOSPROT setting on power-up is zero (disabled). Before starting the cartridge, the BIOS boot code sets the register to 1204h
     * (actually 1205h, but the mis-aligned low-bit is ignored). Once when initialized, further writes to the register are ignored.
     *
     * The double-protected region contains the exception vectors, some bytes of code, and the cartridge KEY1 encryption seed (about 4KBytes). As
     * far as I know, it is impossible to unlock the memory once when it is locked, however, with some trickery, it is possible execute code
     * before it gets locked. Also, the two THUMB opcodes at 05ECh can be used to read all memory at 0..3FFFh,
     *
     * 05ECh  ldrb r3,[r3,12h]      ;requires incoming r3=src-12h
     * 05EEh  pop  r2,r4,r6,r7,r15  ;requires dummy values & THUMB retadr on stack
     *
     * Additionally most BIOS functions (eg. CpuSet), include a software-based protection which rejects source addresses in the BIOS area (the
     * only exception is GetCRC16, though it still cannot bypass the BIOSPROT setting).
     */
    REG_R(0x04000308, u32, BIOSPROT)
    #endif

    // NDS VRAM
    #ifdef ARM7
    /**
     * The possible values for VRAMSTAT.
     *
     * These indicate which VRAM banks are accessible to the ARM7.
     */
    enum VRAMSTAT_Value: u8 {
        NONE = 0,
        C = 1,
        D = 2,
        C_D = 3
    };

    /**
     * VRAMSTAT - 8bit - VRAM Bank Status (R)
     *
     *   0     VRAM C enabled and allocated to NDS7  (0=No, 1=Yes)
     *   1     VRAM D enabled and allocated to NDS7  (0=No, 1=Yes)
     *   2-7   Not used (always zero)
     *
     * The register indicates if VRAM C/D are allocated to NDS7 (as Work RAM), ie. if VRAMCNT_C/D are enabled (Bit7=1), with MST=2 (Bit0-2). However,
     * it does not reflect the OFS value.
     */
    REG_R(0x04000240, VRAMSTAT_Value, VRAMSTAT)
    #endif

    #ifdef ARM9
    constexpr u8 VRAM_ENABLE = 0x80;

    /**
     * The different modes for VRAM bank A.
     */
    _ENUM_VOLATILE enum VramModeA: u8 {
        A_Disable     = 0,
        A_Direct      = VRAM_ENABLE,
        A_Background0 = VRAM_ENABLE | 1 | (0 << 3),
        A_Background1 = VRAM_ENABLE | 1 | (1 << 3),
        A_Background2 = VRAM_ENABLE | 1 | (2 << 3),
        A_Background3 = VRAM_ENABLE | 1 | (3 << 3),
        A_Object0     = VRAM_ENABLE | 2 | (0 << 3),
        A_Object1     = VRAM_ENABLE | 2 | (1 << 3),
        A_Texture0    = VRAM_ENABLE | 3 | (0 << 3),
        A_Texture1    = VRAM_ENABLE | 3 | (1 << 3),
        A_Texture2    = VRAM_ENABLE | 3 | (2 << 3),
        A_Texture3    = VRAM_ENABLE | 3 | (3 << 3)
    };

    /**
     * The different modes for VRAM bank B.
     */
    _ENUM_VOLATILE enum VramModeB: u8 {
        B_Disable     = 0,
        B_Direct      = VRAM_ENABLE,
        B_Background0 = VRAM_ENABLE | 1 | (0 << 3),
        B_Background1 = VRAM_ENABLE | 1 | (1 << 3),
        B_Background2 = VRAM_ENABLE | 1 | (2 << 3),
        B_Background3 = VRAM_ENABLE | 1 | (3 << 3),
        B_Object0     = VRAM_ENABLE | 2 | (0 << 3),
        B_Object1     = VRAM_ENABLE | 2 | (1 << 3),
        B_Texture0    = VRAM_ENABLE | 3 | (0 << 3),
        B_Texture1    = VRAM_ENABLE | 3 | (1 << 3),
        B_Texture2    = VRAM_ENABLE | 3 | (2 << 3),
        B_Texture3    = VRAM_ENABLE | 3 | (3 << 3)
    };

    /**
     * The different modes for VRAM bank C.
     */
    _ENUM_VOLATILE enum VramModeC: u8 {
        C_Disable     = 0,
        C_Direct      = VRAM_ENABLE,
        C_Background0 = VRAM_ENABLE | 1 | (0 << 3),
        C_Background1 = VRAM_ENABLE | 1 | (1 << 3),
        C_Background2 = VRAM_ENABLE | 1 | (2 << 3),
        C_Background3 = VRAM_ENABLE | 1 | (3 << 3),
        C_Texture0    = VRAM_ENABLE | 3 | (0 << 3),
        C_Texture1    = VRAM_ENABLE | 3 | (1 << 3),
        C_Texture2    = VRAM_ENABLE | 3 | (2 << 3),
        C_Texture3    = VRAM_ENABLE | 3 | (3 << 3),
        C_BackgroundB = VRAM_ENABLE | 4,
        C_ARM7        = VRAM_ENABLE | 2
    };

    /**
     * The different modes for VRAM bank D.
     */
    _ENUM_VOLATILE enum VramModeD: u8 {
        D_Disable     = 0,
        D_Direct      = VRAM_ENABLE,
        D_Background0 = VRAM_ENABLE | 1 | (0 << 3),
        D_Background1 = VRAM_ENABLE | 1 | (1 << 3),
        D_Background2 = VRAM_ENABLE | 1 | (2 << 3),
        D_Background3 = VRAM_ENABLE | 1 | (3 << 3),
        D_Texture0    = VRAM_ENABLE | 3 | (0 << 3),
        D_Texture1    = VRAM_ENABLE | 3 | (1 << 3),
        D_Texture2    = VRAM_ENABLE | 3 | (2 << 3),
        D_Texture3    = VRAM_ENABLE | 3 | (3 << 3),
        D_ObjectB     = VRAM_ENABLE | 4,
        D_ARM7        = VRAM_ENABLE | 2
    };

    /**
     * The different modes for VRAM bank E.
     */
    _ENUM_VOLATILE enum VramModeE: u8 {
        E_Disable             = 0,
        E_Direct              = VRAM_ENABLE,
        E_Background          = VRAM_ENABLE | 1,
        E_Object              = VRAM_ENABLE | 2,
        E_Background_Extended = VRAM_ENABLE | 4,
        E_Texture_Palette     = VRAM_ENABLE | 3
    };

    /**
     * The different modes for VRAM bank F.
     */
    _ENUM_VOLATILE enum VramModeF: u8 {
        F_Disable              = 0,
        F_Direct               = VRAM_ENABLE,
        F_Background0          = VRAM_ENABLE | 1 | (0 << 3),
        F_Background1          = VRAM_ENABLE | 1 | (1 << 3),
        F_Background2          = VRAM_ENABLE | 1 | (2 << 3),
        F_Background3          = VRAM_ENABLE | 1 | (3 << 3),
        F_Object0              = VRAM_ENABLE | 2 | (0 << 3),
        F_Object1              = VRAM_ENABLE | 2 | (1 << 3),
        F_Object2              = VRAM_ENABLE | 2 | (2 << 3),
        F_Object3              = VRAM_ENABLE | 2 | (3 << 3),
        F_Background_Extended0 = VRAM_ENABLE | 4 | (0 << 3),
        F_Background_Extended1 = VRAM_ENABLE | 4 | (1 << 3),
        F_Object_Extended0     = VRAM_ENABLE | 5 | (0 << 3),
        F_Object_Extended1     = VRAM_ENABLE | 5 | (1 << 3),
        F_Texture_Palette0     = VRAM_ENABLE | 3 | (0 << 3),
        F_Texture_Palette1     = VRAM_ENABLE | 3 | (1 << 3),
        F_Texture_Palette2     = VRAM_ENABLE | 3 | (2 << 3),
        F_Texture_Palette3     = VRAM_ENABLE | 3 | (3 << 3)
    };

    /**
     * The different modes for VRAM bank G.
     */
    _ENUM_VOLATILE enum VramModeG: u8 {
        G_Disable              = 0,
        G_Direct               = VRAM_ENABLE,
        G_Background0          = VRAM_ENABLE | 1 | (0 << 3),
        G_Background1          = VRAM_ENABLE | 1 | (1 << 3),
        G_Background2          = VRAM_ENABLE | 1 | (2 << 3),
        G_Background3          = VRAM_ENABLE | 1 | (3 << 3),
        G_Object0              = VRAM_ENABLE | 2 | (0 << 3),
        G_Object1              = VRAM_ENABLE | 2 | (1 << 3),
        G_Object2              = VRAM_ENABLE | 2 | (2 << 3),
        G_Object3              = VRAM_ENABLE | 2 | (3 << 3),
        G_Background_Extended0 = VRAM_ENABLE | 4 | (0 << 3),
        G_Background_Extended1 = VRAM_ENABLE | 4 | (1 << 3),
        G_Object_Extended0     = VRAM_ENABLE | 5 | (0 << 3),
        G_Object_Extended1     = VRAM_ENABLE | 5 | (1 << 3),
        G_Texture_Palette0     = VRAM_ENABLE | 3 | (0 << 3),
        G_Texture_Palette1     = VRAM_ENABLE | 3 | (1 << 3),
        G_Texture_Palette2     = VRAM_ENABLE | 3 | (2 << 3),
        G_Texture_Palette3     = VRAM_ENABLE | 3 | (3 << 3)
    };

    /**
     * The different modes for VRAM bank H.
     */
    _ENUM_VOLATILE enum VramModeH: u8 {
        H_Disable              = 0,
        H_Direct               = VRAM_ENABLE,
        H_BackgroundB          = VRAM_ENABLE | 1,
        H_BackgroundB_Extended = VRAM_ENABLE | 2
    };

    /**
     * The different modes for VRAM bank I.
     */
    _ENUM_VOLATILE enum VramModeI: u8 {
        I_Disable          = 0,
        I_Direct           = VRAM_ENABLE,
        I_BackgroundB      = VRAM_ENABLE | 1,
        I_ObjectB          = VRAM_ENABLE | 2,
        I_ObjectB_Extended = VRAM_ENABLE | 3
    };

    /**
     *   0-2   VRAM MST              ;Bit2 not used by VRAM-A,B,H,I
     *   3-4   VRAM Offset (0-3)     ;Offset not used by VRAM-E,H,I
     *   5-6   Not used
     *   7     VRAM Enable (0=Disable, 1=Enable)
     *
     * There is a total of 656KB of VRAM in Blocks A-I.
     * Table below shows the possible configurations.
     *
     *   VRAM    SIZE  MST  OFS   ARM9, Plain ARM9-CPU Access (so-called LCDC mode)
     *   A       128K  0    -     6800000h-681FFFFh
     *   B       128K  0    -     6820000h-683FFFFh
     *   C       128K  0    -     6840000h-685FFFFh
     *   D       128K  0    -     6860000h-687FFFFh
     *   E       64K   0    -     6880000h-688FFFFh
     *   F       16K   0    -     6890000h-6893FFFh
     *   G       16K   0    -     6894000h-6897FFFh
     *   H       32K   0    -     6898000h-689FFFFh
     *   I       16K   0    -     68A0000h-68A3FFFh
     *   VRAM    SIZE  MST  OFS   ARM9, 2D Graphics Engine A, BG-VRAM (max 512K)
     *   A,B,C,D 128K  1    0..3  6000000h+(20000h*OFS)
     *   E       64K   1    -     6000000h
     *   F,G     16K   1    0..3  6000000h+(4000h*OFS.0)+(10000h*OFS.1)
     *   VRAM    SIZE  MST  OFS   ARM9, 2D Graphics Engine A, OBJ-VRAM (max 256K)
     *   A,B     128K  2    0..1  6400000h+(20000h*OFS.0)  ;(OFS.1 must be zero)
     *   E       64K   2    -     6400000h
     *   F,G     16K   2    0..3  6400000h+(4000h*OFS.0)+(10000h*OFS.1)
     *   VRAM    SIZE  MST  OFS   2D Graphics Engine A, BG Extended Palette
     *   E       64K   4    -     Slot 0-3  ;only lower 32K used
     *   F,G     16K   4    0..1  Slot 0-1 (OFS=0), Slot 2-3 (OFS=1)
     *   VRAM    SIZE  MST  OFS   2D Graphics Engine A, OBJ Extended Palette
     *   F,G     16K   5    -     Slot 0  ;16K each (only lower 8K used)
     *   VRAM    SIZE  MST  OFS   Texture/Rear-plane Image
     *   A,B,C,D 128K  3    0..3  Slot OFS(0-3)   ;(Slot2-3: Texture, or Rear-plane)
     *   VRAM    SIZE  MST  OFS   Texture Palette
     *   E       64K   3    -     Slots 0-3                 ;OFS=don't care
     *   F,G     16K   3    0..3  Slot (OFS.0*1)+(OFS.1*4)  ;ie. Slot 0, 1, 4, or 5
     *   VRAM    SIZE  MST  OFS   ARM9, 2D Graphics Engine B, BG-VRAM (max 128K)
     *   C       128K  4    -     6200000h
     *   H       32K   1    -     6200000h
     *   I       16K   1    -     6208000h
     *   VRAM    SIZE  MST  OFS   ARM9, 2D Graphics Engine B, OBJ-VRAM (max 128K)
     *   D       128K  4    -     6600000h
     *   I       16K   2    -     6600000h
     *   VRAM    SIZE  MST  OFS   2D Graphics Engine B, BG Extended Palette
     *   H       32K   2    -     Slot 0-3
     *   VRAM    SIZE  MST  OFS   2D Graphics Engine B, OBJ Extended Palette
     *   I       16K   3    -     Slot 0  ;(only lower 8K used)
     *   VRAM    SIZE  MST  OFS   <ARM7>, Plain <ARM7>-CPU Access
     *   C,D     128K  2    0..1  6000000h+(20000h*OFS.0)  ;OFS.1 must be zero
     *
     * Notes
     * In Plain-CPU modes, VRAM can be accessed only by the CPU (and by the Capture Unit, and by VRAM Display mode). In "Plain <ARM7>-CPU Access"
     * mode, the VRAM blocks are allocated as Work RAM to the NDS7 CPU.
     * In BG/OBJ VRAM modes, VRAM can be accessed by the CPU at specified addresses, and by the display controller.
     * In Extended Palette and Texture Image/Palette modes, VRAM is not mapped to CPU address space, and can be accessed only by the display
     * controller (so, to initialize or change the memory, it should be temporarily switched to Plain-CPU mode).
     * All VRAM (and Palette, and OAM) can be written to only in 16bit and 32bit units (STRH, STR opcodes), 8bit writes are ignored (by STRB
     * opcode). The only exception is "Plain <ARM7>-CPU Access" mode: The ARM7 CPU can use STRB to write to VRAM (the reason for this special
     * feature is that, in GBA mode, two 128K VRAM blocks are used to emulate the GBA's 256K Work RAM).
     *
     * Other Video RAM
     * Aside from the map-able VRAM blocks, there are also some video-related memory regions at fixed addresses:
     *
     *   5000000h Engine A Standard BG Palette (512 bytes)
     *   5000200h Engine A Standard OBJ Palette (512 bytes)
     *   5000400h Engine B Standard BG Palette (512 bytes)
     *   5000600h Engine B Standard OBJ Palette (512 bytes)
     *   7000000h Engine A OAM (1024 bytes)
     *   7000400h Engine B OAM (1024 bytes)
     */
    REG_W(0x04000240, VramModeA, VRAMCNT_A)
    REG_W(0x04000241, VramModeB, VRAMCNT_B)
    REG_W(0x04000242, VramModeC, VRAMCNT_C)
    REG_W(0x04000243, VramModeD, VRAMCNT_D)
    REG_W(0x04000244, VramModeE, VRAMCNT_E)
    REG_W(0x04000245, VramModeF, VRAMCNT_F)
    REG_W(0x04000246, VramModeG, VRAMCNT_G)
    REG_W(0x04000248, VramModeH, VRAMCNT_H)
    REG_W(0x04000249, VramModeI, VRAMCNT_I)
    #endif

    // NDS 32K shared WRAM
    /**
     * The different ways that the 32K NDS shared-WRAM can be mapped between the ARM9 and ARM7 processors.
     */
    #ifdef ARM9
    enum WRAMCNT_Value: u8 {
        #else
        enum WRAMSTAT_Value: u8 {
        #endif
        /**
         * ARM9 area contains all 32K of shared-WRAM (plus mirrors).
         * ARM7 area contains mirrors of ARM7-WRAM.
         */
            ARM9_32K_ARM7_0K      = 0,
        /**
         * ARM9 area contains the 2nd 16K of shared-WRAM (plus mirrors).
         * ARM7 area contains the 1st 16K of shared-WRAM (plus mirrors).
         */
            ARM9_16K_2_ARM7_16K_1 = 1,
        /**
         * ARM9 area contains the 1st 16K of shared-WRAM (plus mirrors).
         * ARM7 area contains the 2nd 16K of shared-WRAM (plus mirrors).
         */
            ARM9_16K_1_ARM7_16K_2 = 2,
        /**
         * ARM9 area contains undefined data.
         * ARM7 area contains all 32K of shared-WRAM (plus mirrors).
         */
            ARM9_0K_ARM7_32K      = 3
    };

    /**
     * NDS9 - WRAMCNT - 8bit - WRAM Bank Control
     * NDS7 - WRAMSTAT - 8bit - WRAM Bank Status
     *
     * Should not be changed when using Nintendo's API.
     *
     *   0-1   ARM9/ARM7 (0-3 = 32K/0K, 2nd 16K/1st 16K, 1st 16K/2nd 16K, 0K/32K)
     *   2-7   Not used
     *
     * The ARM9 WRAM area is 3000000h-3FFFFFFh (16MB range).
     * The ARM7 WRAM area is 3000000h-37FFFFFh (8MB range).
     * The allocated 16K or 32K are mirrored everywhere in the above areas.
     * De-allocation (0K) is a special case: At the ARM9-side, the WRAM area is then empty (containing undefined data). At the ARM7-side, the WRAM
     * area is then containing mirrors of the 64KB ARM7-WRAM (the memory at 3800000h and up).
     */
    #ifdef ARM9
    REG_RW(0x04000247, WRAMCNT_Value, WRAMCNT)
    #else
    REG_RW(0x04000241, WRAMSTAT_Value, WRAMSTAT)
    #endif

    // DSi WRAM registers
    /**
     * MBK9, WRAM-A/B/C Slot Write Protect
     *
     *   0-3   WRAM-A, Port 4004040h-4004043h Write (0=Writeable by ARM9, 1=Read-only)
     *   4-7   Unknown/Unused (0)
     *   8-15  WRAM-B, Port 4004044h-400404Bh Write (0=Writeable by ARM9, 1=Read-only)
     *   16-23 WRAM-C, Port 400404Ch-4004053h Write (0=Writeable by ARM9, 1=Read-only)
     *   24-31 Unknown/Unused (0)   ;but, carthdr has nonzero data for it?
     *
     * Selects whether ARM9 may write to WRAM slot registers at 4004040h-4004053h (in Read-only mode neither ARM7 nor ARM9 can write to those
     * registers; that applies only to that registers, ie. the memory itself isn't write-protected).
     */
    #ifdef ARM9
    REG_R(0x04004060, u32, MKB9)
    #else
    REG_RW(0x04004060, u32, MKB9)
    #endif

    /**
     * MKB1.(0-3), WRAM-A(0-3) - 64K, mappable to ARM7, or ARM9.
     *
     * Read-only on ARM7, read-only or read-write on ARM9 depending on current setting of MKB9.
     *
     *   0    Master (0=ARM9, 1=ARM7)
     *   1    Not used
     *   2-3  Offset (0..3) (slot 0..3) (LSB of address in 64Kbyte units)
     *   4-6  Not used
     *   7    Enable (0=Disable, 1=Enable)
     */
    #ifdef ARM9
    REG_RW(0x04004040, u8, MKB1_0)
    REG_RW(0x04004041, u8, MKB1_1)
    REG_RW(0x04004042, u8, MKB1_2)
    REG_RW(0x04004043, u8, MKB1_3)
    #else
    REG_R(0x04004040, u8, MKB1_0)
    REG_R(0x04004041, u8, MKB1_1)
    REG_R(0x04004042, u8, MKB1_2)
    REG_R(0x04004043, u8, MKB1_3)
    #endif

    /**
     * MKB(2-3).(0-3), WRAM-B(0-7) - 32K, mappable to ARM7, ARM9, or DSP/code.
     *
     * Read-only on ARM7, read-only or read-write on ARM9 depending on current setting of MKB9.
     *
     *   0-1  Master (0=ARM9, 1=ARM7, 2 or 3=DSP/code)
     *   2-4  Offset (0..7) (slot 0..7) (LSB of address in 32Kbyte units)
     *   5-6  Not used (0)
     *   7    Enable (0=Disable, 1=Enable)
     */
    #ifdef ARM9
    REG_RW(0x04004044, u8, MKB2_0)
    REG_RW(0x04004045, u8, MKB2_1)
    REG_RW(0x04004046, u8, MKB2_2)
    REG_RW(0x04004047, u8, MKB2_3)
    REG_RW(0x04004048, u8, MKB3_0)
    REG_RW(0x04004049, u8, MKB3_1)
    REG_RW(0x0400404A, u8, MKB3_2)
    REG_RW(0x0400404B, u8, MKB3_3)
    #else
    REG_R(0x04004044, u8, MKB2_0)
    REG_R(0x04004045, u8, MKB2_1)
    REG_R(0x04004046, u8, MKB2_2)
    REG_R(0x04004047, u8, MKB2_3)
    REG_R(0x04004048, u8, MKB3_0)
    REG_R(0x04004049, u8, MKB3_1)
    REG_R(0x0400404A, u8, MKB3_2)
    REG_R(0x0400404B, u8, MKB3_3)
    #endif

    /**
     * MKB(4-5).(0-3), WRAM-C(0-7) - 32K, mappable to ARM7, ARM9, or DSP/data.
     *
     * Read-only on ARM7, read-only or read-write on ARM9 depending on current setting of MKB9.
     *
     *   0-1  Master (0=ARM9, 1=ARM7, 2 or 3=DSP/data)
     *   2-4  Offset (0..7) (slot 0..7) (LSB of address in 32Kbyte units)
     *   5-6  Not used (0)
     *   7    Enable (0=Disable, 1=Enable)
     */
    #ifdef ARM9
    REG_RW(0x0400404C, u8, MKB4_0)
    REG_RW(0x0400404D, u8, MKB4_1)
    REG_RW(0x0400404E, u8, MKB4_2)
    REG_RW(0x0400404F, u8, MKB4_3)
    REG_RW(0x04004050, u8, MKB5_0)
    REG_RW(0x04004051, u8, MKB5_1)
    REG_RW(0x04004052, u8, MKB5_2)
    REG_RW(0x04004053, u8, MKB5_3)
    #else
    REG_R(0x0400404C, u8, MKB4_0)
    REG_R(0x0400404D, u8, MKB4_1)
    REG_R(0x0400404E, u8, MKB4_2)
    REG_R(0x0400404F, u8, MKB4_3)
    REG_R(0x04004050, u8, MKB5_0)
    REG_R(0x04004051, u8, MKB5_1)
    REG_R(0x04004052, u8, MKB5_2)
    REG_R(0x04004053, u8, MKB5_3)
    #endif

    /**
     * MBK6, WRAM-A, 64K..256K mapping.
     *
     * The ARM7 register is separate from the ARM9 register.
     * > MBK6-8 exist as separate READ/WRITE-ABLE registers on ARM7 and ARM9 side (making it six registers in total).
     *
     *   0-3   Not used (0)
     *   4-11  Start Address (3000000h+N*10000h)     ;=3000000h..3FF0000h
     *   12-13 Image Size (0 or 1=64KB/Slot0, 2=128KB/Slot0+1+2??, 3=256KB/Slot0..3)
     *   14-19 Not used (0)
     *   20-28 End Address   (3000000h+N*10000h-1)   ;=2FFFFFFh..4FEFFFFh
     *   29-31 Not used (0)
     */
    REG_RW(0x04004054, u32, MKB6)

    /**
     * MBK7, WRAM-B.
     *
     * The ARM7 register is separate from the ARM9 register.
     * > MBK6-8 exist as separate READ/WRITE-ABLE registers on ARM7 and ARM9 side (making it six registers in total).
     *
     *   0-2   Not used (0)
     *   3-11  Start Address (3000000h+N*8000h)      ;=3000000h..3FF8000h
     *   12-13 Image Size (0=32K/Slot0,1=64KB/Slot0-1,2=128KB/Slot0-3,3=256KB/Slot0-7)
     *   14-18 Not used (0)
     *   19-28 End Address   (3000000h+N*8000h-1)    ;=2FFFFFFh..4FF7FFFh
     *   29-31 Not used (0)
     */
    REG_RW(0x04004058, u32, MKB7)

    /**
     * MBK8, WRAM-C.
     *
     * The ARM7 register is separate from the ARM9 register.
     * > MBK6-8 exist as separate READ/WRITE-ABLE registers on ARM7 and ARM9 side (making it six registers in total).
     *
     *   0-2   Not used (0)
     *   3-11  Start Address (3000000h+N*8000h)      ;=3000000h..3FF8000h
     *   12-13 Image Size (0=32K/Slot0,1=64KB/Slot0-1,2=128KB/Slot0-3,3=256KB/Slot0-7)
     *   14-18 Not used (0)
     *   19-28 End Address   (3000000h+N*8000h-1)    ;=2FFFFFFh..4FF7FFFh
     *   29-31 Not used (0)
     */
    REG_RW(0x0400405C, u32, MKB8)

    //TODO: there are a LOT more registers that need adding, and more documentation that should be put into the comments for the registers that are already there
}

#undef BLOCK_RW
#undef BLOCK_W
#undef BLOCK_R

#undef REG_RW
#undef REG_W
#undef REG_R

#endif //LIBDSI_REGISTERS_H
