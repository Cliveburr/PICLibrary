/*
 * File:   main.c
 * Author: AManzoli
 *
 * Created on 5 de Janeiro de 2017, 16:16
 */

#include <xc.h>
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/spi1.h"

typedef struct {
  unsigned char Input0: 1;
  unsigned char Input1: 1;
} ButtonsStruct;

ButtonsStruct Buttons;
void ToggleLed0();
void ToggleLed1();

void main(void) {
    
    SYSTEM_Initialize();
    
    SPI1_Initialize();
    
    Led0_SetHigh();
    __delay_ms(500);
    Led0_SetLow();
    Led1_SetHigh();
    __delay_ms(500);
    Led1_SetLow();
    
    while (1) {
    
        if (Input0_GetValue() && !Buttons.Input0) {
            SPI1_Exchange8bit('1');
            Buttons.Input0 = 1;
        }
        else if (!Input0_GetValue() && Buttons.Input0) {
            Buttons.Input0 = 0;
        }
        
        if (Input1_GetValue() && !Buttons.Input1) {
            SPI1_Exchange8bit('2');
            Buttons.Input1 = 1;
        }
        else if (!Input1_GetValue() && Buttons.Input1) {
            Buttons.Input1 = 0;
        }
        
        switch(SPI1_Exchange8bit('0')) {
            case '5': ToggleLed0(); break;
            case '6': ToggleLed1(); break;
        }
    }
    
    return;
}

void ToggleLed0() {
    Led0_Toggle();
}

void ToggleLed1() {
    Led1_Toggle();
}