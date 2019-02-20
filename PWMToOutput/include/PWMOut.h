/* 
* PWMOut.h
*
* Created: 08.07.2018 20:18:52
* Author: gert
*/


#ifndef __PWMOUT_H__
#define __PWMOUT_H__

#include <General.h>


class PWMOut
{
//variables
public:
	static bool isToStopp;
	static bool isToStart;
	static uint8_t actDelayCounter;		// counter to compare the number of PWM timer isr entries
	static uint8_t maxDelay;			// amount of PWM timer1 cycles to delay the PWM-stop for the original servopair after changing to the new servopair

private:
	static uint8_t actPosition;			// which servopair gets the input signal

//functions
public:
	static void init(ServoType st, uint8_t pwmdelay);
	static bool setPosition(uint8_t newPosition);
	static uint8_t getPosition();
	static void delOldPositions();
	static void PWMStart();
	static void PWMStopp();

private:
	PWMOut();
	//PWMOut( const PWMOut &c );
	//PWMOut& operator=( const PWMOut &c );

}; //PWMOut

#endif //__PWMOUT_H__
