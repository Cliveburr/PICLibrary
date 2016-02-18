/* 
 * File:   SoftUART.c
 * Version: 1.0
 * Author: Clive
 *
 * Created on 13 de Fevereiro de 2016, 18:54
 */

#include <htc.h>
#include <string.h>

#include "SoftUART.h"

void InitSoftUart()
{
    #ifdef _HTC_H_
        uT1CKPS = 0;   // Timer1 Input Clock Prescale Select bits 0 = 1:1
        uTMR1CS = 0;   // Timer1 Clock Source Select bit 0 = Internal clock (FOSC/4)
    #endif

    #ifdef SoftUartChannel0
        unsigned short v = 65535 - BDC0 + 10;
        uc0h = v >> 8;
        uc0l = v;
        v = 65535 - HBDC0;
        uc0hh = v >> 8;
        uc0hl = v;
        
        TXC0 = SBC0;
    #endif

    #ifdef SoftUartChannel1
        unsigned short v = 65535 - BDC1 + 10;
        uc1h = v >> 8;
        uc1l = v;
        v = 65535 - HBDC1;
        uc1hh = v >> 8;
        uc1hl = v;
        
        TXC1 = SBC1;
    #endif

    #ifdef DebugRC0
        DebugRC0 = 1;
    #endif

    #ifdef DebugRC1
        DebugRC1 = 1;
    #endif
}

void uwrite_size(char channel, char *data, char size)
{ 
    char tml = 0, tmh = 0, i = 0;
    
    switch(channel)
    {
        #ifdef SoftUartChannel0
        case 0: tml = uc0l; tmh = uc0h; break;
        #endif
        #ifdef SoftUartChannel1
        case 1: tml = uc1l; tmh = uc1h; break;
        #endif
    }
 
    uTMR1L = tml;
    uTMR1H = tmh;
    uTMR1IF = 0;
    uTMR1ON = 1;

    while(i < size)
    {
        char d = data[i], b = 8;
        
        switch(channel)
        {
            #ifdef SoftUartChannel0
            case 0: TXC0 = !SBC0; break;
            #endif
            #ifdef SoftUartChannel1
            case 1: TXC1 = !SBC1; break;
            #endif
        }

        while(!uTMR1IF);

        uTMR1ON = 0;
        uTMR1L = tml;
        uTMR1H = tmh;
        uTMR1IF = 0;
        uTMR1ON = 1;

        while(b)
        {
            char v = d & 1 << 0;
            switch(channel)
            {
                #ifdef SoftUartChannel0
                case 0: TXC0 = v; break;
                #endif
                #ifdef SoftUartChannel1
                case 1: TXC1 = v; break;
                #endif
            }

            d = (d >> 1);
            b--;

            while (!uTMR1IF);
            uTMR1ON = 0;
            uTMR1L = tml;
            uTMR1H = tmh;
            uTMR1IF = 0;
            uTMR1ON = 1;
        }

        switch(channel)
        {
            #ifdef SoftUartChannel0
            case 0: TXC0 = SBC0; break;
            #endif
            #ifdef SoftUartChannel1
            case 1: TXC1 = SBC1; break;
            #endif
        }

        i++;
        
        while(!uTMR1IF);
        uTMR1ON = 0;
        uTMR1L = tml;
        uTMR1H = tmh;
        uTMR1IF = 0;
        uTMR1ON = 1;
    }
    
    uTMR1ON = 0;
}

void uwrite_array(char channel, char* data)
{
    uwrite_size(channel, data, strlen(data));
}

void uwrite_byte(char channel, char data)
{
    uwrite_size(channel, &data, 1);
}

char uread_array(char channel, char *data, char size)
{
    switch(channel)
    {
        #ifdef SoftUartChannel0
        case 0: if(RXC0 == SBC0) return 0; break;
        #endif
        #ifdef SoftUartChannel1
        case 1: if(RXC1 == SBC1) return 0; break;
        #endif
    }

    char tml = 0, tmh = 0, i = 0, c = 1;
    
    #ifdef DebugRX
    DebugRX = 0;
    #endif

    switch(channel)
    {
        #ifdef SoftUartChannel0
        case 0: tml = uc0l; tmh = uc0h; uTMR1L = uc0hl; uTMR1H = uc0hh; break;
        #endif
        #ifdef SoftUartChannel1
        case 1: tml = uc1l; tmh = uc1h; uTMR1L = uc1hl; uTMR1H = uc1hh; break;
        #endif
    }
    
    uTMR1IF = 0;
    uTMR1ON = 1;

    while(!uTMR1IF);
    uTMR1ON = 0;
    uTMR1L = tml;
    uTMR1H = tmh;
    uTMR1IF = 0;
    uTMR1ON = 1;

    while(c && i < size)
    {
        char d = 0, b = 8;

        while(b)
        {
            d = (d >> 1);

            #ifdef DebugRX
            DebugRX = ~DebugRX;
            #endif

            char v = 0;
            switch(channel)
            {
                #ifdef SoftUartChannel0
                case 0: v = RXC0 == SBC0; break;
                #endif
                #ifdef SoftUartChannel1
                case 1: v = RXC1 == SBC1; break;
                #endif
            }  
            if(v)
                d |= 1 << 7;

            b--;

            while (!uTMR1IF);
            uTMR1ON = 0;
            uTMR1L = tml;
            uTMR1H = tmh;
            uTMR1IF = 0;
            uTMR1ON = 1;
        }

        while(!uTMR1IF);
        uTMR1ON = 0;
        uTMR1L = tml;
        uTMR1H = tmh;
        uTMR1IF = 0;
        uTMR1ON = 1;
        
        data[i] = d;
        i++;

        #ifdef DebugRX
        DebugRX = 1;
        #endif

        switch(channel)
        {
            #ifdef SoftUartChannel0
            case 0: c = RXC0 != SBC0; break;
            #endif
            #ifdef SoftUartChannel1
            case 1: c = RXC1 != SBC1; break;
            #endif
        }
        
        while(!uTMR1IF);
        uTMR1ON = 0;
        uTMR1L = tml;
        uTMR1H = tmh;
        uTMR1IF = 0;
        uTMR1ON = 1;
    }
    
    uTMR1ON = 0;
 
    return i;
}

