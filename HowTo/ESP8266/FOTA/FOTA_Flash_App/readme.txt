

https://espressif.com/sites/default/files/documentation/99c-esp8266_fota_guide_en_.pdf


http://www.esp8266.com/viewtopic.php?f=6&t=11690

system_upgrade_userbin_check()
<calculate address to write new firmware i.e. 0x101000 or 0x01000>
system_upgrade_init();
system_upgrade_flag_set(UPGRADE_FLAG_START);
<write image here using spi_flash_erase_sector() & spi_flash_write() - 4bytes aligned>
<if success call system_upgrade_flag_set(UPGRADE_FLAG_FINISH)>
and call system_upgrade_reboot() - See more at: http://www.esp8266.com/viewtopic.php?f=6&t=11690#sthash.fXmABoP6.dpuf



Memory (1024KB + 1024KB) STEP5 = 6

Position    Size       Name
0x000000  |      4kb | Boot
0x001000  |    940kb | User 1 area - Irom0text.bin
0x0EC000  |     64kb | User 1 area - Flash.bin
0x0FC000  |      8kb | 
0x0FE000  |      8kb | User param

0x100000  |      4kb | Reserved
0x101000  |    940kb | User 2 area - Irom0text.bin
0x1EC000  |     64kb | User 2 area - Flash.bin
0x1FC000  |   2048kb | User data
0x3FC000  |      8kb | 
0x3FE000  |      8kb | System param