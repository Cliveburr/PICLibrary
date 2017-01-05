

http://www.ermicro.com/blog/?p=1846

MPLab example
http://www.studentcompanion.co.za/pic-microcontroller-communication-with-spi-bus-xc8/


SPI Functions
https://class.ee.washington.edu/475/peckol/code/Microchip/microchipExamples/doc/periph-lib/SPI.htm


http://www.microchip.com/ParamChartSearch/chart.aspx?branchID=1002&mid=10&lang=en&pageId=74
16f1829-i/p



 /*
 ** test.c
 ** SPI test program for PIC18F4520 and nRF24L01 or nRF24L01+
 ** Checks SPI comms between PIC and wireless chip
 ** 
 ** RA0    LED (output)
 ** RA1    PB (input)
 */
 
 #include <p18f4520.h>
 #include <spi.h>
 
 //function prototypes
 unsigned char spi_Send_Read(unsigned char);
 void dly(void);
 
 // Defines
 #define SPI_SCK        LATCbits.LATC3        // Clock pin, PORTC pin 3 
 #define SPI_SO        LATCbits.LATC5        // Serial output pin, PORTC pin 5 
 #define SPI_SI        PORTCbits.RC4        // Serial input pin, PORTC pin 4 
 #define SPI_CSN        LATCbits.LATC2        // CSN output pin, PORTC pin 2
 #define SPI_CE        LATCbits.LATC1        // CE output pin, PORTC pin 1
 #define SPI_IRQ        PORTBbits.RB0        // IRQ input pin, PORTB pin 0
 #define SPI_SCALE    4                      // postscaling of signal 
 #define LED            LATAbits.LATA0
 #define PB            PORTAbits.RA1
 
 
 // Macros
 #define nop() _asm nop _endasm
 
 void main(void)
 {
     unsigned char status = 0;
     unsigned char data[5];
     int i;
 
     // run internal oscillator at 8 MHz
     OSCCON = OSCCON | 0b01110000;
     while (!OSCCONbits.IOFS)    // wait for IOFS to go high
         ;
 
     OpenSPI(SPI_FOSC_16, MODE_00, SMPMID); //open SPI1
     PORTA = 0x00;
     ADCON1 = 0x0F;        // set up PORTA to be digital I/Os
     TRISA = 0x02;        // PORTA<7.2,0> outputs PORTA<1> input
     TRISCbits.TRISC3 = 0;    // SDO output
     TRISCbits.TRISC5 = 0;   // SCK output
     TRISCbits.TRISC2 = 0;    // CSN output
     TRISCbits.TRISC1 = 0;    // CE output
     SPI_CSN = 1;        // CSN high
     SPI_SCK = 0;        // SCK low
     SPI_CE    = 0;        // CE low
     nop();
 
     //write TX_ADDRESS register
     SPI_CSN = 0;            //CSN low
     spi_Send_Read(0x30);
     spi_Send_Read(0x11);
     spi_Send_Read(0x22);
     spi_Send_Read(0x33);
     spi_Send_Read(0x44);
     spi_Send_Read(0x55);
     SPI_CSN = 1;            //CSN high
 
 
     //read TX_ADDRESS register
     //Check that values are correct using the MPLAB debugger
     SPI_CSN = 0;            //CSN low
     status = spi_Send_Read(0x10);
     data[0] = spi_Send_Read(0x00);    // 0x11
     data[1] = spi_Send_Read(0x00);    // 0x22
     data[2] = spi_Send_Read(0x00);    // 0x33
     data[3] = spi_Send_Read(0x00);    // 0x44
     data[4] = spi_Send_Read(0x00);    // 0x55
     SPI_CSN = 1;                    // CSN high
 
     while (1)
     {
         LED = 1;
         dly();
         LED = 0;
         dly();
     }
 }
 
 
 unsigned char spi_Send_Read(unsigned char byte)
 {
     SSPBUF = byte;    
     while(!DataRdySPI())
         ;    
     return SSPBUF;
 }
 
 void dly(void)
 {
     unsigned int i;
 
     for (i = 0; i < 50000; i++)
         ;
 }    
 