

https://espressif.com/sites/default/files/documentation/99c-esp8266_fota_guide_en_.pdf


http://www.esp8266.com/viewtopic.php?f=6&t=11690

system_upgrade_userbin_check()
<calculate address to write new firmware i.e. 0x101000 or 0x01000>
system_upgrade_init();
system_upgrade_flag_set(UPGRADE_FLAG_START);
<write image here using spi_flash_erase_sector() & spi_flash_write() - 4bytes aligned>
<if success call system_upgrade_flag_set(UPGRADE_FLAG_FINISH)>
and call system_upgrade_reboot() - See more at: http://www.esp8266.com/viewtopic.php?f=6&t=11690#sthash.fXmABoP6.dpuf