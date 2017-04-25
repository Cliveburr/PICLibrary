#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "espconn.h"
#include "upgrade.h"
#include "mem.h"
#include "flash_funcs.h"

struct espconn udp_client;
esp_udp udp;

struct espconn tcp_conn;
esp_tcp esptcp;

uint8 lock_upgrade;
uint32 upgrade_length;
uint16 upgrade_sector;
//uint16 upgrade_sector_count;
char *upgrade_buffer;
uint16 upgrade_buffer_length;

uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void print_buffer(uint8 *buf, uint8 len) {
    uint8 i = 0;
    os_printf("[");
    for (i = 0; i < len; i++) {
        os_printf("%u", buf[i]);
        if (i < len - 1)
            os_printf(",");
    }
    os_printf("]\n");
}

// void UdpRecvCb(void *arg, char *pdata, unsigned short len)
// {
// 	os_printf("udp message received:\n");
// 	os_printf("%s\r\n", pdata);
// }

void send_userbin_check(void *arg)
{
    struct espconn *pesp_conn = arg;

    uint8 userbin = system_upgrade_userbin_check();

    os_printf("send_userbin_check: %u\n", userbin);

    uint8 buffer[1] = {0};
    buffer[0] = userbin;

    espconn_sent(pesp_conn, buffer, 1);
}

void start_system_upgrade(char *data)
{
    os_printf("start_system_upgrade\n");

    upgrade_buffer = (char*)os_zalloc(SPI_FLASH_SEC_SIZE);
    os_memset(upgrade_buffer, 0, SPI_FLASH_SEC_SIZE);
    upgrade_buffer_length = 0;

    upgrade_sector = (data[1] << 8) | data[0];
    //upgrade_sector_count = (data[3] << 8) | data[2];
    upgrade_length = (data[5] << 24) | (data[4] << 16) | (data[3] << 8) | data[2];

    os_printf("upgrade_sector: %u\n", upgrade_sector);
    //os_printf("upgrade_sector_count: %u\n", upgrade_sector_count);
    os_printf("upgrade_length: %u\n", upgrade_length);

    system_upgrade_init();
    system_upgrade_flag_set(UPGRADE_FLAG_START);

    lock_upgrade = 1;
}

char write_system_upgrade(char *data, unsigned short length)
{
    os_printf("write_system_upgrade - length: %u\n", length);

    uint16 tlength = 0;
    uint16 left = 0;

    if (upgrade_buffer_length + length > SPI_FLASH_SEC_SIZE) {
        tlength = SPI_FLASH_SEC_SIZE - upgrade_buffer_length;
        left = length - tlength;

        os_memcpy(upgrade_buffer + upgrade_buffer_length, data, tlength);
        upgrade_buffer_length += tlength;
    }
    else {
        os_memcpy(upgrade_buffer + upgrade_buffer_length, data, length);
        upgrade_buffer_length += length;
    }

    if (upgrade_buffer_length == SPI_FLASH_SEC_SIZE) {
        os_printf("spi_flash_write: %u bytes for %u sector\n", upgrade_buffer_length, upgrade_sector);

        spi_flash_erase_sector(upgrade_sector);
        spi_flash_write(upgrade_sector * SPI_FLASH_SEC_SIZE, (uint32*)upgrade_buffer, SPI_FLASH_SEC_SIZE);

        upgrade_sector++;
        os_memset(upgrade_buffer, 0, SPI_FLASH_SEC_SIZE);
        upgrade_buffer_length = 0;
    }

    if (left > 0) {
        os_memcpy(upgrade_buffer, data + tlength, left);
        upgrade_buffer_length += left;
    }

    // system_upgrade(data, length);

    upgrade_length -= length;

    if (upgrade_length == 0) {
        if (upgrade_buffer_length > 0) {
            os_printf("spi_flash_write: %u bytes for %u sector\n", upgrade_buffer_length, upgrade_sector);

            spi_flash_erase_sector(upgrade_sector);
            spi_flash_write(upgrade_sector * SPI_FLASH_SEC_SIZE, (uint32*)upgrade_buffer, SPI_FLASH_SEC_SIZE);
        }

        return true;
    }

    return false;

    //os_memcpy(upgrade_buffer + (upgrade_buffer_pos * 1024), data, length);

    // upgrade_buffer_pos++;

    // if (upgrade_buffer_pos == 4) {
    //     os_printf("upgrade_sector: %u\n", upgrade_sector);

    //     spi_flash_erase_sector(upgrade_sector);
    //     spi_flash_write(upgrade_sector * SPI_FLASH_SEC_SIZE, (uint32*)upgrade_buffer, 4096);

    //     upgrade_sector++;
    //     upgrade_sector_count--;

    //     if (upgrade_sector_count == 0) {
    //         system_upgrade_flag_set(UPGRADE_FLAG_FINISH);
    //         system_upgrade_reboot();
    //     }

    //     os_memset(upgrade_buffer, 0, 4096);
    //     upgrade_buffer_pos = 0;
    // }
    // else {
    //     os_printf("\n");
    // }
}

// void finish_system_upgrade()
// {
//     os_printf("finish_system_upgrade\n");
    
//     system_upgrade_flag_set(UPGRADE_FLAG_FINISH);
//     system_upgrade_reboot();
// }

LOCAL void ICACHE_FLASH_ATTR
tco_received(void *arg, char *data, unsigned short length)
{
    struct espconn *pesp_conn = arg;

    if (lock_upgrade) {
        uint8 buffer[1] = {0};

        if (write_system_upgrade(data, length)) {
            buffer[0] = 86;
            espconn_sent(pesp_conn, buffer, 1);

            espconn_disconnect(pesp_conn);

            system_upgrade_flag_set(UPGRADE_FLAG_FINISH);
            system_upgrade_reboot();
        } else {
            buffer[0] = 85;
            espconn_sent(pesp_conn, buffer, 1);
        }
    }
    else {
        os_printf("tco_received %d.%d.%d.%d:%d\n",
            pesp_conn->proto.tcp->remote_ip[0],
            pesp_conn->proto.tcp->remote_ip[1],
            pesp_conn->proto.tcp->remote_ip[2],
            pesp_conn->proto.tcp->remote_ip[3],
            pesp_conn->proto.tcp->remote_port);
        os_printf("data: "); print_buffer(data, length);

        // if (data[0] == 7 && data[1] == 7) {
        //     //os_memcpy(udp_client.proto.udp->remote_ip, address, 4);
        //     //udp_client.proto.udp->remote_port = SEND_PORT;

        //     uint8 buffer[2] = { 8, 8 };
        //     espconn_sent(pesp_conn, buffer, 2);
        // }

        switch (data[0]) {
            case 5: send_userbin_check(arg); break;
            case 6: start_system_upgrade(&data[1]); break;
            //case 7: write_system_upgrade(&data[1]); break;
            //case 8: finish_system_upgrade(); break;
        }
    }
}

LOCAL ICACHE_FLASH_ATTR
void tcp_reconnection(void *arg, sint8 err)
{
    struct espconn *pesp_conn = arg;

    os_printf("tcp_reconnection %d.%d.%d.%d:%d err %d reconnect\n", pesp_conn->proto.tcp->remote_ip[0],
    		pesp_conn->proto.tcp->remote_ip[1],pesp_conn->proto.tcp->remote_ip[2],
    		pesp_conn->proto.tcp->remote_ip[3],pesp_conn->proto.tcp->remote_port, err);
}

LOCAL ICACHE_FLASH_ATTR
void tcp_disconnect(void *arg)
{
    struct espconn *pesp_conn = arg;

    os_printf("tcp_disconnect %d.%d.%d.%d:%d disconnect\n", pesp_conn->proto.tcp->remote_ip[0],
        		pesp_conn->proto.tcp->remote_ip[1],pesp_conn->proto.tcp->remote_ip[2],
        		pesp_conn->proto.tcp->remote_ip[3],pesp_conn->proto.tcp->remote_port);
}

LOCAL void ICACHE_FLASH_ATTR
tcp_connection(void *arg)
{
    struct espconn *pesp_conn = arg;

	os_printf("tcp_connection %d.%d.%d.%d:%d\n",
        pesp_conn->proto.tcp->remote_ip[0],
    	pesp_conn->proto.tcp->remote_ip[1],
        pesp_conn->proto.tcp->remote_ip[2],
    	pesp_conn->proto.tcp->remote_ip[3],
        pesp_conn->proto.tcp->remote_port);

    espconn_regist_recvcb(pesp_conn, tco_received);
    espconn_regist_reconcb(pesp_conn, tcp_reconnection);
    espconn_regist_disconcb(pesp_conn, tcp_disconnect);
}


void wifi_handle_event_cb(System_Event_t *evt)
{
    os_printf("wifi_handle_event_cb %x\n", evt->event);
	
    switch (evt->event) {
        case EVENT_STAMODE_GOT_IP:
			//  udp_client.type = ESPCONN_UDP;
			//  udp_client.proto.udp=&udp;
			//  udp.local_port=8080;
			//  espconn_regist_recvcb(&udp_client,UdpRecvCb);
			//  espconn_create(&udp_client);

            tcp_conn.type = ESPCONN_TCP;
            tcp_conn.state = ESPCONN_NONE;
            tcp_conn.proto.tcp = &esptcp;
            tcp_conn.proto.tcp->local_port = 15555;
            espconn_regist_connectcb(&tcp_conn, tcp_connection);
            espconn_accept(&tcp_conn);
            break;
        default:
            break;
    }
} 

void user_init(void)
{
    os_printf("Clivedurr - HowTo - ESP8266 FOTA 6\n");
    os_printf("SDK version:%s\n", system_get_sdk_version());
	
    config_load();

    struct station_config stationConf;
	wifi_set_opmode_current(STATION_MODE);
	os_memset(&stationConf, 0, sizeof(struct station_config));
	os_sprintf(stationConf.ssid, "Matrix");
	os_sprintf(stationConf.password, "xxx");
	wifi_station_set_config_current(&stationConf);
	wifi_set_event_handler_cb(wifi_handle_event_cb);
    wifi_station_disconnect();
	wifi_station_connect();
	wifi_set_sleep_type(LIGHT_SLEEP_T); 


    config_save();

}