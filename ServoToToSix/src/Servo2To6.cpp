/* 
* Servo2To6.cpp
*
* Created: 12.04.2017 17:19:26
* Author: gert
*/


#include <Servo2To6.h>

	uint8_t	Servo2To6::DelayValue = 50;				// times the Sig1 PCINT ISR counts for the delay of the servo
	bool	Servo2To6::SigSwitchMeassure = false;	// is Switch Position meassurement active
	bool	Servo2To6::setNewValues = false;		// are the new Values different and therefor to change
	bool	Servo2To6::PWMSynchronizing = false;	// next Sig PCINT up: a PWM start is required
	bool	Servo2To6::PWMActive = false;			// is PWM active
	uint8_t	Servo2To6::SwitchPosition = 0;			// current SwitchPosition to compare with new meassure
	uint8_t	Servo2To6::NewSwitchPosition =  0;		// next SwitchPosition to assign
	uint8_t	Servo2To6::DTZValue = 0;				// current Counter Value for DTZ (reaching DelayValue)
	ServoType	Servo2To6::_servoType = ServoType::analog;	// ServoType to calculate the ICR value of PWM timer
	uint16_t	Servo2To6::_MinAngle = 0;			// min angle as timer OCR value calculated from degree angle
	uint16_t	Servo2To6::_MidAngle = 0;			// zero angle as timer OCR value calculated from degree angle
	uint16_t	Servo2To6::_MaxAngle = 0;			// max angle as timer OCR Value calculated from degree angle
	uint8_t	Servo2To6::_MinDecision = 0;		// Decision value for the lower position
	uint8_t	Servo2To6::_MaxDecision = 0;		// Decision value for the upper position (everything between is middle position)



 /**
  * \brief 
  * Constructor
  *		does all the necessary initializations to provide a valid object
  * \param servotype
  *		which servo is used (e.g. analog => 50Hz, digital100 => 100Hz, ...)
  *		to set the correct PWM frequency (ICR value)
  * \param zeroing
  *		which position should the generated PWM provide to the inactive servos in degree(-90 to +90) (0� corresponds to middle position)
  * \param anglerange
  *		max range of servo angle (e.g. 120� => +-60� from 0�(90�) => not zeroing, so it can be asymmetrical from zeroing
  * \return 
  *		no return value
  */
 void Servo2To6::init(ServoType servotype, int8_t zeroing, uint8_t anglerange) {
	_servoType = servotype;
	if (zeroing < -90) 
		zeroing = -90;
	else if (zeroing > 90)
		zeroing = 90;
	_MidAngle = 3000 + (20 * zeroing);
	_MinAngle = _MidAngle - (20 * (anglerange/2));
	_MaxAngle = _MidAngle + (20 * (anglerange/2));
	DelayValue = 50;			// at 50Hz = 50 is one second delay
	SigSwitchMeassure = false;
	setNewValues = false;
	PWMSynchronizing = false;
	PWMActive = false;
	_MinDecision = 78; //(uint8_t)((23.4375 - (0.15625 * ((float)anglerange/4.0))) + 0.5);
	_MaxDecision = 137; //(uint8_t)((23.4375 + (0.15625 * ((float)anglerange/4.0))) + 0.5);

	// ---------------- PIN Configurations
	ServoOpenDir |= (1 << SV12OpenPin) | (1 << SV34OpenPin) | (1 << SV56OpenPin);			// SwitchPort and ServoOpenPort to OUTPUT
	ServoOpenPort &= ~((1 << SV12OpenPin) | (1 << SV34OpenPin) | (1 << SV56OpenPin));		// All Servos signal off

	PWMOutDir |= ( 1 << PWMOut1Pin) | (1 << PWMOUT2Pin);									// PWM Pins to OUTPUT
	PWMOutPort &= ~(( 1 << PWMOut1Pin) | (1 << PWMOUT2Pin));								// PWM Pins to LOW

	PwmOnDir |= (1 << PwmOn12Pin) | (1 << PwmOn34Pin) | (PwmOn56Pin);						// PWM-ON Pins to OUTPUT
	PwmOnPort &= ~((1 << PwmOn12Pin) | (1 << PwmOn34Pin) | (PwmOn56Pin));					// PWM-ON Pins to LOW

	EICRA |= (1 << ISC00);			// INT0 -> SigSwitch: any logical change on PD2 generates an Interrupt
	PCICR |= (1 << PCIE1);			// PCINT11 -> SigS1:  any logical change on PC3 generates an Interrupt

	initPWMTimer1();
	SwitchSenseISR(true);
}



uint8_t Servo2To6::getPosition(uint8_t position)
{
	if (position <= _MinDecision) {
		return 0;
	} else if (position >= _MaxDecision) {
		return 2;
	} else {
		return 1;
	}
}

void Servo2To6::setNewPositions()
{
	switch (SwitchPosition) 
	{
		case 0:			
			ServoOpenPort |= (1 << SV12OpenPin);
			break;
		case 1:
			ServoOpenPort |= (1 << SV34OpenPin);
			break;
		case 2:
			ServoOpenPort |= (1 << SV56OpenPin);
			break;
	}
	switch (NewSwitchPosition)
	{
		case 0:
			ServoOpenPort &= ~(1 << SV12OpenPin);
			PwmOnPort |= (1 << PwmOn12Pin);
			break;
		case 1:
		ServoOpenPort &= ~(1 << SV34OpenPin);
			PwmOnPort |= (1 << PwmOn34Pin);
			break;
		case 2:
			ServoOpenPort &= ~(1 << SV56OpenPin);
			PwmOnPort |= (1 << PwmOn56Pin);
			break;
	}
	SwitchPosition = NewSwitchPosition;
}

/**
 * \brief 
 *	Enables/Disables Interrupt on Signal from Servo 1
 * \param enable
 *	true ... enable ISR, false ... disable
 * \return void
 */
void Servo2To6::SigInServo1ISR(bool enable)
{
	if(enable) {
		PCMSK1 |= (1 << PCINT11);
	} else {
		PCMSK1 &= ~(1 << PCINT11);
	}
}

/**
 * \brief 
 *  starts/stops SwitchTimer0, sets prescaler to 256 or 0
 * \param start
 *  true ... start timer, false ... stop timer
 * \return void
 */
void Servo2To6::SwitchTimer(bool start)
{
	if(start) {
		TCCR0B |= (1 << CS02);
	} else {
		TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
	}
}

/**
 * \brief 
 *  Enables/disables Interrupt INT0 on Switch Signal PD2
 * \param enable
 *  true ... INT0 is active, false .. INT0 is inactive
 * \return void
 */
void Servo2To6::SwitchSenseISR(bool enable)
{
	if (enable) {
		EIMSK |= (1 << INT0);
	} else {
		EIMSK &= ~(1 << INT0);
	}
}

/**
 * \brief 
 *  starts/stops PWMTimer1, sets prescaler to 8/0
 * \param start
 * true ... start PWMTimer1; false ... stop PWMTimer1
 * \return void
 */
void Servo2To6::PWMTimer(bool start)
{
	if(start) {
		TCCR1B |= (1 << CS11);
	} else {
		TCCR1B &= ~((1 << CS12) | (1 << CS11) | ( 1 << CS10));
	}
}

/**
 * \brief 
 * starts/stops 3msTimer, sets prescaler to 1024/0
 * \param start
 *	true .. start Timer2; false .. stop timer2
 * \return void
 */
void Servo2To6::ThreeMsCounter(bool start)
{
	if(start) {
		TCCR2B |=  (1 << CS22) | (1 << CS21) | (1 << CS20);
	} else {
		TCCR2B &=  ~((1 << CS22) | (1 << CS21) | (1 << CS20));
	}
}

/**
 * \brief 
 *	PWM Timer
 *		Timer 1 in Fast PWM Mode creates Servo PWM and OC1A PWM output
 * 
 * \return void
 */
void Servo2To6::initPWMTimer1() {	
	PWMOutDir |= (1 << PWMOut1Pin);				// Timer1 OC1A output
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);		// Fast PWM Mode
	TCCR1A |= (1 << COM1A1);
	
	OCR1A = _MidAngle;
	switch (_servoType)
	{
		case ServoType::analog:
			ICR1 = 39999;
			break;
		case ServoType::digital100:
			ICR1 = 19999;
			break;
		case ServoType::digital200:
			ICR1 = 9999;
			break;
		case ServoType::digital300:
			ICR1 = 6666;
			break;
	} // switch
}


