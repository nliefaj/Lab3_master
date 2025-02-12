/*
 * SPI.c
 *
 * Created: 2/6/2025 7:28:12 PM
 *  Author: njfg0
 */ 
#include "SPI.h"

void spiInit(Spi_Type sType, Spi_Data_Order sDataOrder, Spi_Clock_Polarity sClockPolarity, Spi_Clock_Phase sClockPhase){
	//PB2=SS
	//PB3=MOSI
	//PB4=MISO
	//PB5=SCK
	
	if (sType&(1<<MSTR)){
		DDRB|=(1<<DDB3)|(1<<DDB5)|(1<<DDB2);
		DDRB&=~(1<<DDB4);
		SPCR|=(1<<MSTR);
		
		uint8_t temp=sType&0b00000111;
		switch(temp){
			case 0://DIV2
				SPCR&=~((1<<SPR1)|(1<<SPR0));
				SPSR|=(1<<SPI2X);
			break;
			case 1://DIV4
				SPCR&=~((1<<SPR1)|(1<<SPR0));
				SPSR&=~(1<<SPI2X);
			break;
			case 2:
				SPCR|=(1<<SPR0);
				SPCR&=~(1<<SPR1);
				SPSR|=(1<<SPI2X);
			break;
			case 3:
				SPCR|=(1<<SPR0);
				SPCR&=~(1<<SPR1);
				SPSR&=~(1<<SPI2X);
			break;
			case 4:
				SPCR&=~(1<<SPR0);
				SPCR|=(1<<SPR1);
				SPSR|=(1<<SPI2X);
			break;
			case 5:
				SPCR&=~(1<<SPR0);
				SPCR|=(1<<SPR1);
				SPSR&=~(1<<SPI2X);
			break;
			case 6://DIV128
				SPCR|=((1<<SPR1)|(1<<SPR0));
				SPSR&=~(1<<SPI2X);
			break;
		}
	}else{//Slave mode
		DDRB|=(1<<DDB4);
		DDRB&=~((1<<DDB3)|(1<<DDB5)|(1<<DDB2));
		SPCR&=~(1<<MSTR);
	} 
	SPCR|=(1<<SPE)|sDataOrder|sClockPolarity|sClockPhase;
}

static void spiReceiveWait(){//espera a recibir la data completa
	while(!(SPSR&(1<<SPIF)));
}

void spiWrite(uint8_t dat){
	SPDR=dat;//envia la data al bus 
	while(!(SPSR&(1<<SPIF))); //espera a que se termine de mandar
}

unsigned spiDataReady(){
	if(SPSR &(1<<SPIF))
	return 1;
	else
	return 0;
}

uint8_t spiRead(void){
	//while(!(SPSR&(1<<SPIF)));//ESPERA A QUE SE COMPLETE DE RECIBIR TODA LA INFO
	return(SPDR);//LEE LA INFORMACIÓN
}