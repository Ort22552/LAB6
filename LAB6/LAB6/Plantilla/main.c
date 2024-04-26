/*
 * Lab6.c
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
const uint8_t Tabla[] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57};
volatile uint8_t Terminal=0; 
volatile uint8_t Respuesta=0; 
volatile uint8_t Estado=0; 
volatile uint8_t Potencimetro=0;
volatile uint8_t PotencimetroC=0;
volatile uint8_t PotencimetroD=0;
volatile uint8_t PotencimetroU=0;
uint8_t i;
/*
	------------------------------------------------------
	DEFINICION DE FUNCIONES
	------------------------------------------------------
*/
void Setup(void);
void MandarC(char * Caracter);
void MandarP(char Letra);
void Menu(void);

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
	Menu();

    while (1) 
    {
		
    }
}

/*
	------------------------------------------------------
	PROGRAMACION DE FUNCIONES
	------------------------------------------------------
*/
void Setup(void){
	SetupSalida(C, PD0);
	SetupSalida(C, PD1);
	SetupSalida(B, Todo);
	SetupAUART(Normal, 9600, Ninguno, 8);
	SetupADC(PC6);
}

void MandarC(char * Caracter){
	for (i=0; Caracter[i]!= '\0';i++)
	{
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = Caracter[i];
	}
}

void MandarP(char Letra){
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = Letra;
}

void Menu(void){
	MandarC("Seleccione una accion");
	MandarP(10);
	MandarP(13);
	MandarC("[1] Leer Potenciometro");
	MandarP(10);
	MandarP(13);
	MandarC("[2] Mostrar Caracter");
	MandarP(10);
	MandarP(13);
}
/*
	------------------------------------------------------
	INTERRUPCIONES
	------------------------------------------------------
*/
ISR(USART_RX_vect){
	switch (Estado)
	{
	case 0:
		Terminal = UDR0;
		switch (Terminal)
		{
		case 49:
			Estado = 1;
			MandarC("El valor del potenciometro es ");
			ADCSRA |= (1<<ADSC);
			break;
		case 50:
			MandarC("Ingrese un caracter ");
			MandarP(10);
			MandarP(13);
			Estado = 2;
			break;
		default:
			MandarC("Valor no Admitible");
			MandarP(10);
			MandarP(13);
			Menu();
			break;
		}
		break;
	case 1:
		break;
	case 2:
		Respuesta = UDR0;
		MandarP(Respuesta);
		MandarP(10);
		MandarP(13);
		Menu();
		Estado=0;
		PORTB = (Respuesta & 0b00111111);
		PORTC = (Respuesta >>6);
		break;
	}
}

ISR(ADC_vect){
	Potencimetro = ADCH;
	PotencimetroC = (Potencimetro/100) ;
	PotencimetroD = ((Potencimetro-(PotencimetroC*100))/10);
	PotencimetroU = (Potencimetro-((PotencimetroC*100)+(PotencimetroD*10)));
	MandarP(Tabla[PotencimetroC]);
	MandarP(Tabla[PotencimetroD]);
	MandarP(Tabla[PotencimetroU]);
	MandarP(10);
	MandarP(13);
	Menu();
	Estado=0;
	PORTB = (Potencimetro & 0b00111111);
	PORTC = (Potencimetro >>6);
	ADCSRA |= (1<<ADIF);
}