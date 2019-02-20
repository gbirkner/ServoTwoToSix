/* 
* SwitchTime.cpp
*
* Created: 10.07.2018 17:53:36
* Author: gert
*/


#include <SwitchTime.h>



int8_t SwitchTime::ZeroPos = 94;
uint8_t SwitchTime::LowPos = 78;
uint8_t SwitchTime::HighPos = 109;
bool SwitchTime::isRunning = false;
bool SwitchTime::isToPosition = false;
bool SwitchTime::isToSynchonize = false;
uint8_t SwitchTime::nextPosition = Servo12;

/**
 * \brief 
 * initialising the static class variables and set zeroing and anglerange
 * sets: Low- Zero- and Highpos as timervalue
 * sets the 3ms timer for safety issues but does not start the timer
 * sets pinchange isr for the input signal but does not activate the interrupt call
 * sets extern interrupt INT0 to meassure the switch's dutycycle but does not activate the isr call
 * \param zeroing
 *		offset from the math. zeroing 1500µs in degree
 * \param anglerange
 *		overall anglerange between 0 and 180
 * \return void
 */
void SwitchTime::init(int8_t zeroing, uint8_t anglerange)
{
	if (zeroing < -90) {
		zeroing = -90;
	} else if (zeroing > 90) {
		zeroing = 90;
	}
	ZeroPos = (uint8_t)(SwitchZero + (SwitchDegree * (float)zeroing));
	LowPos = ZeroPos - (uint8_t)((SwitchDegree * ((float)anglerange/2))/2);
	HighPos = ZeroPos + (uint8_t)((SwitchDegree * ((float)anglerange/2))/2);

	
	// Timer0 init normal mode and TOV0 ISR
	TIMSK0 |= (1 << TOIE0);		// activate isr on Timer0 overflow (if switch is more then appr. 3ms)
	TCCR2A |= (1 << WGM21);		// Timer2 CTC Mode
	OCR2A = 187;				// COMPA after 3ms
	TIMSK2 |= (1 << OCIE2A);	// call ISR after COMPA hit (3ms)
	PCICR |= (1 << PCIE1);		// PinChange isr config for PCINT11 on PC3
	EICRA |= (1 << ISC00);		// sence rising and falling edge of external interrupt INT0

}


/**
 * \brief 
 * sets the next to activate servo pair when the synchronization happens
 * \param tv
 * timervalue from witch to calc the next selected servopair
 * \return void
 */
void SwitchTime::setPosition(uint8_t tv)
{
	if (tv >= HighPos) {
		SwitchTime::nextPosition = Servo56;
		//PWMOut::setPosition(Servo56);
	} else if (tv <= LowPos) {
		SwitchTime::nextPosition = Servo12;
		//PWMOut::setPosition(Servo12);
	} else {
		SwitchTime::nextPosition = Servo34;
		//PWMOut::setPosition(Servo34);
	}
	isToPosition = true;
	PCMSK1 |= (1 << PCINT11);	// sense PCInt11
}

/**
 * \brief 
 * starting sensing the switch pwm with extern interrupt INT0
 * 
 * \return void
 */
void SwitchTime::startSwitchSense()
{
	isRunning = false;
	EIMSK |= (1 << INT0);
}
