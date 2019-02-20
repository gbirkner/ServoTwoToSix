/* 
* PWMOut.cpp
*
* Created: 08.07.2018 20:18:52
* Author: gert
*/


#include <PWMOut.h>


uint8_t PWMOut::actPosition = 3;
bool PWMOut::isToStopp = false;
bool PWMOut::isToStart = false;
uint8_t PWMOut::actDelayCounter = 0;
uint8_t PWMOut::maxDelay = 50;

/**
 * \brief 
 * initializing the generation of the PWM on timer1
 * \param st
 *		defines which serotype to use
 * \param pwmdelay
 *		defines the number of pwm cycles (ISR calls depends on pwm frequency => servotype) where to stop the intern PWM
 * \return void
 */
void PWMOut::init(ServoType st, uint8_t pwmdelay)
{
	// PORT settings
	PwmOnDir |= PwmON1To6Pins;			//output PWM for servo 12, 34, 56 tristate control pins
	ServoOpenDir |= SV1To6OpenPins;		//output signals for servo 12, 34, 56 tristate control pins
	PWMOutDir |= (1 << PORTB1);
	maxDelay = pwmdelay;
	// PWM Timer 1 settings without starting timer
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);	// Fast PWM setting( f = ICR1, dutycycle = OCR1A
	TCCR1A |= (1 << COM1A1);				// OC1a (PortB1) noninverting mode
	TIMSK1 |= (1 << OCIE1A);				
	switch (st) {							// set servo frequency
		case ServoType::analog:
			ICR1 = (uint16_t)((F_CPU / 8UL / 50UL) - 1);	// 50Hz
			break;
		case ServoType::digital100:
			ICR1 = (uint16_t)((F_CPU / 8UL / 100UL) - 1);	// 100Hz
		break;
		case ServoType::digital200:
			ICR1 = (uint16_t)((F_CPU / 8UL / 200UL) - 1);	// 200Hz
		break;
		case ServoType::digital300:
			ICR1 = (uint16_t)((F_CPU / 8UL / 300UL) - 1);	// 300Hz
		break;
	}

	OCR1A = (uint16_t)((F_CPU / 8 / 667) - 1);			// 1,5ms dutycycle (90°)	
	TCNT1 = 0;
	ServoOpenPort &= ~((1 << SV12OpenPin) | (1 << SV34OpenPin) | (1 << SV56OpenPin));
	PwmOnPort |= (1 << PwmOn12Pin) | (1 << PwmOn34Pin) | (1 << PwmOn56Pin);
	PWMOut::PWMStart();
}

/**
 * \brief 
 * actually activates the signal pwm on the new servopair and
 * activates the intern PWM for the old servopair
 * \param newPosition
 * servopair12 .. 0, servopair34 .. 1, servopair .. 2
 * \return bool
 * true .. servopair had to be changed, else false
 */
bool PWMOut::setPosition(uint8_t newPosition)
{
	if (newPosition != actPosition) {
		// set actPosition to PWM
		switch (actPosition) {
			case Servo12:
				ServoOpenPort &= ~(1 << SV12OpenPin);
				PwmOnPort |= (1 << PwmOn12Pin);
				break;
			case Servo34:
				ServoOpenPort &= ~(1 << SV34OpenPin);
				PwmOnPort |= (1 << PwmOn34Pin);
				break;
			case Servo56:
				ServoOpenPort &= ~(1 << SV56OpenPin);
				PwmOnPort |= (1 << PwmOn56Pin);
				break;
			default:								// on start actPosition is 3 (invalid) therefor all pins are cleared
				ServoOpenPort &= ~(SV1To6OpenPins);
				PwmOnPort &= ~(PwmON1To6Pins);
				break;
		}

		// set newPosition to servo signal
		switch (newPosition) {
			case Servo12:
				PwmOnPort &= ~(	1 << PwmOn12Pin);
				ServoOpenPort |= (1 << SV12OpenPin);				
				break;
			case Servo34:
				PwmOnPort &= ~(	1 << PwmOn34Pin);
				ServoOpenPort |= (1 << SV34OpenPin);
				break;
			case Servo56:
				PwmOnPort &= ~(	1 << PwmOn56Pin);
				ServoOpenPort |= (1 << SV56OpenPin);
				break;
		}
		//PWMStart();							//TODO: darf erst nach dem setPosition (beim 2. SigServo1 rising edge eingeschaltet werden
		actPosition = newPosition;
		isToStart = true;
		return true;
	}
	return false;
}

/**
 * \brief 
 * gets the current activated servopair
 * 
 * \return uint8_t
 * current position
 */
uint8_t PWMOut::getPosition()
{
	return actPosition;
}

/**
 * \brief 
 * clears all pins which control the tristates for intern PWM
 * 
 * \return void
 */
void PWMOut::delOldPositions()
{
	PwmOnPort &= ~PwmON1To6Pins;
}


/**
 * \brief 
 * starts the intern PWM and the Switch input signal sensing
 * 
 * \return void
 */
void PWMOut::PWMStart()
{
	if (TCNT1 > (ICR1 / 2)) TCNT1 = (ICR1 -1); else TCNT1 = 0;
	TCCR1B |= (1 << CS11);	// start PWM with prescaler 8
	actDelayCounter = 0;
	isToStopp = false;
	isToStart = false;
	SwitchTime::startSwitchSense();
}

/**
 * \brief 
 * prepares the intern PWM to stopp on sync
 * 
 * \return void
 */
void PWMOut::PWMStopp()
{
	isToStopp = true;
}

// default constructor
PWMOut::PWMOut() { } //PWMOut

