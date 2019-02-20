/* 
* Servo2To6.h
*
* Created: 12.04.2017 17:19:27
* Author: gert
* Description: 
*/


#ifndef __SERVO2TO6_H__
#define __SERVO2TO6_H__

#include <General.h>

/**
 * \brief 
 *  class to manage the servo signal signals
 *
*/
class Servo2To6
{
//variables
public:
	static uint8_t		DelayValue;			// times the Sig1 PCINT ISR counts for the delay of the servo
	static ServoStatus	Status;				// Flags for the status the servo is in
	static uint8_t		SwitchPosition;		// current SwitchPosition to compare with new meassure
	static uint8_t		NewSwitchPosition;	// next SwitchPosition to assign
	static uint8_t		DTZValue;			// current Counter Value for DTZ (reaching DelayValue)

private:
	static ServoType	_servoType;			// ServoType to calculate the ICR value of PWM timer
	static uint16_t		_MinAngle;			// min angle as timer OCR value calculated from degree angle
	static uint16_t		_MidAngle;			// zero angle as timer OCR value calculated from degree angle
	static uint16_t		_MaxAngle;			// max angle as timer OCR Value calculated from degree angle
	static uint8_t		_MinDecision;		// Decision value for the lower position
	static uint8_t		_MaxDecision;		// Decision value for the upper position (everything between is middle position)
	
//functions
public:
	// ------- ISR Inlines -----------------
	static void init(ServoType servotype, int8_t zeroing, uint8_t anglerange, uint16_t pwmdelay);
	static uint8_t getPosition(uint8_t position);
	static void setNewPositions();
	static void SigInServo1ISR(bool enable);
	static void SwitchTimer(bool start);
	static void SwitchSenseISR(bool enable);
	static void PWMTimer(bool start);
	static void ThreeMsCounter(bool start);

private:
	static void initPWMTimer1();				// Timer 1 as fast PWM Timer (without prescaler)

};  //Servo2To6

#endif //__SERVO2TO6_H__
