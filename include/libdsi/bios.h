#ifndef LIBDSI_DSIBIOS_H
#define LIBDSI_DSIBIOS_H

#include <libdsi/types.h>

namespace dsi::bios {
    namespace bitunpack {
        enum SourceWidth: u8 {
            Source_1bit = 1,
            Source_2bit = 2,
            Source_4bit = 4,
            Source_8bit = 8
        };

        enum DestinationWidth: u8 {
            Destination_1bit  = 1,
            Destination_2bit  = 2,
            Destination_4bit  = 4,
            Destination_8bit  = 8,
            Destination_16bit = 16
        };

        struct Information {
            const u16              sourceLength;
            const SourceWidth      sourceWidth;
            const DestinationWidth destinationWidth;
            const u32              dataOffset_zeroDataFlag;

            Information(u16 sourceLength, SourceWidth sourceWidth, DestinationWidth destinationWidth, u32 dataOffset, bool zeroDataFlag):
                    sourceLength(sourceLength),
                    sourceWidth(sourceWidth),
                    destinationWidth(destinationWidth),
                    dataOffset_zeroDataFlag((dataOffset & 0x7FFFFFFF) | (zeroDataFlag << 31)) {}
        };
    }

    /**
     * Stores the state for the DSi's BIOS SHA1 functions.
     */
    struct SHA1Context {
        u32 state[5];
        u32 length[2]; //in bits
        u8 buffer[0x40];
        u32 fragmentSize;
        void (*sha_block)(SHA1Context& ctx, void* source, u32 len);
    };

    /**
     * A struct containing pointer to callback functions used for reading data into BIOS "ByCallback" decompression functions.
     *
     * All functions may use ARM or THUMB code (indicated by address bit0). The current source address (r0) is passed to all callback functions.
     * Additionally, the initial destination address (r1), and a user defined parameter (r2) are passed to the Open function. For Huffman r2 must
     * point to a temp buffer (max 200h bytes needed, internally used by the SWI function to make a copy of the huffman tree; needed for
     * random-access to the tree, which wouldn't work with the sequentially reading callbacks).
     *
     * All functions have return values in r0. The Open function normally returns the first word (containing positive length and type),
     * alternatively it may return a negative error code to abort/reject decompression. The Close function, if it is defined, should return zero
     * (or any positive value), or a negative errorcode. The other functions return raw data, without errorcodes. The SWI returns the length of
     * decompressed data, or the signed errorcode from the Open/Close functions.
     */
    struct UnCompCallbacks {
            template<typename T> using Callback = T (*)(void* sourceAddress, void* initialDestinationAddress, u32 userParam);

        public:
            Callback<i32> open_and_get_32bit;
            Callback<i32> close;
            Callback<u8> get_8bit;
        private:
            Callback<u16> get_16bit; //never used
        public:
            Callback<u32> get_32bit; //only used by huffman

            UnCompCallbacks(Callback<i32> open_and_get_32bit, Callback<u8> get_8bit, Callback<u32> get_32bit):
                    open_and_get_32bit(open_and_get_32bit),
                    close(nullptr),
                    get_8bit(get_8bit),
                    get_16bit(nullptr),
                    get_32bit(get_32bit) {}

            UnCompCallbacks(Callback<i32> open_and_get_32bit, Callback<i32> close, Callback<u8> get_8bit, Callback<u32> get_32bit):
                    open_and_get_32bit(open_and_get_32bit),
                    close(close),
                    get_8bit(get_8bit),
                    get_16bit(nullptr),
                    get_32bit(get_32bit) {}
    };

    namespace _internal {
        extern "C" i32 _asm_div(i32 numerator, i32 denominator);
        extern "C" u16 _asm_sqrt(u32 number);
        extern "C" void _asm_bitUnPack(void* source, void* destination, bitunpack::Information* information);
        #ifdef ARM9
        extern "C" void _asm_diff8bitUnFilterWrite8bit(void* source, void* destination);
        extern "C" void _asm_diff16bitUnFilter(void* source, void* destination);
        #endif
        extern "C" void _asm_huffUnCompReadByCallback(void* source, void* destination, void* buffer, UnCompCallbacks* callbacks);
        extern "C" void _asm_lz77UnCompReadNormalWrite8bit(void* source, void* destination);
        extern "C" void _asm_lz77UnCompReadByCallbackWrite8bit(void* source, void* destination, u32 userParam, UnCompCallbacks* callbacks);
        extern "C" void _asm_lz77UnCompReadByCallbackWrite16bit(void* source, void* destination, u32 userParam, UnCompCallbacks* callbacks);
        extern "C" void _asm_rlUnCompReadNormalWrite8bit(void* source, void* destination);
        extern "C" void _asm_rlUnCompReadByCallbackWrite16bit(void* source, void* destination, u32 userParam, UnCompCallbacks* callbacks);
        extern "C" void _asm_cpuSet(void* source, void* destination, u32 length_mode);
        extern "C" void _asm_cpuFastSet(void* source, void* destination, u32 length_mode);
        extern "C" void _asm_halt();
        #ifdef ARM9
        extern "C" void _asm_intrWait(bool ignorePrior, u32 flags);
        #else
        extern "C" void _asm_intrWait(bool ignorePrior, u32 flags, u32 flags_extra);
        #endif
        extern "C" void _asm_vBlankIntrWait();
        #ifdef ARM7
        extern "C" void _asm_sleep();
        #endif
        extern "C" void _asm_waitByLoop(u32 delayCycles);
        extern "C" u16 _asm_getCRC16(u16 initial, void* source, u32 length);
        #ifdef ARM7
        extern "C" u16 _asm_getSineTable(u16 index);
        extern "C" u16 _asm_getPitchTable(u16 index);
        extern "C" u8 _asm_getVolumeTable(u16 index);
        extern "C" void _asm_soundBias(u16 biasLevel, u32 delayCount);
        #endif
        extern "C" void _asm_sha1_init(SHA1Context* context);
        extern "C" void _asm_sha1_update(SHA1Context* context, const void* source, u32 length);
        extern "C" void _asm_sha1_finish(void* digest, SHA1Context* context);
        extern "C" void _asm_sha1_init_update_finish(void* digest, const void* source, u32 length);
        extern "C" bool _asm_sha1_compare_20_bytes(const void* source1, const void* source2);
        extern "C" void _asm_sha1_init_update_finish_mess(void* digest, u32 digestLength, const void* source, u32 length);

        //TODO: implement RSA functions (or don't, it'd probably make more sense to use an actual library for that)
    }

    /**
     * Signed Division, r0/r1.
     *
     * r0  signed 32bit Number
     * r1  signed 32bit Denom
     *
     * Return:
     *
     * r0  Number DIV Denom ;signed
     * r1  Number MOD Denom ;signed
     * r3  ABS (Number DIV Denom) ;unsigned
     *
     * For example, incoming -1234, 10 should return -123, -4, +123.
     * The function usually gets caught in an endless loop upon division by zero.
     * Note: The NDS9 and DSi9 additionally support hardware division, by math coprocessor, accessed via I/O Ports, however, the SWI function is
     * a raw software division.
     */
    inline i32 div(i32 numerator, i32 denominator) { return _internal::_asm_div(numerator, denominator); }

    /**
     * Calculate square root.
     *
     * r0   unsigned 32bit number
     *
     * Return:
     *
     * r0   unsigned 16bit number
     *
     * The result is an integer value, so Sqrt(2) would return 1, to avoid this inaccuracy, shift left incoming number by 2*N as much as possible
     * (the result is then shifted left by 1*N). Ie. Sqrt(2 shl 30) would return 1.41421 shl 15.
     *
     * Note: The NDS9 and DSi9 additionally support hardware square root calculation, by math coprocessor, accessed via I/O Ports, however, the
     * SWI function is a raw software calculation.
     */
    inline u16 sqrt(u32 number) { return _internal::_asm_sqrt(number); }

    /**
     * Used to increase the color depth of bitmaps or tile data. For example, to convert a 1bit monochrome font into 4bit or 8bit GBA tiles. The
     * Unpack Info is specified separately, allowing to convert the same source data into different formats.
     *
     * r0  Source Address      (no alignment required)
     * r1  Destination Address (must be 32bit-word aligned)
     * r2  Pointer to UnPack information:
     *   16bit  Length of Source Data in bytes     (0-FFFFh)
     *   8bit   Width of Source Units in bits      (only 1,2,4,8 supported)
     *   8bit   Width of Destination Units in bits (only 1,2,4,8,16,32 supported)
     *   32bit  Data Offset (Bit 0-30), and Zero Data Flag (Bit 31)
     *
     * The Data Offset is always added to all non-zero source units.
     * If the Zero Data Flag was set, it is also added to zero units.
     */
    inline void bitUnPack(void* source, void* destination, bitunpack::Information* information) { _internal::_asm_bitUnPack(source, destination, information); }

    /**
     * Shortcut for {@link #bitUnpack(void*, void*, bitunpack::Information*)} which allocates a {@link bitunpack::Information} on the stack.
     */
    inline void bitUnPack(void* source, void* destination, u16 sourceLength, bitunpack::SourceWidth sourceWidth, bitunpack::DestinationWidth destinationWidth, u32 dataOffset, bool zeroDataFlag) {
        bitunpack::Information information(sourceLength, sourceWidth, destinationWidth, dataOffset, zeroDataFlag);
        bitUnPack(source, destination, &information);
    }

    #ifdef ARM9
    /**
     * These aren't actually real decompression functions, destination data will have exactly the same size as source data. However, assume a
     * bitmap or wave form to contain a stream of increasing numbers such like 10..19, the filtered/unfiltered data would be:
     *
     * unfiltered:   10  11  12  13  14  15  16  17  18  19
     * filtered:     10  +1  +1  +1  +1  +1  +1  +1  +1  +1
     *
     * In this case using filtered data (combined with actual compression algorithms) will obviously produce better compression results.
     * Data units may be either 8bit or 16bit used with Diff8bit or Diff16bit functions respectively.
     *
     * r0  Source address (must be aligned by 4) pointing to data as follows:
     *   Data Header (32bit)
     *     Bit 0-3   Data size (must be 1 for Diff8bit, 2 for Diff16bit)
     *     Bit 4-7   Type (must be 8 for DiffFiltered)
     *     Bit 8-31  24bit size after decompression
     *   Data Units (each 8bit or 16bit depending on used SWI function)
     *     Data0          ;original data
     *     Data1-Data0    ;difference data
     *     Data2-Data1    ;...
     *     Data3-Data2
     *     ...
     * r1  Destination address
     *
     * Return: No return value, Data written to destination address.
     */
    inline void diff8bitUnFilterWrite8bit(void* source, void* destination) { _internal::_asm_diff8bitUnFilterWrite8bit(source, destination); }

    /**
     * {@link #diff8bitUnFilterWrite8bit(void*, void*)}
     */
    inline void diff16bitUnFilter(void* source, void* destination) { _internal::_asm_diff16bitUnFilter(source, destination); }
    #endif

    /**
     * The decoder starts in root node, the separate bits in the bitstream specify if the next node is node0 or node1, if that node is a data node,
     * then the data is stored in memory, and the decoder is reset to the root node. The most often used data should be as close to the root node
     * as possible. For example, the 4-byte string "Huff" could be compressed to 6 bits: 10-11-0-0, with root.0 pointing directly to data "f", and
     * root.1 pointing to a child node, whose nodes point to data "H" and data "u".
     * Data is written in units of 32bits, if the size of the compressed data is not a multiple of 4, please adjust it as much as possible by
     * padding with 0.
     * Align the source address to a 4Byte boundary.
     *
     * r0  Source Address, aligned by 4, pointing to:
     *   Data Header (32bit)
     *     Bit0-3   Data size in bit units (normally 4 or 8)
     *     Bit4-7   Compressed type (must be 2 for Huffman)
     *     Bit8-31  24bit size of decompressed data in bytes
     *   Tree Size (8bit)
     *     Bit0-7   Size of Tree Table/2-1 (ie. Offset to Compressed Bitstream)
     *   Tree Table (list of 8bit nodes, starting with the root node)
     *    Root Node and Non-Data-Child Nodes are:
     *     Bit0-5   Offset to next child node,
     *              Next child node0 is at (CurrentAddr AND NOT 1)+Offset*2+2
     *              Next child node1 is at (CurrentAddr AND NOT 1)+Offset*2+2+1
     *     Bit6     Node1 End Flag (1=Next child node is data)
     *     Bit7     Node0 End Flag (1=Next child node is data)
     *    Data nodes are (when End Flag was set in parent node):
     *    Bit0-7   Data (upper bits should be zero if Data Size is less than 8)
     *  Compressed Bitstream (stored in units of 32bits)
     *    Bit0-31  Node Bits (Bit31=First Bit)  (0=Node0, 1=Node1)
     * r1  Destination Address
     * r2  Callback temp buffer      ;\for NDS/DSi "ReadByCallback" variants only
     * r3  Callback structure        ;/(see Callback notes below)
     *
     * Return: No return value, Data written to destination address.
     */
    inline void huffUnCompReadByCallback(void* source, void* destination, void* buffer, UnCompCallbacks* callbacks) { _internal::_asm_huffUnCompReadByCallback(source, destination, buffer, callbacks); }

    /**
     * Expands LZ77-compressed data. The Wram function is faster, and writes in units of 8bits. For the Vram function the destination must be
     * halfword aligned, data is written in units of 16bits.
     * CAUTION: Writing 16bit units to [dest-1] instead of 8bit units to [dest] means that reading from [dest-1] won't work, ie. the "Vram"
     * function works only with disp=001h..FFFh, but not with disp=000h.
     * If the size of the compressed data is not a multiple of 4, please adjust it as much as possible by padding with 0. Align the source address
     * to a 4-Byte boundary.
     *
     * r0  Source address, pointing to data as such:
     *   Data header (32bit)
     *     Bit 0-3   Reserved
     *     Bit 4-7   Compressed type (must be 1 for LZ77)
     *     Bit 8-31  Size of decompressed data
     *   Repeat below. Each Flag Byte followed by eight Blocks.
     *   Flag data (8bit)
     *     Bit 0-7   Type Flags for next 8 Blocks, MSB first
     *   Block Type 0 - Uncompressed - Copy 1 Byte from Source to Dest
     *     Bit 0-7   One data byte to be copied to dest
     *   Block Type 1 - Compressed - Copy N+3 Bytes from Dest-Disp-1 to Dest
     *     Bit 0-3   Disp MSBs
     *     Bit 4-7   Number of bytes to copy (minus 3)
     *     Bit 8-15  Disp LSBs
     * r1  Destination address
     * r2  Callback parameter        ;\for NDS/DSi "ReadByCallback" variants only
     * r3  Callback structure        ;/(see Callback notes below)
     *
     * Return: No return value.
     */
    inline void lz77UnCompReadNormalWrite8bit(void* source, void* destination) { _internal::_asm_lz77UnCompReadNormalWrite8bit(source, destination); }

    /**
     * {@link #lz77UnCompReadNormalWrite8bit(void*, void*)}
     */
    inline void lz77UnCompReadByCallbackWrite8bit(void* source, void* destination, u32 userParam, UnCompCallbacks* callbacks) { _internal::_asm_lz77UnCompReadByCallbackWrite8bit(source, destination, userParam, callbacks); }

    /**
     * {@link #lz77UnCompReadNormalWrite8bit(void*, void*)}
     */
    inline void lz77UnCompReadByCallbackWrite16bit(void* source, void* destination, u32 userParam, UnCompCallbacks* callbacks) { _internal::_asm_lz77UnCompReadByCallbackWrite16bit(source, destination, userParam, callbacks); }

    /**
     * Expands run-length compressed data. The Wram function is faster, and writes in units of 8bits. For the Vram function the destination must be
     * halfword aligned, data is written in units of 16bits.
     * If the size of the compressed data is not a multiple of 4, please adjust it as much as possible by padding with 0. Align the source address
     * to a 4Byte boundary.
     *
     * r0  Source Address, pointing to data as such:
     *   Data header (32bit)
     *     Bit 0-3   Reserved
     *     Bit 4-7   Compressed type (must be 3 for run-length)
     *     Bit 8-31  Size of decompressed data
     *   Repeat below. Each Flag Byte followed by one or more Data Bytes.
     *   Flag data (8bit)
     *     Bit 0-6   Expanded Data Length (uncompressed N-1, compressed N-3)
     *     Bit 7     Flag (0=uncompressed, 1=compressed)
     *   Data Byte(s) - N uncompressed bytes, or 1 byte repeated N times
     * r1  Destination Address
     * r2  Callback parameter        ;\for NDS/DSi "ReadByCallback" variants only
     * r3  Callback structure        ;/(see Callback notes below)
     *
     * Return: No return value, Data written to destination address.
     */
    inline void rlUnCompReadNormalWrite8bit(void* source, void* destination) { _internal::_asm_rlUnCompReadNormalWrite8bit(source, destination); }

    /**
     * {@link #rlUnCompReadNormalWrite8bit(void*, void*)}
     */
    inline void rlUnCompReadByCallbackWrite16bit(void* source, void* destination, u32 userParam, UnCompCallbacks* callbacks) { _internal::_asm_rlUnCompReadByCallbackWrite16bit(source, destination, userParam, callbacks); }

    /**
     * Memory copy/fill in units of 4 bytes or 2 bytes. Memcopy is implemented as repeated LDMIA/STMIA [Rb]!,r3 or LDRH/STRH r3,[r0,r5] instructions. Memfill as single LDMIA or LDRH followed by repeated STMIA [Rb]!,r3 or STRH r3,[r0,r5].
     * The length must be a multiple of 4 bytes (32bit mode) or 2 bytes (16bit mode). The (half)wordcount in r2 must be length/4 (32bit mode) or length/2 (16bit mode), ie. length in word/halfword units rather than byte units.
     *
     * r0    Source address        (must be aligned by 4 for 32bit, by 2 for 16bit)
     * r1    Destination address   (must be aligned by 4 for 32bit, by 2 for 16bit)
     * r2    Length/Mode
     *      Bit 0-20  Wordcount (for 32bit), or Halfwordcount (for 16bit)
     *      Bit 24    Fixed Source Address (0=Copy, 1=Fill by {HALF}WORD[r0])
     *      Bit 26    Datasize (0=16bit, 1=32bit)
     *
     * Return: No return value, Data written to destination address.
     */
    inline void cpuSet(void* source, void* destination, u32 length_mode) { _internal::_asm_cpuSet(source, destination, length_mode); }
    inline void cpuSet_copy16(void* source, void* destination, u32 halfwords)   { cpuSet(source, destination, (0 << 26) | (0 << 24) | halfwords); }
    inline void cpuSet_copy32(void* source, void* destination, u32 words)   { cpuSet(source, destination, (1 << 26) | (0 << 24) | words); }
    inline void cpuSet_fill16(void* source, void* destination, u32 halfwords)   { cpuSet(source, destination, (0 << 26) | (1 << 24) | halfwords); }
    inline void cpuSet_fill32(void* source, void* destination, u32 words)   { cpuSet(source, destination, (1 << 26) | (1 << 24) | words); }

    /**
     * Memory copy/fill in units of 32 bytes. Memcopy is implemented as repeated LDMIA/STMIA [Rb]!,r2-r9 instructions. Memfill as single LDR
     * followed by repeated STMIA [Rb]!,r2-r9.
     * After processing all 32-byte-blocks, the NDS/DSi additonally processes the remaining words as 4-byte blocks. BUG: The NDS/DSi uses the fast
     * 32-byte-block processing only for the first N bytes (not for the first N words), so only the first quarter of the memory block is FAST, the
     * remaining three quarters are SLOWLY copied word-by-word.
     * The length is specifed as wordcount, ie. the number of bytes divided by 4.
     * On the GBA, the length should be a multiple of 8 words (32 bytes) (otherwise the GBA is forcefully rounding-up the length). On NDS/DSi, the
     * length may be any number of words (4 bytes).
     *
     * r0    Source address        (must be aligned by 4)
     * r1    Destination address   (must be aligned by 4)
     * r2    Length/Mode
     *      Bit 0-20  Wordcount (GBA: rounded-up to multiple of 8 words)
     *      Bit 24    Fixed Source Address (0=Copy, 1=Fill by WORD[r0])
     *
     * Return: No return value, Data written to destination address.
     */
    inline void cpuFastSet(void* source, void* destination, u32 length_mode) { _internal::_asm_cpuFastSet(source, destination, length_mode); }
    inline void cpuFastSet_copy(void* source, void* destination, u32 words) { cpuFastSet(source, destination, (0 << 24) | words); }
    inline void cpuFastSet_fill(void* source, void* destination, u32 words) { cpuFastSet(source, destination, (1 << 24) | words); }

    /**
     * Halts the CPU until an interrupt request occurs. The CPU is switched into low-power mode, all other circuits (video, sound, timers, serial,
     * keypad, system clock) are kept operating.
     * Halt mode is terminated when any enabled interrupts are requested, that is when (IE AND IF) is not zero, the GBA locks up if that condition
     * doesn't get true. However, the state of CPUs IRQ disable bit in CPSR register, and the IME register are don't care, Halt passes through even
     * if either one has disabled interrupts.
     * On GBA and NDS7/DSi7, Halt is implemented by writing to HALTCNT, Port 4000301h. On NDS9/DSi9, Halt is implemted by writing to System Control
     * Coprocessor (mov p15,0,c7,c0,4,r0 opcode), this opcode hangs if IME=0.
     * No parameters, no return value.
     * (GBA/NDS7/DSi7: all registers unchanged, NDS9/DSi9: R0 destroyed)
     */
    inline void halt() { _internal::_asm_halt(); }

    /**
     * Continues to wait in Halt state until one (or more) of the specified interrupt(s) do occur. The function forcefully sets IME=1. When using
     * multiple interrupts at the same time, this function is having less overhead than repeatedly calling the Halt function.
     *
     * r0    0=Return immediately if an old flag was already set (NDS9: bugged!)
     *       1=Discard old flags, wait until a NEW flag becomes set
     * r1    Interrupt flag(s) to wait for (same format as IE/IF registers)
     * r2    DSi7 only: Extra flags (same format as DSi7's IE2/IF2 registers)
     *
     * Caution: When using IntrWait or VBlankIntrWait, the user interrupt handler MUST update the BIOS Interrupt Flags value in RAM; when acknowleding
     * processed interrupt(s) by writing a value to the IF register, the same value should be also ORed to the BIOS Interrupt Flags value, at
     * following memory location:
     *
     * Host     GBA (16bit)  NDS7 (32bit)  NDS9 (32bit)  DSi7-IF2 (32bit)
     * Address  [3007FF8h]   [380FFF8h]    [DTCM+3FF8h]  [380FFC0h]
     *
     * NDS9: BUG: No Discard (r0=0) doesn't work. The function always waits for at least one IRQ to occur (no matter which, including IRQs that are
     * not selected in r1), even if the desired flag was already set. NB. the same bug is also found in the GBA/NDS7 functions, but it's compensated
     * by a second bug, ie. the GBA/NDS7 functions are working okay because their "bug doesn't work".
     * Return: No return value, the selected flag(s) are automatically reset in BIOS Interrupt Flags value in RAM upon return.
     * DSi9: BUG: The function tries to enter Halt state via Port 4000301h (which would be okay on ARM7, but it's probably ignored on ARM9, which
     * should normally use CP15 to enter Halt state; if Port 4000301h is really ignored, then the function will "successfully" wait for interrupts,
     * but without actually entering any kind of low power mode).
     * DSi7: BUG: The function tries to wait for IF and IF2 interrupts, but it does accidently ignore the old IF interrupts, and works only with new
     * IF2 ones.
     */
    #ifdef ARM9
    inline void intrWait(bool ignorePrior, u32 flags) { _internal::_asm_intrWait(ignorePrior, flags); }
    #else
    inline void intrWait(bool ignorePrior, u32 flags, u32 flags_extra) { _internal::_asm_intrWait(ignorePrior, flags, flags_extra); }
    #endif

    /**
     * Continues to wait in Halt status until a new V-Blank interrupt occurs.
     * The function sets r0=1 and r1=1 (plus r2=0 on DSi7) and does then execute IntrWait (SWI 04h), see IntrWait for details.
     * No parameters, no return value.
     */
    inline void vBlankIntrWait() { _internal::_asm_vBlankIntrWait(); }

    #ifdef ARM7
    /**
     * No info, probably similar as GBA SWI 03h (Stop). Sleep is implemented for ARM7 only, not for ARM9. But maybe the ARM7 function does stop
     * <both> ARM7 and ARM9 (?)
     */
    inline void sleep() { _internal::_asm_sleep(); }
    #endif

    /**
     * Performs a "LOP: SUB R0,1 / BGT LOP" wait loop, the loop is executed in BIOS memory, which provides reliable timings (regardless of the
     * memory waitstates & cache state of the calling procedure). Intended only for short delays (eg. flash memory programming cycles).
     *
     * r0  Delay value (should be in range 1..7FFFFFFFh)
     *
     * Execution time varies for ARM7 vs ARM9. On ARM9 it does also depend on whether ROM is cached, and on DSi it does further depended on the
     * ARM9 CPU clock, and on whether using NDS or DSi BIOS ROM (NDS uses faster THUMB code, whilst DSi uses ARM code, which is slow on uncached
     * ARM9 ROM reads). For example, to get a 1 millisecond delay, use following values:
     *
     * CPU  Clock     Cache   BIOS     Value for 1ms
     * ARM7 33.51MHz  none    NDS/DSi  r0=20BAh    ;=20BAh  ;-ARM7
     * ARM9 67.03MHz  on      NDS/DSi  r0=20BAh*2  ;=4174h  ;\ARM9 with cache
     * ARM9 134.06MHz on      DSi      r0=20BAh*4  ;=82E8h  ;/
     * ARM9 67.03MHz  off     NDS      r0=20BAh/2  ;=105Dh  ;\
     * ARM9 67.03MHz  off     DSi      r0=20BAh/4  ;=082Eh  ; ARM9 without cache
     * ARM9 134.06MHz off     DSi      r0=20BAh/3  ;=0AE8h  ;/
     *
     * Return: No return value.
     */
    inline void waitByLoop(u32 delayCycles) { _internal::_asm_waitByLoop(delayCycles); }

    /**
     * r0  Initial CRC value (16bit, usually FFFFh)
     * r1  Start Address   (must be aligned by 2)
     * r2  Length in bytes (must be aligned by 2)
     *
     * CRC16 checksums can be calculated as such:
     *
     *   val[0..7] = C0C1h,C181h,C301h,C601h,CC01h,D801h,F001h,A001h
     *   for i=start to end
     *     crc=crc xor byte[i]
     *     for j=0 to 7
     *       crc=crc shr 1:if carry then crc=crc xor (val[j] shl (7-j))
     *     next j
     *   next i
     *
     * Return:
     *
     * r0  Calculated 16bit CRC Value
     *
     * Additionally, if the length is nonzero, r3 contains the last processed halfword at [addr+len-2]. Unlike most other NDS7/DSi7 SWI functions
     * (which do reject reading from BIOS memory), this allows to dump the NDS7/DSi7 BIOS (except for the memory region that is locked via BIOSPROT
     * Port 4000308h).
     */
    inline u16 getCRC16(u16 initial, void* source, u32 length) { return _internal::_asm_getCRC16(initial, source, length); }
    inline u16 getCRC16(void* source, u32 length) { return _internal::_asm_getCRC16(0xFFFF, source, length); }

    #ifdef ARM7
    /**
     * r0  Index (0..3Fh) (must be in that range, otherwise returns garbage)
     *
     * Return: r0 = Desired Entry (0000h..7FF5h) ;SIN(0 .. 88.6 degrees)*8000h
     */
    inline u16 getSineTable(u16 index) { return _internal::_asm_getSineTable(index); }

    /**
     * r0  Index (0..2FFh) (must be in that range, otherwise returns garbage)
     *
     * BUG: DSi7 accidently reads from SineTable instead of PitchTable, as workaround for obtaining PitchTable values, one can set
     * "r0=(0..2FFh)-46Ah" on DSi.
     * Return: r0 = Desired Entry (0000h..FF8Ah) (unsigned)
     */
    inline u16 getPitchTable(u16 index) { return _internal::_asm_getPitchTable(index); }

    /**
     * r0  Index (0..2D3h) (must be in that range, otherwise returns garbage)
     *
     * Return: r0 = Desired Entry (00h..7Fh) (unsigned)
     */
    inline u8 getVolumeTable(u16 index) { return _internal::_asm_getVolumeTable(index); }

    /**
     * Increments or decrements the current level of the SOUNDBIAS register (with short delays) until reaching the desired new level. The upper bits of the register are kept unchanged.
     *
     * r0   BIAS level (0=Level 000h, any other value=Level 200h)
     * r1   Delay Count (NDS/DSi only) (GBA uses a fixed delay count of 8)
     *
     * Return: No return value.
     */
    inline void soundBias(u16 biasLevel, u32 delayCount) { _internal::_asm_soundBias(biasLevel, delayCount); }
    #endif

    inline void sha1_init(SHA1Context* context) { _internal::_asm_sha1_init(context); }
    inline void sha1_update(SHA1Context* context, const void* source, u32 length) { _internal::_asm_sha1_update(context, source, length); }
    inline void sha1_finish(void* digest, SHA1Context* context) { _internal::_asm_sha1_finish(digest, context); }
    inline void sha1_init_update_finish(void* digest, const void* source, u32 length) { _internal::_asm_sha1_init_update_finish(digest, source, length); }
    inline bool sha1_compare_20_bytes(const void* source1, const void* source2) { return _internal::_asm_sha1_compare_20_bytes(source1, source2); }
    inline void sha1_init_update_finish_mess(void* digest, u32 digestLength, const void* source, u32 length) { _internal::_asm_sha1_init_update_finish_mess(digest, digestLength, source, length); }
}

#endif //LIBDSI_DSIBIOS_H
