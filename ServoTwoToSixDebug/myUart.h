/* 
* uart.h
*
* Created: 06.07.2017 14:02:00
* Author: gert
*/


#ifndef __MYUART_H__
#define __MYUART_H__

#include <avr/io.h>
#include <stdlib.h>

class myUart {

//variables
public:
	
private:
	const static uint16_t UBRR_VAL = 51; // ((F_CPU+BAUD*8)/(BAUD*16)-1)

//functions
public:
	myUart();
	~myUart();

	void enable();
	void disable();
	void write(unsigned char c);
	void write(char *s);
	void write(uint16_t i);
	void write(int16_t i);
	void write(uint32_t i);
	void write(int32_t i);
	void write(float f, uint8_t precision = 2);
	void write(double d, uint8_t precision = 2);
	uint8_t read(unsigned char *x, uint8_t size);
	unsigned char read();


private:
	myUart( const myUart &c );
	myUart& operator=( const myUart &c );

};   //myUart





#endif //__MYUART_H__
