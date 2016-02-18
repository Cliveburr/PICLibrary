/* 
 * File:   SoftUART.h
 * Version: 1.0
 * Author: Clive
 *
 * Created on 13 de Fevereiro de 2016, 18:54
 */

/*
 * Dependencies:
 * 
 * SoftUART_Config.h
 * 
 * Need to set TRISIO for all ports, TX = 0, RX = 1
 * Need to define all channels used
 * 
 * Baud setting table:
 * _XTAL_FREQ / Baud |     9600 |     19200
 *              4MHz    104/000      52/000
 *             20MHz    520/000     260/350
 */

#ifndef SOFTUART_H
#define	SOFTUART_H

#include "SoftUART_Config.h"

#ifdef _HTC_H_
    #define uT1CKPS T1CONbits.T1CKPS
    #define uTMR1CS T1CONbits.TMR1CS
    #define uTMR1ON T1CONbits.TMR1ON
    #define uTMR1L TMR1L
    #define uTMR1H TMR1H
    #define uTMR1IF TMR1IF
#endif

void InitSoftUart();

void uwrite_size(char channel, char *data, char size);
void uwrite_array(char channel, char* data);
void uwrite_byte(char channel, char data);

char uread_array(char channel, char *data, char size);

#ifdef SoftUartChannel0
unsigned char uc0h;
unsigned char uc0l;
unsigned char uc0hh;
unsigned char uc0hl;
#endif

#ifdef SoftUartChannel1
unsigned char uc1h;
unsigned char uc1l;
unsigned char uc1hh;
unsigned char uc1hl;
#endif

#endif	/* SOFTUART_H */