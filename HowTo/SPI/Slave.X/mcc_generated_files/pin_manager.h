/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.0
        Device            :  PIC16F1829
        Version           :  1.01
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set Input0 aliases
#define Input0_TRIS               TRISAbits.TRISA0
#define Input0_LAT                LATAbits.LATA0
#define Input0_PORT               PORTAbits.RA0
#define Input0_WPU                WPUAbits.WPUA0
#define Input0_ANS                ANSELAbits.ANSA0
#define Input0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define Input0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define Input0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define Input0_GetValue()           PORTAbits.RA0
#define Input0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define Input0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define Input0_SetPullup()      do { WPUAbits.WPUA0 = 1; } while(0)
#define Input0_ResetPullup()    do { WPUAbits.WPUA0 = 0; } while(0)
#define Input0_SetAnalogMode()  do { ANSELAbits.ANSA0 = 1; } while(0)
#define Input0_SetDigitalMode() do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set Input1 aliases
#define Input1_TRIS               TRISAbits.TRISA1
#define Input1_LAT                LATAbits.LATA1
#define Input1_PORT               PORTAbits.RA1
#define Input1_WPU                WPUAbits.WPUA1
#define Input1_ANS                ANSELAbits.ANSA1
#define Input1_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define Input1_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define Input1_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define Input1_GetValue()           PORTAbits.RA1
#define Input1_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define Input1_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define Input1_SetPullup()      do { WPUAbits.WPUA1 = 1; } while(0)
#define Input1_ResetPullup()    do { WPUAbits.WPUA1 = 0; } while(0)
#define Input1_SetAnalogMode()  do { ANSELAbits.ANSA1 = 1; } while(0)
#define Input1_SetDigitalMode() do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set Led0 aliases
#define Led0_TRIS               TRISAbits.TRISA2
#define Led0_LAT                LATAbits.LATA2
#define Led0_PORT               PORTAbits.RA2
#define Led0_WPU                WPUAbits.WPUA2
#define Led0_ANS                ANSELAbits.ANSA2
#define Led0_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define Led0_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define Led0_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define Led0_GetValue()           PORTAbits.RA2
#define Led0_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define Led0_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define Led0_SetPullup()      do { WPUAbits.WPUA2 = 1; } while(0)
#define Led0_ResetPullup()    do { WPUAbits.WPUA2 = 0; } while(0)
#define Led0_SetAnalogMode()  do { ANSELAbits.ANSA2 = 1; } while(0)
#define Led0_SetDigitalMode() do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set RB4 procedures
#define RB4_SetHigh()    do { LATBbits.LATB4 = 1; } while(0)
#define RB4_SetLow()   do { LATBbits.LATB4 = 0; } while(0)
#define RB4_Toggle()   do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RB4_GetValue()         PORTBbits.RB4
#define RB4_SetDigitalInput()   do { TRISBbits.TRISB4 = 1; } while(0)
#define RB4_SetDigitalOutput()  do { TRISBbits.TRISB4 = 0; } while(0)
#define RB4_SetPullup()     do { WPUBbits.WPUB4 = 1; } while(0)
#define RB4_ResetPullup()   do { WPUBbits.WPUB4 = 0; } while(0)
#define RB4_SetAnalogMode() do { ANSELBbits.ANSB4 = 1; } while(0)
#define RB4_SetDigitalMode()do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RB6 procedures
#define RB6_SetHigh()    do { LATBbits.LATB6 = 1; } while(0)
#define RB6_SetLow()   do { LATBbits.LATB6 = 0; } while(0)
#define RB6_Toggle()   do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define RB6_GetValue()         PORTBbits.RB6
#define RB6_SetDigitalInput()   do { TRISBbits.TRISB6 = 1; } while(0)
#define RB6_SetDigitalOutput()  do { TRISBbits.TRISB6 = 0; } while(0)
#define RB6_SetPullup()     do { WPUBbits.WPUB6 = 1; } while(0)
#define RB6_ResetPullup()   do { WPUBbits.WPUB6 = 0; } while(0)

// get/set Led1 aliases
#define Led1_TRIS               TRISCbits.TRISC0
#define Led1_LAT                LATCbits.LATC0
#define Led1_PORT               PORTCbits.RC0
#define Led1_WPU                WPUCbits.WPUC0
#define Led1_ANS                ANSELCbits.ANSC0
#define Led1_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define Led1_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define Led1_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define Led1_GetValue()           PORTCbits.RC0
#define Led1_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define Led1_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define Led1_SetPullup()      do { WPUCbits.WPUC0 = 1; } while(0)
#define Led1_ResetPullup()    do { WPUCbits.WPUC0 = 0; } while(0)
#define Led1_SetAnalogMode()  do { ANSELCbits.ANSC0 = 1; } while(0)
#define Led1_SetDigitalMode() do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()    do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()   do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()   do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()         PORTCbits.RC7
#define RC7_SetDigitalInput()   do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()  do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()     do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()   do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode() do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/