#ifndef LIBDSI_ROOT_ROMHEADER_H
#define LIBDSI_ROOT_ROMHEADER_H

#include <porklib.h>

using namespace porklib;

struct ndsHeader {
    u8 title[0xC];
    u8 gamecode[0x4];
    u8 makercode[2];
    u8 unitcode;							// product code. 0=NDS, 2=NDS+DSi, 3=DSi
    u8 devicetype;						// device code. 0 = normal
    u8 devicecap;						// device size. (1<<n Mbit)
    u8 reserved1[0x7];					// 0x015..0x01D
    u8 dsi_flags;
    u8 nds_region;
    u8 romversion;
    u8 reserved2;						// 0x01F
    u32_le arm9_rom_offset;					// points to libsyscall and rest of ARM9 binary
    u32_le arm9_entry_address;
    u32_le arm9_ram_address;
    u32_le arm9_size;
    u32_le arm7_rom_offset;
    u32_le arm7_entry_address;
    u32_le arm7_ram_address;
    u32_le arm7_size;
    u32_le fnt_offset;
    u32_le fnt_size;
    u32_le fat_offset;
    u32_le fat_size;
    u32_le arm9_overlay_offset;
    u32_le arm9_overlay_size;
    u32_le arm7_overlay_offset;
    u32_le arm7_overlay_size;
    u32_le rom_control_info1;					// 0x00416657 for OneTimePROM
    u32_le rom_control_info2;					// 0x081808F8 for OneTimePROM
    u32_le banner_offset;
    u16_le secure_area_crc;
    u16_le rom_control_info3;				// 0x0D7E for OneTimePROM
    u32_le offset_0x70;						// magic1 (64 bit encrypted magic code to disable LFSR)
    u32_le offset_0x74;						// magic2
    u32_le offset_0x78;						// unique ID for homebrew
    u32_le offset_0x7C;						// unique ID for homebrew
    u32_le application_end_offset;			// rom size
    u32_le rom_header_size;
    u32_le offset_0x88;						// reserved... ?
    u32_le offset_0x8C;

    // reserved
    u32_le offset_0x90;
    u32_le offset_0x94;
    u32_le offset_0x98;
    u32_le offset_0x9C;
    u32_le offset_0xA0;
    u32_le offset_0xA4;
    u32_le offset_0xA8;
    u32_le offset_0xAC;
    u32_le offset_0xB0;
    u32_le offset_0xB4;
    u32_le offset_0xB8;
    u32_le offset_0xBC;

    u8 logo[156];						// character data
    u16_le logo_crc;
    u16_le header_crc;

    // 0x160..0x17F reserved
    u32_le debug_rom_offset;
    u32_le debug_size;
    u32_le debug_ram_address;
    u32_le offset_0x16C;
    u8 zero[0x10];

    // DSi extended stuff below
    u8 global_mbk_setting[5][4];
    u32_le arm9_mbk_setting[3];
    u32_le arm7_mbk_setting[3];
    u32_le mbk9_wramcnt_setting;

    u32_le region_flags;
    u32_le access_control;
    u32_le scfg_ext_mask;
    u8 offset_0x1BC[3];
    u8 appflags;

    u32_le dsi9_rom_offset;
    u32_le offset_0x1C4;
    u32_le dsi9_ram_address;
    u32_le dsi9_size;
    u32_le dsi7_rom_offset;
    u32_le offset_0x1D4;
    u32_le dsi7_ram_address;
    u32_le dsi7_size;

    u32_le digest_ntr_start;
    u32_le digest_ntr_size;
    u32_le digest_twl_start;
    u32_le digest_twl_size;
    u32_le sector_hashtable_start;
    u32_le sector_hashtable_size;
    u32_le block_hashtable_start;
    u32_le block_hashtable_size;
    u32_le digest_sector_size;
    u32_le digest_block_sectorcount;

    u32_le banner_size;
    u32_le offset_0x20C;
    u32_le total_rom_size;
    u32_le offset_0x214;
    u32_le offset_0x218;
    u32_le offset_0x21C;

    u32_le modcrypt1_start;
    u32_le modcrypt1_size;
    u32_le modcrypt2_start;
    u32_le modcrypt2_size;

    u32_le tid_low;
    u32_le tid_high;
    u32_le public_sav_size;
    u32_le private_sav_size;
    u8 reserved3[176];
    u8 age_ratings[0x10];

    u8 hmac_arm9[20];
    u8 hmac_arm7[20];
    u8 hmac_digest_master[20];
    u8 hmac_icon_title[20];
    u8 hmac_arm9i[20];
    u8 hmac_arm7i[20];
    u8 reserved4[40];
    u8 hmac_arm9_no_secure[20];
    u8 reserved5[2636];
    u8 debug_args[0x180];
    u8 rsa_signature[0x80];
};

#endif //LIBDSI_ROOT_ROMHEADER_H
