// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       SPI_Slave.ino
    Created:	28/10/2018 10:54:13
    Author:     Samuel Costas
*/

#include <SPI.h>
char buf[100]; //Es un array de car�cteres de 100 posiciones en este caso, se podr�a almacenar una palabra de 99 letras Ej. char label[10] = "Single" = | S | i | n | g | l | e | \0 |   |   |   |
volatile byte pos; /*volatile hace que el compilador genere c�digo que siempre lee la variable desde la RAM y no "almacena en cach�" el �ltimo valor le�do en un registro.
Volatile SIEMPRE debe usarse en cualquier variable que pueda modificarse por una interrupci�n, o cualquier fuente externa. 
Por ejemplo, las direcciones de registro de chips (por ejemplo, el registro de estado en un UART u otro chip de comunicaciones) tambi�n deben declararse como vol�tiles,
de modo que el compilador siempre lea el registro f�sico.*/
volatile boolean recibido;

void setup()
{
	Serial.begin(9600);  //iniciamos comunicaci�n con el pc

	// Se pone SPI en slave mode
	SPCR |= _BV(SPE); //Esto pone a 1 el bit 6 del SPI Control register (SPCR) este bit a 1 inicia SPI, por defecto el bit 4(MSTR) est� vac�o(0) si se deja de este modo lo que indica
	//que el dispositivo es esclavo

	// Se pone el pin MISO como salida por ser esclavo
	pinMode(MISO, OUTPUT);

	// get ready for an interrupt 
	pos = 0;   //posici�n cero de los byte recibidos
	recibido = false; //iniciamos como que no hemos recibido nada

	// Se activa la interrupci�n del SPI 
	SPI.attachInterrupt(); /*Las rutinas de interrupci�n se programan para gestionar eventos de interrupci�n predefinidos,
	no son invocadas por el programa principal, si no que son invocadas cuando ocurre el evento de interrupci�n correspondiente�
	permite ejecutar una serie de instrucciones ante eventos de interrupci�n*/
}  


// SPI rutina de interrupci�n
ISR(SPI_STC_vect) // SPI_STC_vect = SPI Serial Transfer Complete, se lanza cuando se completa la trasferencia
{
	byte c = SPDR;  // Se obtiene el byte del SPI Data Register, SPDR � SPI Data Register � The SPI Data Register is the read/write register where the actual data transfer takes place.

	  // a�adir al buffer
	if (pos < (sizeof(buf) - 1)) //Se comprueba que el valor de posici�n es menor que el tama�o del bufer menos 1 pues la ultima posicion del bufer es para ' \0'
		buf[pos++] = c; //se almacena el byte c en buf[pos++]

	// example: newline means time to process buffer
	if (c == '\n')
		recibido = true; //se pone recibido a true para indicar que se recibio toda la informaci�n del registro de datos SPDR

}  // end of interrupt routine SPI_STC_vect

// bucle principal: espera a que se establezca el indicador en la rutina de interrupci�n
void loop()
{
	if (recibido)
	{
		buf[pos] = 0;
		Serial.println(buf);
		pos = 0;
		recibido = false;
	}  // end of flag set

}  // end of loop
