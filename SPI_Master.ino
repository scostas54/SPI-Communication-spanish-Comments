// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       SPI_Master.ino
    Created:	28/10/2018 10:37:40
    Author:     Samuel Costas
*/

#include <SPI.h>
// set up the speed, data order and data mode

void setup()
{
	Serial.begin(9600);
	digitalWrite(SS, HIGH);  // se pone el pin ss en alto para asegurar que no se trasmite nada, ya viene definido en el arduino el pin 10 como SS

	//Al iniciar el SPI ya se ponen los pines SCK, MOSI, SS en output mode
	// también SCK, MOSI se ponen en LOW state y SS en HIGH state.
	// Then put SPI hardware into Master mode and turn SPI on
	SPI.begin();
	SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));  // 2 MHz clock, MSB first, mode 0 

}  // end of setup


void loop()
{
	char c; // 1 byte de datos

	// enable Slave Select
	digitalWrite(SS, LOW);    // SS is pin 10

	// send test string
	for (const char * p = "Hello, world!\n"; c = *p; p++) { /*p++ = p + 1, const char *p : This is a pointer to a constant character. 
	//You cannot change the value pointed by p, but you can change the pointer itself. “const char *” is a (non-const) pointer to a const char. 
	//El compilador crea el literal "Hello World" en una zona solo-lectura de la memoria y *p apunta a esa dirección
	*p apunta a la primera dirección de memoria en la que está la primera letra del string, si imprimimos *p estamos imprimiendo el valor de esa ubicación en concreto (esa letra), si imprimimos p imprimos el conjunto del string
	a partir de esa ubicación, al hacer p++ estamos pasando a la siguiente ubicación de memoria
	La condición c = *p implica que cuando *p ya no sea un char se salga*/
		Serial.println(*p);
		Serial.println(c);
		SPI.transfer(c);
		//Serial.print(c);
	}
	Serial.println("fuera");
	// disable Slave Select
	digitalWrite(SS, HIGH);

	delay(1000);  // 1 seconds delay 
}  // end of loop
