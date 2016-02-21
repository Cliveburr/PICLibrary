/*
 * File:   main.c
 * Author: Clive
 *
 * Created on 20 de Fevereiro de 2016, 16:55
 */

#include "system.h"
#include <system_config.h>

#include "usb.h"
#include "usb_device_hid.h"

#include "usb_app.h"

void t_delay_100ms(int n);

void main(void) {
    SYSTEM_Initialize();

    POWERON = 1;
    t_delay_100ms(50);
    POWERON = 0;
    t_delay_100ms(50);
    POWERON = 1;

    USBDeviceInit();
    LedControl_Init();

    while(1)
    {
        if (USBGETSIGNAL)
        {
            USBDeviceAttach();
            USBSTATE = 1;
            
            while (USBGETSIGNAL)
            {
                //char tt = USBGetDeviceState();
                
                if (USBGetDeviceState() >= CONFIGURED_STATE && !USBIsDeviceSuspended())
                {
                    APP_DeviceTasks();
                }
                
                SYSTEM_Task();
            }
            
            USBDeviceDetach();
            USBSTATE = 0;
        }
        
        SYSTEM_Task();
    }   
}

void t_delay_100ms(int n)
{
    while(n--)
    {
        __delaywdt_ms(10);
    }
}