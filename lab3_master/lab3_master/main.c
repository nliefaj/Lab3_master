/*
 * lab3_master.c
 *
 * Created: 2/6/2025 7:24:20 PM
 * Author : njfg0
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "SPI/SPI.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "com_uart/com_uart.h"


uint8_t valorpot1=0;
uint8_t valorpot2=0;
uint8_t flag=1;
uint8_t leer_uart=0;
volatile char buffRX;
char cadena[4];
char *wordRX;
char *inicio_wordRX;
uint8_t num_Uart = 255;

void refreshPort(uint8_t valor);
void writetxtUART(char* texto);

int main(void)
{
	cli();
	initUART9600();
	DDRD|=0xFC;
	DDRB|=0x03;
	sei();
	DDRB|=(1<<DDB0)|(1<<DDB1);
	spiInit(SPI_MASTER_OSC_DIV16,SPI_DATA_ORDER_MSB,SPI_CLOCK_IDLE_LOW,SPI_CLOCK_FIRST_EDGE);
	wordRX = cadena;
	inicio_wordRX = wordRX;
    /* Replace with your application code */
    while (1) 
    {
		PORTB &=~(1<<PORTB2);
		spiWrite('0');
		valorpot2=spiRead();
		_delay_ms(250);
		PORTB|=(1<<PORTB2);
		
		PORTB &=~(1<<PORTB2);
		spiWrite('1');
		valorpot1=spiRead();
		_delay_ms(250);
		PORTB|=(1<<PORTB2);
		
		
		float mpot1=(valorpot1*5.0)/255.0;
		float mpot2=(valorpot2*5.0)/255.0;
		
		int enterov1=(int)mpot1;
		int decimalv1=(int)((mpot1-enterov1)*100);
		
		int enterov2=(int)mpot2;
		int decimalv2=(int)((mpot2-enterov2)*100);
		
		char mensaje1[5];
		mensaje1[0]='0'+enterov1;
		mensaje1[1]='.';
		mensaje1[2]='0'+(decimalv1/10);
		mensaje1[3]='0'+(decimalv1%10);
		mensaje1[4]='V';
		mensaje1[5]='\0';
		
		char mensaje[100];
		strcpy(mensaje,"Potenciometro 1:");
		strcat(mensaje,mensaje1);
		
		char mensaje2[5];
		mensaje2[0]='0'+enterov2;
		mensaje2[1]='.';
		mensaje2[2]='0'+(decimalv2/10);
		mensaje2[3]='0'+(decimalv2%10);
		mensaje2[4]='V';
		mensaje2[5]='\0';
		
		strcat(mensaje," Potenciometro 2:");
		strcat(mensaje,mensaje2);
		strcat(mensaje,"\n");
		writetxtUART(mensaje);
		
		if (leer_uart==1){
			num_Uart = atoi(wordRX);
			//writetxtUART(wordRX);
			wordRX[0] = '\0';
			PORTB &=~(1<<PORTB2);
			spiWrite('a');
			spiRead();
			_delay_ms(250);
			PORTB|=(1<<PORTB2);
			PORTB &=~(1<<PORTB2);
			spiWrite(num_Uart);
			spiRead();
			_delay_ms(250);
			PORTB|=(1<<PORTB2);
			leer_uart=0;
		}
		PORTD=((PORTD&0x03)|((num_Uart<<2)&0xFC));
		PORTB=((PORTB&0xFC)|((num_Uart>>6)&0x03));
		
    }
}
void writetxtUART(char* texto){
	uint8_t i;
	for (i=0;texto[i]!='\0';i++){
		while(!(UCSR0A&(1<<UDRE0)));//esperar hasta que el udre0 est� en 1
		UDR0=texto[i];//cuando i nulo se acaba
		flag=0;
	}
}

/*void stringToInt(const char *str){
	while (*str) {
		char car = *str;
		
	}
}*/

ISR(USART_RX_vect){
	buffRX=UDR0;
	if ((buffRX == '\n')||(buffRX == '\r')) { //(buffRX==0)||
		leer_uart = 1;
		*wordRX = '\0';
		wordRX++;
		wordRX = inicio_wordRX;
	} else {
		*wordRX = buffRX;
		wordRX++;
	}
}