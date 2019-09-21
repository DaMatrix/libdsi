#ifndef LIBDSI_DSIWIFI_H
#define LIBDSI_DSIWIFI_H

#include <libdsi/types.h>

namespace dsi::wifi {
    /**
     * Struct defining the format of the Wifi connection data in memory. (used for connections 1-3)
     * <p>
     * See https://problemkaputt.de/gbatek.htm#dsfirmwarewifiinternetaccesspoints
     *
     * @author DaPorkchop_
     */
    struct WifiConnectionData {
        u8  _unknown_000[64]; //(usually 00h-filled)
        u8  ssid[32]; //SSID (ASCII name of the access point) (padded with 00h's)
        u8  ssid_web64_aoss[32]; //SSID for WEP64 on AOSS router (each security level has its own SSID)
        u8  wep_keys[4][16]; //WEP keys (1-3 are usually 00h-filled)
        u32 ip; //IP Address (0=Auto/DHCP)
        u32 gateway; //Gateway (0=Auto/DHCP)
        u32 dns_primary; //Primary DNS Server (0=Auto/DHCP)
        u32 dns_secondary; //Secondary DNS Server (0=Auto/DHCP)
        u8  subnetMask; //Subnet Mask (0=Auto/DHCP, 1..1Ch=# of leading ones) (eg. 6 = FC.00.00.00)
        u8  _unknown_0D1[21]; //(usually 00h-filled)
        u8  wep_mode; //WEP Mode (0=None, 1/2/3=5/13/16 byte hex, 5/6/7=5/13/16 byte ascii)
        u8  status; //Status (00h=Normal, 01h=AOSS, FFh=connection not configured/deleted)
        u8  _unknown_0E8; //(always 00h)
        u8  _unknown_0E9; //(usually 00h)
        u16 _unknown_0EA; //(usually 00h)
        u8  _unknown_0EC[3]; //(usually 00h-filled)
        u8  configured_flags; //bit0/1/2 - connection 1/2/3 (1=Configured, 0=Not configured)
        u8  wfc_userid[6]; //Nintendo Wifi Connection (WFC) 43bit User ID (ID=([F0h] AND 07FFFFFFFFFFh)*1000, shown as decimal string NNNN-NNNN-NNNN-N000) (the upper 5bit of the last byte contain additional/unknown nonzero data)
        u8  _unknown_0F6[8]; //(nonzero stuff !?!)
        u16 checksum_000_0FD; //CRC16 of _unknown_000 to _unknown_0F6 (0000h=deleted)
    };

    /**
     * Struct defining the format of the DSi's Wifi connection data in memory. (used only for connections 4-6)
     * <p>
     * See https://problemkaputt.de/gbatek.htm#dsfirmwarewifiinternetaccesspoints
     *
     * @author DaPorkchop_
     */
    struct DSiWifiConnectionData {
        u8  proxy_username[32]; //Proxy Authentication Username (ASCII string, padded with 00h's)
        u8  proxy_password[32]; //Proxy Authentication Password (ASCII string, padded with 00h's)
        u8  ssid[32]; //SSID (ASCII name of the access point) (padded with 00h's)
        u8  ssid_web64_aoss[32]; //SSID for WEP64 on AOSS router (each security level has its own SSID)
        u8  wep_keys[4][16]; //WEP keys (1-3 are usually 00h-filled) (all are 00h-filled for WPA)
        u32 ip; //IP Address (0=Auto/DHCP)
        u32 gateway; //Gateway (0=Auto/DHCP)
        u32 dns_primary; //Primary DNS Server (0=Auto/DHCP)
        u32 dns_secondary; //Secondary DNS Server (0=Auto/DHCP)
        u8  subnetMask; //Subnet Mask (0=Auto/DHCP, 1..1Ch=# of leading ones) (eg. 6 = FC.00.00.00)
        u8  _unknown_0D1[21]; //(usually 00h-filled)
        u8  wep_mode; //WEP Mode (0=None, 1/2/3=5/13/16 byte hex, 5/6/7=5/13/16 byte ascii)
        u8  wpa_mode; //WPA Mode (00h=Normal, 10h=WPA/WPA2, 13h=WPS+WPA/WPA2, FFh=unused/deleted)
        u8  ssid_length; //SSID Length in characters (01h..20h, or 00h=unused)
        u8  _unknown_0E9; //(usually 00h)
        u16 mtu; //MTU Value (Max transmission unit) (576..1500, usually 1400)
        u8  _unknown_0EC[3]; //(usually 00h-filled)
        u8  configured_flags; //bit0/1/2 - connection 4/5/6 (1=Configured, 0=Not configured)
        u8  _unknown_0F0[14]; //Zerofilled (or maybe ID as on NDS, if any such ID exists for DSi?)
        u16 checksum_000_0FD; //CRC16 of _unknown_000 to _unknown_0F0 (0000h=deleted)

        u8  wpa_psk[32]; //Precomputed PSK (based on WPA/WPA2 password and SSID) (all zero for WEP)
        u8  wpa_password[64]; //WPA/WPA2 password (ASCII string, padded with 00's) (all zero for WEP)
        u8  _unknown_160[33]; //Zerofilled
        u8  wpa_mode_specific; //WPA (0=None/WEP, 4=WPA-TKIP, 5=WPA2-TKIP, 6=WPA-AES, 7=WPA2-AES)
        u8  proxy_enable; //Proxy Enable (00h=None, 01h=Yes)
        u8  proxy_auth; //Proxy Authentication (00h=None, 01h=Yes)
        u8  proxy_name[48]; //Proxy Name (ASCII string, max 47 chars, padded with 00h's)
        u8  _unknown_1B4[52]; //Zerofilled
        u16 proxy_port; //Proxy Port (16bit)
        u8  _unknown_1EA[20]; //Zerofilled
        u16 checksum_100_1FD; //CRC16 of wpa_psk to _unknown_1EA (0000h=deleted)
    };
}

#endif //LIBDSI_DSIWIFI_H
