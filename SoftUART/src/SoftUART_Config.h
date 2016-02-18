/* 
 * File:   SoftUART_Config.h
 * Author: Clive
 *
 * Created on 13 de Fevereiro de 2016, 21:03
 */

#ifndef SOFTUART_CONFIG_H
#define	SOFTUART_CONFIG_H

#define SoftUartChannel0
#define TXC0 PORTAbits.RA0          // TX GPIO for the channel 0, write
#define RXC0 PORTAbits.RA1          // RX GPIO for the channel 0, read
#define SBC0 1                      // Singal mode for the channel 0, 1 = Normal or 0 = Inverted
#define BDC0 260                    // Baud setting for the channel 0, (_XTAL_FREQ / 4) / BAUDRATE
#define HBDC0 250 // 310                   // Baud and half

#define SoftUartChannel1
#define TXC1 PORTBbits.RB2          // TX GPIO for the channel 1, write
#define RXC1 PORTBbits.RB1          // RX GPIO for the channel 1, read
#define SBC1 1                      // Singal mode for the channel 1, 1 = Normal or 0 = Inverted
#define BDC1 520                    // Baud setting for the channel 1, (_XTAL_FREQ / 4) / BAUDRATE
#define HBDC1 520 //730                   // Baud and half

#define DebugRX PORTAbits.RA2

#endif	/* SOFTUART_CONFIG_H */

