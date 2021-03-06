#ifndef SYSTEM_H
#define SYSTEM_H

//#include <xc.h>
//#include <stdbool.h>
#include <htc.h>

#include "fixed_address_memory.h"

#include "LedControl.h"

#define POWERON PORTBbits.RB2
#define USBSTATE PORTAbits.RA2
#define USBGETSIGNAL PORTCbits.RC2

#define _XTAL_FREQ 48000000

void SYSTEM_Initialize();

void SYSTEM_Task();

#endif //SYSTEM_H
