/*
 * File:   main.c
 * Author: Clive
 *
 * Created on 14 de Fevereiro de 2016, 11:42
 */

#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000

#include <xc.h>
#include <string.h>

#include "SoftUART/SoftUART.h"

void main() {
    
    CMCON = 0x07;         // Shut off the Comparator
    VRCON = 0x00;         // Set no voltage reference
    //ADCON0 = 0b00000000;  // Set A/D converter
    TRISA = 0b00001010;     // Set port A as input or output, 0 output, 1 input
    PORTA = 0b00000000;     // Set value for ports
    TRISB = 0b00000010;     // Set port A as input or output, 0 output, 1 input
    PORTB = 0b00000000;     // Set value for ports

    InitSoftUart();
    
    uwrite_array(0, "Uart initialized...\r\n");

    while (0)
    {
        char b[2] = { 0 };
        if (uread_array(0, b, 2))
            uwrite_byte(0, b);
    }
    
    while (1)
    {
        char b;
        // Read from terminal
        if (uread_array(0, &b, 1))
        {
            switch(b)
            {
                case '1':
                    uwrite_array(0, "Func 1!\r\n");
                    uwrite_byte(1, '1');
                    break;
                case '2':
                    uwrite_array(0, "Func 2!\r\n");
                    uwrite_array(1, "2abcd");
                    break;
                case '3':
                    uwrite_array(0, "Func 3!\r\n");
                    uwrite_array(1, "3abcdefghijlmnopqrstuvxzwy0123456789");
                    break;
            }
        }
        
        char rec[8] = { 0 };
        // Read from another PIC
        if (uread_array(1, rec, 8))
        {
            switch(rec[0])
            {
                case '1':
                    uwrite_array(0, "Func 1 receveid!\r\n");
                    break;
                case '2':
                    uwrite_array(0, "Func 2 receveid! ");
                    if (strncmp(rec, "2abcd", 5))
                    {
                       char msg[7 + 5 + 3] = "FAIL! {";
                       strcat(msg, &rec);
                       strcat(msg, "}\r\n");
                       uwrite_array(0, msg);
                    }
                    else
                        uwrite_array(0, "OK!\r\n");
                    break;
                case '3':
                    //uread_C1_array_size(f3, 35);
                    //uwrite_C0_array("Func 3 receveid! ");
                    //if (strcmp(f3, "abcdefghijlmnopqrstuvxzwy012345678"))
                    //    uwrite_C0_array("OK!\r\n");
                    //else
                    //    uwrite_C0_array("FAIL!\r\n");
                    break;
            }
        }
    }
    
    return;
}