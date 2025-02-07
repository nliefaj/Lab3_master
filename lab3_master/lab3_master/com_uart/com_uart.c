/*
 * com_uart.c
 *
 * Created: 2/6/2025 8:39:48 AM
 *  Author: njfg0
 */ 
#include "com_uart.h"
void initUART9600(void){
	//configurar pines tx y rx
	DDRD &=~(1<<DDD0);//entrada
	DDRD|=(1<<DDD1);//salida Tx
	
	//configurar registro A modo fast u2x0 en 1
	UCSR0A=0;
	UCSR0A|=(1<<U2X0);
	
	//configurar registro B, habilotar isr rx, bits para habilitar RX y TX
	UCSR0B=0;
	UCSR0B|=(1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	//Configurar registro C<Frame asincrono 8 bits de datos sin pariedad, 1 bit de stop
	UCSR0C=0;
	UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
	
	//baudrate = 207 con % de error igual a 0.16% con 9600
	UBRR0=207;
	
}