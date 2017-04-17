#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "espconn.h"
#include "gpio.h"


/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
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



void UdpRecvCb(void *arg, char *pdata, unsigned short len)
{

    if(pdata[0] == '-'){
        return;
    }
    
	os_printf("udp message received:\n");
	os_printf("%s\r\n", pdata);

     if(pdata[0] == '0'){
        GPIO_OUTPUT_SET(GPIO_ID_PIN(4),1);
    }else{
        GPIO_OUTPUT_SET(GPIO_ID_PIN(4),0);
    }


}


struct espconn udp_client;
esp_udp udp;

int connected = 0;

LOCAL struct espconn tcp_conn;
LOCAL struct _esp_tcp _tcp;
int connected;

struct espconn *conn;

void ConnCb(void *arg){
    os_printf("Connected\n");
    os_printf("Connected\n");
    os_printf("Connected\n");
    os_printf("Connected\n");
    conn = arg;
    connected = 1;

}

void ConnCbErr(void *arg, sint8	err){
    os_printf("Err\n");
    os_printf("Err\n");
    os_printf("Err\n");
    os_printf("Err\n");
    //os_printf(arg);
}

void wifi_handle_event_cb(System_Event_t *evt)
{
    //os_printf("event %x\n", evt->event);
	int res;

    if(!connected){
        connected = 1;

    switch (evt->event) {
         //case 7:    
         case EVENT_STAMODE_GOT_IP:
			// udp_client.type=ESPCONN_UDP; 
			// udp_client.proto.udp=&udp;
			// udp.local_port=8080;
			// udp.remote_port=8080;
			// espconn_regist_recvcb(&udp_client,UdpRecvCb);
			// res = espconn_create(&udp_client);
			// os_printf("%d\n",res);


             tcp_conn.proto.tcp = &_tcp;
             tcp_conn.type = ESPCONN_TCP;
             tcp_conn.state = ESPCONN_NONE;
             const char esp_tcp_server_ip[4] = {192, 168, 0, 76}; // remote IP of TCP server
             os_memcpy(tcp_conn.proto.tcp->remote_ip, esp_tcp_server_ip, 4);
             tcp_conn.proto.tcp->remote_port = 8089;
             tcp_conn.proto.tcp->local_port = espconn_port();

             espconn_regist_connectcb(&tcp_conn, ConnCb); // register connect callback
             espconn_regist_reconcb(&tcp_conn, ConnCbErr); // register reconnect callback as error handler
             espconn_connect(&tcp_conn); 



             break;
         default:
             break;
    }
 }
} 



/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
#include "driver/uart.h"


LOCAL os_timer_t testTM;
LOCAL os_timer_t testTM2;

int bla;
void test(){
    bla += 1;
    int level = GPIO_INPUT_GET(GPIO_ID_PIN(15));
    if(level){
        return;
    }

    os_printf("time: %d us. \n" , bla);
    bla = 0;
    os_timer_disarm(&testTM);
}

void test2(){
    os_printf("i' here' \n" );
    
    
    if(bla){
        GPIO_OUTPUT_SET(GPIO_ID_PIN(4),1);
        bla = 0;
    }else{
        GPIO_OUTPUT_SET(GPIO_ID_PIN(4),0);
        bla = 1;
    }

    if(connected){
        uint8 data[11] = {'V', 'a', 'i',' ','P', 'l', 'a', 'n', 'e', 't', 'a'};
        espconn_send(conn, data, 11);
    }
}

struct	ip_info	info;

void user_init(void)
{
    bla = 0;
    os_printf("SDK version:%s\n", system_get_sdk_version());
    os_printf("Vai Planeta, aaaa");
    struct station_config stationConf;
	wifi_set_opmode_current(STATION_MODE);
	os_memset(&stationConf, 0, sizeof(struct station_config));
//	os_sprintf(stationConf.ssid, "Fortaleza da Solidao");
	//os_sprintf(stationConf.ssid, "INTERNET");
	os_sprintf(stationConf.ssid, "WIFI_RAO_USERS");
//	os_sprintf(stationConf.password, "97566152");
	os_sprintf(stationConf.password, "tti@14rao");
	wifi_station_set_config_current(&stationConf);
	wifi_set_event_handler_cb(wifi_handle_event_cb);
    wifi_set_broadcast_if (STATIONAP_MODE);



    wifi_station_disconnect();
	wifi_station_connect();
	wifi_set_sleep_type(LIGHT_SLEEP_T);  


    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO4);
  //  PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO15);
    
  //  os_timer_setfn(&testTM,	(os_timer_func_t*)test,NULL);
    


    os_timer_setfn(&testTM2, (os_timer_func_t*)test2,NULL);
    os_timer_arm(&testTM2,1000,true);
}


