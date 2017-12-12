/* 
* uart.h
* Simple UART class for uart0 on atmega32
* Overloaded methods write for different datatypes
* no string needed, but every char array must be '\0' terminated
* to ensure correct char array length
* reading can be done by reading only one char or a char array handed over as char* and size
*
* uart must be enabled and can be disabled to start/stop sensing uart port and bind/unbind the rx and tx pins 
*
* with every write method a linefeed can be carried out with the second parameter (boolean), default is no linefeed
* Linefeed and ClearScreen can be executed with extra methods and these are VT100 and above compatible
*
* TODO: insert parameters to contructor (Baud-rate)
* TODO: use different uarts on different controllers
*
* Created: 06.12.2017
* Author: gert birkner bSd 
*/


#ifndef __MYUART_H__
#define __MYUART_H__

#include <avr/io.h>
#include <stdlib.h>
#include <General.h>


#define UART_LF	'\n'
#define UART_CR '\r'


/*
*\brief
* UART standard class to write char, char*, integers, float and double
* and to read char and char*
*/
class myUart {

//variables
public:
	
private:
	const uint16_t UBRR_VAL = (((F_CPU+BAUD_RATE*8)/(BAUD_RATE*16))-1); // 51 for Baudrate 19200 und f_cpu = 16MHz

//functions
public:
	myUart();
	~myUart();

	void enable();
	void disable();
	void write(unsigned char c, bool linefeed = false);
	void write(char *s, bool linefeed = false);
	void write(uint16_t i, bool linefeed = false);
	void write(int16_t i, bool linefeed = false);
	void write(uint32_t i, bool linefeed = false);
	void write(int32_t i, bool linefeed = false);
	void write(float f, uint8_t precision = 2, bool linefeed = false);
	void write(double d, uint8_t precision = 2, bool linefeed = false);
	void cls();
	void LfCr();
	uint8_t read(unsigned char *x, uint8_t size);
	unsigned char read();


private:
	myUart( const myUart &c );
	myUart& operator=( const myUart &c );

};   //myUart

extern myUart uart;



#endif //__MYUART_H__
