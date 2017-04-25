#include "osapi.h"
#include "user_config.h"
#include "spi_flash.h"
#include "mem.h"

void config_save() {

    SpiFlashOpResult result;

    config.id++;

#ifdef DEBUG
    os_printf("config_save...\n");
#endif

    do {
        config.pos++;
        
        if (config.pos == CONFIG_SEC_COUNT)
            config.pos = 0;

        spi_flash_erase_sector(CONFIG_START_SEC + config.pos);
        result = spi_flash_write((CONFIG_START_SEC + config.pos) * SPI_FLASH_SEC_SIZE, (uint32*)&config, sizeof(struct ConfigStruct));

#ifdef DEBUG
    os_printf("config.id = %d\n", config.id);
    os_printf("config.pos = %u\n", config.pos);
#endif

    } while (result != SPI_FLASH_RESULT_OK);
}

void config_load() {

    uint8 i = 0;
    struct ConfigStruct readConfig;
    SpiFlashOpResult result;

    os_memset(&config, 0, sizeof(struct ConfigStruct));
    config.safe = 85;

#ifdef DEBUG
    os_printf("config_load...\n");
#endif

    for (i = 0; i < CONFIG_SEC_COUNT; i++) {
        
        result = spi_flash_read((CONFIG_START_SEC + i) * SPI_FLASH_SEC_SIZE, (uint32*)&readConfig, sizeof(struct ConfigStruct));

        if (result == SPI_FLASH_RESULT_OK) {
            if (readConfig.safe == 85 && readConfig.id > config.id) {
                config.id = readConfig.id;
                config.pos = i;

                //os_memcpy(&config, &readConfig, sizeof(struct ConfigStruct));
            }
        }
    }

#ifdef DEBUG
    os_printf("config.id = %d\n", config.id);
    os_printf("config.pos = %u\n", config.pos);
#endif

    os_free(&readConfig);
}