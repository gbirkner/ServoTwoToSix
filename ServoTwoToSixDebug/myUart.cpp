/* 
* uart.cpp
*
* Created: 06.07.2017 14:01:59
* Author: gert
*/


#include "myUart.h"
#include "avr/interrupt.h"


void myUart::write(unsigned char c) {
	while(!(UCSR0A & (1 << UDRE0))) {	}
	UDR0 = c;
}

void myUart::write(char *s) {
	while (*s) {
		write((unsigned char)*s);
		s++;
	}
}

void myUart::write(int16_t i) {
	char s[7];
	itoa(i, s, 10);
	write(s);
}

void myUart::write(uint16_t i)
{
	char s[6];
	itoa(i, s, 10);
	write(s);	
}

void myUart::write(uint32_t i)
{
	char s[8];
	itoa(i, s, 10);
	write(s);
}

void myUart::write(int32_t i)
{
	char s[9];
	write(itoa(i, s, 10));	
}

void myUart::write(float f, uint8_t precision)
{
	char s[10];
	write(dtostrf(f, 7, precision, s));
}

void myUart::write(double d, uint8_t precision /*= 2*/)
{
	char s[12];
	write(dtostrf(d, 9, precision, s));
}


// default constructor
myUart::myUart() {
  UBRR0H = (uint8_t)(UBRR_VAL >> 8);
  UBRR0L = (uint8_t)UBRR_VAL;
  UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00); // Asynchron 8N1
} //uart



// default destructor
myUart::~myUart() {
} //~uart


uint8_t myUart::read(unsigned char *x, uint8_t size)
{
	uint8_t i = 0;

	if (size == 0) return 0;            // return 0 if no space

	while (i < size - 1) {              // check space is available (including additional null char at end)
		unsigned char c;
		while ( !(UCSR0A & (1<<RXC0)) );  // wait for another char - WARNING this will wait forever if nothing is received
		c = UDR0;
		if (c == '\0' || c == '\n') break;           // break on NULL character
		x[i] = c;                       // write into the supplied buffer
		i++;
	}
	x[i] = 0;                           // ensure string is null terminated

	return i + 1;                       // return number of characters written
}


unsigned char myUart::read() {
	while( !(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void myUart::enable()
{
	  UCSR0B |= (1<<RXEN0) | (1<<TXEN0);  // UART RX, TX 
}


void myUart::disable()
{
	UCSR0B &= ~((1<<RXEN0) | (1<<TXEN0));  // UART RX, TX 
}

