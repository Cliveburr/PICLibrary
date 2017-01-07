/*
 * File:   main.c
 * Author: AManzoli
 *
 * Created on 5 de Janeiro de 2017, 16:16
 */

#include <xc.h>
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/ext_int.h"

#define _XTAL_FREQ 32000

typedef struct {
  unsigned char Input0: 1;
  unsigned char Input1: 1;
} ButtonsStruct;

ButtonsStruct Buttons;
void ProcessReceive(unsigned char msg);
void ToggleLed0();
void ToggleLed1();

unsigned char SendBuffer;
unsigned char HasSend;
unsigned char ReceiveBuffer;
unsigned char OldReceiveBuffer;

void SlaveInt() {
    ReceiveBuffer = SPI1_Exchange8bit(SendBuffer);
    ProcessReceive(ReceiveBuffer);
    SendBuffer = DUMMY_DATA;
    //Led0_Toggle();
}

void main(void) {
    
    SYSTEM_Initialize();
    
    //INTERRUPT_GlobalInterruptEnable();
    //INTERRUPT_PeripheralInterruptEnable();
    //INT_SetInterruptHandler(SlaveInt);
    
    //SPI1_Initialize();
    
    Led0_SetHigh();
    __delay_ms(500);
    Led0_SetLow();
    Led1_SetHigh();
    __delay_ms(500);
    Led1_SetLow();
    
    while (1) {
    
        if (!Input0_GetValue() && !Buttons.Input0) {
            SendBuffer = '5';
            HasSend = 1;
            Buttons.Input0 = 1;
            //ToggleLed0();
            //SPI1_Exchange8bit('5');
        }
        else if (Input0_GetValue() && Buttons.Input0) {
            Buttons.Input0 = 0;
        }
        
        if (!Input1_GetValue() && !Buttons.Input1) {
            SendBuffer = '6';
            HasSend = 1;
            Buttons.Input1 = 1;
            //ToggleLed1();
            //SPI1_Exchange8bit('6');
        }
        else if (Input1_GetValue() && Buttons.Input1) {
            Buttons.Input1 = 0;
        }
        
        if (SPI1_IsBufferFull()) {
            ReceiveBuffer = SSP1BUF;
            SSP1BUF = SendBuffer;
        //    ReceiveBuffer = SPI1_Exchange8bit(SendBuffer);
            SendBuffer = DUMMY_DATA;
            ProcessReceive(ReceiveBuffer);
        }
        //else if (HasSend) {
        //    SSP1BUF = SendBuffer;
        //    HasSend = 0;
       // }
        //else {
        //    ReceiveBuffer = SPI1_Exchange8bit(SendBuffer);
        //}

        //ReceiveBuffer = SPI1_Exchange8bit(SendBuffer);
        //SPI1_Exchange8bitBuffer(&SendBuffer, 1, &ReceiveBuffer);
        
        //ProcessReceive(ReceiveBuffer);
        
        //ReceiveBuffer = SendBuffer = DUMMY_DATA;
    }
    
    return;
}



void ProcessReceive(unsigned char msg) {
    switch(msg) {
        case '3': ToggleLed0(); break;
        case '4': ToggleLed1(); break;
    }
}

void ToggleLed0() {
    Led0_Toggle();
}

void ToggleLed1() {
    Led1_Toggle();
}