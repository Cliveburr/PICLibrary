/*
 * File:   main.c
 * Author: AManzoli
 *
 * Created on 5 de Janeiro de 2017, 16:16
 */

#include <xc.h>
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/spi1.h"

#define _XTAL_FREQ 32000

typedef struct {
  unsigned char Input0: 1;
  unsigned char Input1: 1;
} ButtonsStruct;

ButtonsStruct Buttons;
void ToggleLed0();
void ToggleLed1();

unsigned char SendBuffer;
unsigned char ReceiveBuffer;

void main(void) {
    
    SYSTEM_Initialize();
    
    //SPI1_Initialize();
    
    Led0_SetHigh();
    __delay_ms(500);
    Led0_SetLow();
    Led1_SetHigh();
    __delay_ms(500);
    Led1_SetLow();
    
    while (1) {
    
        if (!Input0_GetValue() && !Buttons.Input0) {
            SendBuffer = '3';
            Buttons.Input0 = 1;
            //ToggleLed0();
            //Slave0_SetLow();
            //SPI1_Exchange8bit('3');
            //Slave0_SetHigh();
        }
        else if (Input0_GetValue() && Buttons.Input0) {
            Buttons.Input0 = 0;
        }
        
        if (!Input1_GetValue() && !Buttons.Input1) {
            SendBuffer = '4';
            Buttons.Input1 = 1;
            //ToggleLed1();
            //Slave0_SetLow();
            //SPI1_Exchange8bit('4');
            //Slave0_SetHigh();
        }
        else if (Input1_GetValue() && Buttons.Input1) {
            Buttons.Input1 = 0;
        }
        
        //Slave0_SetLow();
        //__delay_ms(1);
        ReceiveBuffer = SPI1_Exchange8bit(SendBuffer);
        //Slave0_SetHigh();
        //SPI1_Exchange8bitBuffer(&SendBuffer, 1, &ReceiveBuffer);
        
        switch(ReceiveBuffer) {
            case '5': ToggleLed0(); break;
            case '6': ToggleLed1(); break;
        }
        
        ReceiveBuffer = SendBuffer = DUMMY_DATA;
    }
    
    return;
}

void ToggleLed0() {
    Led0_Toggle();
}

void ToggleLed1() {
    Led1_Toggle();
}