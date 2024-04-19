/*
 * Plantilla.c
 *
 * Created: 10/04/2024 22:05:28
 * Author : Josue Ortiz
 */ 
/*
	------------------------------------------------------
	DEFINICIONES, INCLUDES Y VARIABLES
	------------------------------------------------------
*/
#define F_CPU 16000000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include "Setups/SetupsMicro.h"
volatile uint8_t Terminal=0; 
/*
	------------------------------------------------------
	DEFINICION DE FUNCIONES
	------------------------------------------------------
*/
void Setup(void);
void MandarC(char * Caracter);
/*
	------------------------------------------------------
	MAIN-LOOP
	------------------------------------------------------
*/

int main(void)
{
    cli();
	Setup();
	sei();
	MandarC("Hola Pedro :D");
    while (1) 
    {
		PORTB = Terminal; 
    }
}

/*
	------------------------------------------------------
	PROGRAMACION DE FUNCIONES
	------------------------------------------------------
*/
void Setup(void){
	SetupSalida(D, PD0);
	SetupSalida(D, PD1);
	SetupAUART(Normal, 9600, Ninguno, 8);
}

void MandarC(char * Caracter){
	uint8_t i;
	for (i=0; Caracter[i]!=/0;i++)
	{
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = Caracter[i];
	}
}
/*
	------------------------------------------------------
	INTERRUPCIONES
	------------------------------------------------------
*/
ISR(USART_RX_vect){
	Terminal = UDR0;
}