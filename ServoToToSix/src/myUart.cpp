/* 
* uart.cpp
*
* Created: 06.07.2017 14:01:59
* Author: gert
*/


#include <myUart.h>


void myUart::write(unsigned char c, bool linefeed /*= false*/) {
	while(!(UCSR0A & (1 << UDRE0))) {	}
	UDR0 = c;
	if (linefeed) {
		LfCr();
	}

}

void myUart::write(char *s, bool linefeed /*= false*/) {
	while (*s) {
		write((unsigned char)*s, false);
		s++;
	}
	if (linefeed) LfCr();
}

void myUart::write(int16_t i, bool linefeed /*= false*/) {
	char s[7];
	itoa(i, s, 10);
	write(s, linefeed);
}

void myUart::write(uint16_t i, bool linefeed /*= false*/) {
	char s[6];
	itoa(i, s, 10);
	write(s, linefeed);	
}

void myUart::write(uint32_t i, bool linefeed /*= false*/) {
	char s[8];
	itoa(i, s, 10);
	write(s, linefeed);
}

void myUart::write(int32_t i, bool linefeed /*= false*/) {
	char s[9];
	write(itoa(i, s, 10), linefeed);	
}

void myUart::write(float f, uint8_t precision, bool linefeed /*= false*/) {
	char s[10];
	write(dtostrf(f, 7, precision, s), linefeed);
}

void myUart::write(double d, uint8_t precision /*= 2*/, bool linefeed /*= false*/) {
	char s[12];
	write(dtostrf(d, 9, precision, s), linefeed);
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

uint8_t myUart::read(unsigned char *x, uint8_t size) {
	uint8_t i = 0;
	if (size == 0) return 0;				// return 0 if no space
	unsigned char c = 0;
	while (i < (size - 1)) {				// check space is available (including additional null char at end)
		while ( !(UCSR0A & (1<<RXC0)) );	// wait for another char - WARNING this will wait forever if nothing is received
		c = UDR0;		
		if (c == '\n' || c == '\r') break;  // break on NULL OR ENTER character
		x[i] = c;							// write into the supplied buffer
		i++;
	}
	if (i >= size) i = size - 1;// ensure the last character in the array is '\0'
	x[i] = '\0';	
	return i;				// return number of characters written (incl. '\0')
}

unsigned char myUart::read() {
	while( !(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void myUart::enable() {
	  UCSR0B |= (1<<RXEN0) | (1<<TXEN0);  // UART RX, TX 
}

void myUart::disable() {
	UCSR0B &= ~((1<<RXEN0) | (1<<TXEN0));  // UART RX, TX 
}

void myUart::cls() {
	write((unsigned char)(27));
	write((unsigned char)('['));
	write((unsigned char)('2'));
	write((unsigned char)('J'));
	write((unsigned char)(27));
	write((unsigned char)('['));
	write((unsigned char)('H'));
}

void myUart::LfCr() {
	while(!(UCSR0A & (1 << UDRE0))) {	}
	UDR0 = ((unsigned char)UART_LF);
	while(!(UCSR0A & (1 << UDRE0))) {	}
	UDR0 = ((unsigned char)UART_CR);
}