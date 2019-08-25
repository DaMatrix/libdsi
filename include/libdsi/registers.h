#ifndef LIBDSI_REGISTERS_H
#define LIBDSI_REGISTERS_H

#include <libdsi/dsitypes.h>

/**
 * Definitions of all NDS registers, along with some basic documentation.
 *
 * Mostly copied from gbatek (https://problemkaputt.de/gbatek.htm), with some adaptations.
 */
namespace libdsi::reg {
    #define REG_R(ADDRESS, TYPE, NAME) constexpr TYPE NAME() { return *((TYPE* ) (ADDRESS)); }
    #define REG_W(ADDRESS, TYPE, NAME) constexpr TYPE NAME(TYPE value) { return *((TYPE* ) (ADDRESS)) = value; }
    #define REG_RW(ADDRESS, TYPE, NAME) REG_R(ADDRESS, TYPE, NAME) REG_W(ADDRESS, TYPE, NAME)

    /**
     * 2D Engine A+B - DISPSTAT - General LCD Status (Read/Write)
     */
    REG_RW(0x4000004, u16, DISPSTAT)
    /**
     * 2D Engine A+B - VCOUNT - Vertical Counter (Read only)
     */
    REG_R(0x4000006, u16, VCOUNT)

    // ARM9 Display Engine A
    #ifdef ARM9
    /**
     * 2D Engine A - DISPCNT - LCD Control (Read/Write)
     */
    REG_RW(0x4000000, u32, DISPCNT_A)
    /**
     * BG0 Control
     */
    REG_RW(0x4000008, u16, BG0CNT_A)
    /**
     * BG1 Control
     */
    REG_RW(0x40000A0, u16, BG1CNT_A)
    /**
     * BG2 Control
     */
    REG_RW(0x40000C0, u16, BG2CNT_A)
    /**
     * BG3 Control
     */
    REG_RW(0x40000E0, u16, BG3CNT_A)
    /**
     * BG0 X-Offset
     */
    REG_W(0x4000010, u16, BG0HOFS_A)
    /**
     * BG0 Y-Offset
     */
    REG_W(0x4000012, u16, BG0VOFS_A)
    /**
     * BG1 X-Offset
     */
    REG_W(0x4000014, u16, BG1HOFS_A)
    /**
     * BG1 Y-Offset
     */
    REG_W(0x4000016, u16, BG1VOFS_A)
    /**
     * BG2 X-Offset
     */
    REG_W(0x4000018, u16, BG2HOFS_A)
    /**
     * BG2 Y-Offset
     */
    REG_W(0x400001A, u16, BG2VOFS_A)
    /**
     * BG3 X-Offset
     */
    REG_W(0x400001C, u16, BG3HOFS_A)
    /**
     * BG3 Y-Offset
     */
    REG_W(0x400001E, u16, BG3VOFS_A)
    /**
     * BG2 Rotation/Scaling Parameter A (dx)
     */
    REG_W(0x4000020, u16, BG2PA_A)
    /**
     * BG2 Rotation/Scaling Parameter B (dmx)
     */
    REG_W(0x4000022, u16, BG2PB_A)
    /**
     * BG2 Rotation/Scaling Parameter C (dy)
     */
    REG_W(0x4000024, u16, BG2PC_A)
    /**
     * BG2 Rotation/Scaling Parameter D (dmy)
     */
    REG_W(0x4000026, u16, BG2PD_A)
    /**
     * BG2 Reference Point X-Coordinate
     */
    REG_W(0x4000028, u32, BG2X_A)
    /**
     * BG2 Reference Point Y-Coordinate
     */
    REG_W(0x400002C, u32, BG2Y_A)
    /**
     * BG3 Rotation/Scaling Parameter A (dx)
     */
    REG_W(0x4000030, u16, BG3PA_A)
    /**
     * BG3 Rotation/Scaling Parameter B (dmx)
     */
    REG_W(0x4000032, u16, BG3PB_A)
    /**
     * BG3 Rotation/Scaling Parameter C (dy)
     */
    REG_W(0x4000034, u16, BG3PC_A)
    /**
     * BG3 Rotation/Scaling Parameter D (dmy)
     */
    REG_W(0x4000036, u16, BG3PD_A)
    /**
     * BG3 Reference Point X-Coordinate
     */
    REG_W(0x4000038, u32, BG3X_A)
    /**
     * BG3 Reference Point Y-Coordinate
     */
    REG_W(0x400003C, u32, BG3Y_A)
    /**
     * Window 0 Horizontal Dimensions
     */
    REG_W(0x4000040, u16, WIN0H_A)
    /**
     * Window 1 Horizontal Dimensions
     */
    REG_W(0x4000042, u16, WIN1H_A)
    /**
     * Window 0 Vertical Dimensions
     */
    REG_W(0x4000044, u16, WIN0V_A)
    /**
     * Window 1 Vertical Dimensions
     */
    REG_W(0x4000046, u16, WIN1V_A)
    /**
     * Inside of Window 0 and 1
     */
    REG_RW(0x4000048, u16, WININ_A)
    /**
     * Inside of OBJ Window & Outside of Windows
     */
    REG_RW(0x400004A, u16, WINOUT_A)
    /**
     * Mosaic Size
     */
    REG_W(0x400004C, u16, MOSAIC_A)
    /**
     * Color Special Effects Selection
     */
    REG_RW(0x4000050, u16, BLDCNT_A)
    /**
     * Alpha Blending Coefficients
     */
    REG_RW(0x4000052, u16, BLDALPHA_A)
    /**
     * Brightness (Fade-In/Out) Coefficient
     */
    REG_W(0x4000054, u16, BLDY_A)
    /**
     * 3D Display Control Register
     */
    REG_RW(0x4000060, u16, DISP3DCNT_A)
    /**
     * Display Capture Control Register
     */
    REG_RW(0x4000064, u32, DISPCAPCNT_A)
    /**
     * Main Memory Display FIFO
     */
    REG_RW(0x4000068, u32, DISP_MMEM_FIFO_A)
    /**
     * 2D Engine A - Master Brightness Up/Down
     */
    REG_RW(0x400006C, u16, MASTER_BRIGHT_A)
    #endif

    // ARM9 Display Engine B
    #ifdef ARM9
    //TODO: i'm not quite certain about these
    /**
     * 2D Engine B - DISPCNT - LCD Control (Read/Write)
     */
    REG_RW(0x4000000 + 0x1000, u32, DISPCNT_B)
    /**
     * BG0 Control
     */
    REG_RW(0x4000008 + 0x1008, u16, BG0CNT_B)
    /**
     * BG1 Control
     */
    REG_RW(0x40000A0 + 0x1008, u16, BG1CNT_B)
    /**
     * BG2 Control
     */
    REG_RW(0x40000C0 + 0x1008, u16, BG2CNT_B)
    /**
     * BG3 Control
     */
    REG_RW(0x40000E0 + 0x1008, u16, BG3CNT_B)
    /**
     * BG0 X-Offset
     */
    REG_W(0x4000010 + 0x1008, u16, BG0HOFS_B)
    /**
     * BG0 Y-Offset
     */
    REG_W(0x4000012 + 0x1008, u16, BG0VOFS_B)
    /**
     * BG1 X-Offset
     */
    REG_W(0x4000014 + 0x1008, u16, BG1HOFS_B)
    /**
     * BG1 Y-Offset
     */
    REG_W(0x4000016 + 0x1008, u16, BG1VOFS_B)
    /**
     * BG2 X-Offset
     */
    REG_W(0x4000018 + 0x1008, u16, BG2HOFS_B)
    /**
     * BG2 Y-Offset
     */
    REG_W(0x400001A + 0x1008, u16, BG2VOFS_B)
    /**
     * BG3 X-Offset
     */
    REG_W(0x400001C + 0x1008, u16, BG3HOFS_B)
    /**
     * BG3 Y-Offset
     */
    REG_W(0x400001E + 0x1008, u16, BG3VOFS_B)
    /**
     * BG2 Rotation/Scaling Parameter A (dx)
     */
    REG_W(0x4000020 + 0x1008, u16, BG2PA_B)
    /**
     * BG2 Rotation/Scaling Parameter B (dmx)
     */
    REG_W(0x4000022 + 0x1008, u16, BG2PB_B)
    /**
     * BG2 Rotation/Scaling Parameter C (dy)
     */
    REG_W(0x4000024 + 0x1008, u16, BG2PC_B)
    /**
     * BG2 Rotation/Scaling Parameter D (dmy)
     */
    REG_W(0x4000026 + 0x1008, u16, BG2PD_B)
    /**
     * BG2 Reference Point X-Coordinate
     */
    REG_W(0x4000028 + 0x1008, u32, BG2X_B)
    /**
     * BG2 Reference Point Y-Coordinate
     */
    REG_W(0x400002C + 0x1008, u32, BG2Y_B)
    /**
     * BG3 Rotation/Scaling Parameter A (dx)
     */
    REG_W(0x4000030 + 0x1008, u16, BG3PA_B)
    /**
     * BG3 Rotation/Scaling Parameter B (dmx)
     */
    REG_W(0x4000032 + 0x1008, u16, BG3PB_B)
    /**
     * BG3 Rotation/Scaling Parameter C (dy)
     */
    REG_W(0x4000034 + 0x1008, u16, BG3PC_B)
    /**
     * BG3 Rotation/Scaling Parameter D (dmy)
     */
    REG_W(0x4000036 + 0x1008, u16, BG3PD_B)
    /**
     * BG3 Reference Point X-Coordinate
     */
    REG_W(0x4000038 + 0x1008, u32, BG3X_B)
    /**
     * BG3 Reference Point Y-Coordinate
     */
    REG_W(0x400003C + 0x1008, u32, BG3Y_B)
    /**
     * Window 0 Horizontal Dimensions
     */
    REG_W(0x4000040 + 0x1008, u16, WIN0H_B)
    /**
     * Window 1 Horizontal Dimensions
     */
    REG_W(0x4000042 + 0x1008, u16, WIN1H_B)
    /**
     * Window 0 Vertical Dimensions
     */
    REG_W(0x4000044 + 0x1008, u16, WIN0V_B)
    /**
     * Window 1 Vertical Dimensions
     */
    REG_W(0x4000046 + 0x1008, u16, WIN1V_B)
    /**
     * Inside of Window 0 and 1
     */
    REG_RW(0x4000048 + 0x1008, u16, WININ_B)
    /**
     * Inside of OBJ Window & Outside of Windows
     */
    REG_RW(0x400004A + 0x1008, u16, WINOUT_B)
    /**
     * Mosaic Size
     */
    REG_W(0x400004C + 0x1008, u16, MOSAIC_B)
    /**
     * Color Special Effects Selection
     */
    REG_RW(0x4000050 + 0x1008, u16, BLDCNT_B)
    /**
     * Alpha Blending Coefficients
     */
    REG_RW(0x4000052 + 0x1008, u16, BLDALPHA_B)
    /**
     * Brightness (Fade-In/Out) Coefficient
     */
    REG_W(0x4000054 + 0x1008, u16, BLDY_B)
    /**
     * 2D Engine B - Master Brightness Up/Down
     */
    REG_RW(0x400006C + 0x1000, u16, MASTER_BRIGHT_B)
    #endif

    // ARM9 DMA and Timers
    #ifdef ARM9
    //TODO
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
    REG_R(0x4000130, u16, KEYINPUT)
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
    REG_RW(0x4000132, u16, KEYCNT)
    #endif

    // ARM9 IPC/ROM
    #ifdef ARM9
    /**
     * IPC Synchronize Register
     */
    REG_RW(0x4000180, u16, IPCSYNC)
    /**
     * IPC Fifo Control Register
     */
    REG_RW(0x4000184, u16, IPCFIFOCOUNT)
    /**
     * IPC Send Fifo
     */
    REG_RW(0x4000188, u32, IPCFIFOSEND)
    /**
     * Gamecard ROM and SPI Control
     */
    REG_RW(0x40001A0, u16, AUXSPICNT)
    /**
     * Gamecard SPI Bus Data/Strobe
     */
    REG_RW(0x40001A2, u16, AUXSPIDATA)
    /**
     * Gamecard bus timing/control
     */
    REG_RW(0x40001A4, u32, GAMECARD_BUS_TIMING_CONTROL)
    /**
     * Gamecard bus 8-byte command out
     */
    REG_RW(0x40001A8, u64, GAMECARD_BUS_CMD_OUT)
    /**
     * Gamecard Encryption Seed 0 Lower 32bit
     */
    REG_RW(0x40001B0, u32, GAMECARD_ENCRYPTION_SEED_0_LOWER)
    /**
     * Gamecard Encryption Seed 1 Lower 32bit
     */
    REG_RW(0x40001B4, u32, GAMECARD_ENCRYPTION_SEED_1_LOWER)
    /**
     * Gamecard Encryption Seed 0 Upper 7bit (bit7-15 unused)
     */
    REG_RW(0x40001B8, u16, GAMECARD_ENCRYPTION_SEED_0_UPPER)
    /**
     * Gamecard Encryption Seed 1 Upper 7bit (bit7-15 unused)
     */
    REG_RW(0x40001BA, u16, GAMECARD_ENCRYPTION_SEED_1_UPPER)
    #endif

    // ARM9 Memory and IRQ Control
    #ifdef ARM9
    /**
     * External Memory Control 
     */
    REG_RW(0x4000204, u16, EXMEMCNT)
    /**
     * Interrupt Master Enable 
     */
    REG_RW(0x4000208, u16, IME)
    /**
     * Interrupt Enable 
     */
    REG_RW(0x4000210, u16, IE)
    /**
     * Interrupt Request Flags 
     */
    REG_RW(0x4000214, u16, IF)
    /**
     * VRAM-A (128K) Bank Control 
     */
    REG_W(0x4000240, u8, VRAMCNT_A)
    /**
     * VRAM-B (128K) Bank Control 
     */
    REG_W(0x4000241, u8, VRAMCNT_B)
    /**
     * VRAM-C (128K) Bank Control 
     */
    REG_W(0x4000242, u8, VRAMCNT_C)
    /**
     * VRAM-D (128K) Bank Control 
     */
    REG_W(0x4000243, u8, VRAMCNT_D)
    /**
     * VRAM-E (64K) Bank Control 
     */
    REG_W(0x4000244, u8, VRAMCNT_E)
    /**
     * VRAM-F (16K) Bank Control 
     */
    REG_W(0x4000245, u8, VRAMCNT_F)
    /**
     * VRAM-G (16K) Bank Control 
     */
    REG_W(0x4000246, u8, VRAMCNT_G)
    /**
     * WRAM Bank Control 
     */
    REG_W(0x4000247, u8, WRAMCNT)
    /**
     * VRAM-H (32K) Bank Control 
     */
    REG_W(0x4000248, u8, VRAMCNT_H)
    /**
     * VRAM-I (16K) Bank Control 
     */
    REG_W(0x4000249, u8, VRAMCNT_I)
    #endif

    // ARM9 Math
    #ifdef ARM9
    /**
     * Division Control 
     */
    REG_RW(0x4000280, u8, DIVCNT)
    /**
     * Division Numerator 
     */
    REG_RW(0x4000290, u64, DIV_NUMER)
    /**
     * Division Denominator 
     */
    REG_RW(0x4000298, u64, DIV_DENOM)
    /**
     * Division Quotient (=Numer/Denom) 
     */
    REG_R(0x40002A0, u64, DIV_RESULT)
    /**
     * Division Remainder (=Numer MOD Denom) 
     */
    REG_R(0x40002A8, u64, DIVREM_RESULT)
    /**
     * Square Root Control 
     */
    REG_RW(0x40002B0, u8, SQRTCNT)
    /**
     * Square Root Result 
     */
    REG_R(0x40002B4, u32, SQRT_RESULT)
    /**
     * Square Root Parameter Input 
     */
    REG_RW(0x40002B8, u64, SQRT_PARAM)
    /**
     * Undocumented
     */
    REG_R(0x4000300, u32, POSTFLG)
    /**
     * Graphics Power Control Register 
     */
    REG_RW(0x4000304, u8, POWCNT1)
    #endif

    // ARM9 3D Display Engine
    #ifdef ARM9
    //TODO
    #endif

    #undef REG_RW
    #undef REG_W
    #undef REG_R
}

#endif //LIBDSI_REGISTERS_H
