/*
 * ServoTwoToSixDebug.cpp
 *
 * Created: 19.11.2017 13:42:44
 * Author : gert
 */ 

#include <avr/io.h>
#include "myUart.h"

myUart uart;


int main(void)
{

	uart.enable();
	uart.write((char*)"Test");

    /* Replace with your application code */
    while (1) 
    {
		uart.write((unsigned char)(97));
		uart.write((unsigned char)(98));
    }
}
