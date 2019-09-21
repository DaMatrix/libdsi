#ifndef LIBDSI_DSIBIOS_H
#define LIBDSI_DSIBIOS_H

#include <libdsi/dsitypes.h>

namespace libdsi::bios {
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
    //__attribute__((naked,target("thumb"))) i32 div(i32 numerator, i32 denominator) { asm("swi 0x09"); asm("bx lr"); }
    //inline i32 div(i32 numerator, i32 denominator) { return _internal::_asm_div(numerator, denominator); }
    i32 div(i32 numerator, i32 denominator);

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
    //__attribute__((naked,target("thumb"))) u16 sqrt(u32 number) { asm("swi 0x0D"); asm("bx lr"); }

    enum BitUnPackInformationSourceWidth: u8 {
        Source_1bit = 1,
        Source_2bit = 2,
        Source_4bit = 4,
        Source_8bit = 8
    };

    enum BitUnPackInformationDestinationWidth: u8 {
        Destination_1bit = 1,
        Destination_2bit = 2,
        Destination_4bit = 4,
        Destination_8bit = 8,
        Destination_16bit = 16
    };

    struct BitUnPackInformation {
        u16 sourceLength;
        BitUnPackInformationSourceWidth sourceWidth;
        BitUnPackInformationDestinationWidth destinationWidth;
        u32 dataOffset_zeroDataFlag;

        BitUnPackInformation(u16 sourceLength, BitUnPackInformationSourceWidth sourceWidth, BitUnPackInformationDestinationWidth destinationWidth, u32 dataOffset, bool zeroDataFlag):
                sourceLength(sourceLength),
                sourceWidth(sourceWidth),
                destinationWidth(destinationWidth),
                dataOffset_zeroDataFlag((dataOffset & 0x7FFFFFFF) | (zeroDataFlag << 31)) {}
    };

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
    //__attribute__((naked,target("thumb"))) void bitUnPack(void* source, void* destination, BitUnPackInformation* information) { asm("swi 0x10"); asm("bx lr"); }
    /*void bitUnPack(void* source, void* destination, u16 sourceLength, BitUnPackInformationSourceWidth sourceWidth, BitUnPackInformationDestinationWidth destinationWidth, u32 dataOffset, bool zeroDataFlag)    {
        BitUnPackInformation information(sourceLength, sourceWidth, destinationWidth, dataOffset, zeroDataFlag);
        bitUnPack(source, destination, &information);
    }*/

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
    //__attribute__((naked,target("thumb"))) void diff8bitUnFilterWrite8bit(void* source, void* destination) { asm("swi 0x16"); asm("bx lr"); }

    /**
     * {@link #diff8bitUnFilterWrite8bit(void*, void*)}
     */
    //__attribute__((naked,target("thumb"))) void diff16bitUnFilter(void* source, void* destination) { asm("swi 0x18"); asm("bx lr"); }
    #endif

    /**
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
    struct DecompressionCallbacks {
            using Callback = i32 (*)(void* sourceAddress, void* initialDestinationAddress, u32 userParam);

        public:
            Callback open_and_get_32bit;
            Callback close;
            Callback get_8bit;
        private:
            Callback get_16bit; //never used
        public:
            Callback get_32bit; //only used by huffman

            DecompressionCallbacks(Callback open_and_get_32bit, Callback get_8bit, Callback get_32bit):
                    open_and_get_32bit(open_and_get_32bit),
                    close(nullptr),
                    get_8bit(get_8bit),
                    get_16bit(nullptr),
                    get_32bit(get_32bit) {}

            DecompressionCallbacks(Callback open_and_get_32bit, Callback close, Callback get_8bit, Callback get_32bit):
                    open_and_get_32bit(open_and_get_32bit),
                    close(close),
                    get_8bit(get_8bit),
                    get_16bit(nullptr),
                    get_32bit(get_32bit) {}
    };

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
    //__attribute__((naked,target("thumb"))) void huffUnCompReadByCallback(void* source, void* destination) { asm("swi 0x13"); asm("bx lr"); }

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
    //__attribute__((naked,target("thumb"))) void lz77UnCompReadNormalWrite8bit(void* source, void* destination) {asm("swi 0x11"); asm("bx lr"); }

    /**
     * {@link #lz77UnCompReadNormalWrite8bit(void*, void*)}
     */
    //__attribute__((naked,target("thumb"))) void lz77UnCompReadByCallbackWrite8bit(void* source, void* destination, u32 userParam, DecompressionCallbacks* callbacks) {asm("swi 0x01"); asm("bx lr"); }

    /**
     * {@link #lz77UnCompReadNormalWrite8bit(void*, void*)}
     */
    //__attribute__((naked,target("thumb"))) void lz77UnCompReadByCallbackWrite16bit(void* source, void* destination, u32 userParam, DecompressionCallbacks* callbacks) {asm("swi 0x02"); asm("bx lr"); }

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
    //__attribute__((naked,target("thumb"))) void rlUnCompReadNormalWrite8bit(void* source, void* destination) {asm("swi 0x14"); asm("bx lr"); }

    /**
     * {@link #rlUnCompReadNormalWrite8bit(void*, void*)}
     */
    //__attribute__((naked,target("thumb"))) void rlUnCompReadByCallbackWrite16bit(void* source, void* destination, u32 userParam, DecompressionCallbacks* callbacks) {asm("swi 0x15"); asm("bx lr"); }

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
    //__attribute__((naked,target("thumb"))) void cpuFastSet(void* source, void* destination, u32 length_mode) {asm("swi 0x0C"); asm("bx lr"); }

    //void CpuSet(); //0x0B
}

#endif //LIBDSI_DSIBIOS_H
