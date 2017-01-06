

http://www.ermicro.com/blog/?p=1846

MPLab example
http://www.studentcompanion.co.za/pic-microcontroller-communication-with-spi-bus-xc8/


SPI Functions
https://class.ee.washington.edu/475/peckol/code/Microchip/microchipExamples/doc/periph-lib/SPI.htm


http://www.microchip.com/ParamChartSearch/chart.aspx?branchID=1002&mid=10&lang=en&pageId=74
16f1829-i/p

Registers:

SSPSTAT
{
	SMP - Define a posição onde sera lida a informação na onda, 1 para no final, 0 no meio, para escravos sempre 0
	CKE - Formato da transmição, 0 na descida da onda, 1 na subida da onda
	D/A - Não usado
	P   - Não usado
	S   - Não usado
	R/W - Não usado
	UA  - Não usado
	BF  - Buffer de full status, 0 transmição não completa, 1 transmição completa
}

SSPCON1
{
	WCOL  - Detector de colisão do buffer
	SSPOV - Detector de overflow, apenas acontece no escravo
	SSPEN - Ativa o funcionamento sincrono das porta serial
	CKP   - Polaridade do clock, 0 descanço no level baixo, 1 descanço no level alto
	SSPM3 - Modo de funcionamento do modo serial
	SSPM2 - Modo de funcionamento do modo serial
	SSPM1 - Modo de funcionamento do modo serial
}

SSPCON2
{
	GCEN    - Não usado
	ACKSTAT - Não usado
	ACKEN   - Não usado
	RCEN    - Não usado
	PEN     - Não usado
	RSEN    - Não usado
	SEN     - Não usado
}


        if (SPI1_IsBufferFull()) {
            ReceiverBuffer = SPI1_Exchange8bit(DUMMY_DATA);
            SPI1_Exchange8bit(SendBuffer);
        }
        else {
            ReceiverBuffer = SPI1_Exchange8bit(SendBuffer);
        }