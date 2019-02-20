/*
 * PWMToOutput.cpp
 *
 * Created: 08.07.2018 20:13:59
 * Author : gert
 */ 


#include <General.h>
#include <ISRs.h>

int main(void)
{
	DDRB |= (1 << PORTB0);		//DEBUG: output reaction from ISRs
	SwitchTime::init(0, 180);
	PWMOut::init(ServoType::analog, 50);
	SwitchTime::startSwitchSense();

	sei();
    while (1) 
    {

    }
}

