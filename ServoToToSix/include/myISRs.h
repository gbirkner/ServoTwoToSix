/*
 * myISRs.h
 *
 * Created: 12.04.2017 20:14:28
 *  Author: gert
 */ 


#ifndef MYISRS_H_
#define MYISRS_H_


//#ifdef __cplusplus
	//extern "C" {
//#endif

/**
 * \brief
 *  Switch position Interrupt
 *  every time the switch input signal changes the logic level this ISR is called.
 *  on rising edge:  the switch timer starts the Switch timer 
 *	on falling edge: the switch timer stops and the value is evaluated to calculate the 
 *	correct settings for the tristate gates
 */
ISR(SwitchSenseInterrupt) {
	Servo2To6::SwitchTimer(false);
	uint8_t cValue = TCNT0;
	
	if(SigInSwitchInput & (1 << SigInSwitchPin)) {
		PORTB |= (1 << PORTB0);	//DEBUG
		TCNT0 = 0;
		Servo2To6::SigSwitchMeassure = true;
		Servo2To6::SwitchTimer(true);
	} else {
		PORTB &= ~(1 << PORTB0);	//DEBUG
		if (Servo2To6::SigSwitchMeassure) {			
			Servo2To6::SigSwitchMeassure = false;
			Servo2To6::NewSwitchPosition = Servo2To6::getPosition(cValue);
			if (Servo2To6::NewSwitchPosition != Servo2To6::SwitchPosition) {
				Servo2To6::SwitchSenseISR(false);
				Servo2To6::DTZValue = 0;
				Servo2To6::setNewValues = true;
				Servo2To6::SigInServo1ISR(true);
			} else {
				Servo2To6::setNewValues = false;				
			} // if (Servo2To6::NewSwitchPosition != Servo2To6::SwitchPosition)
		} // if (Servo2To6::SigSwitchMeassure)
	} // if(SigInSwitchInput & (1 << SigInSwitchPin))
}




/**
 * \brief
 *  Signal from servo 1 changes level on PORTC3
 *
 */
ISR(SigInServo1Interrupt) {
	PORTD |= (1 << PORTD7); //DEBUG
	if(SigInSwitchInput & (1 << SigInServo1Pin)) {
		if(Servo2To6::setNewValues) {
			TCNT2 = 0;
			Servo2To6::ThreeMsCounter(true);
			Servo2To6::SigInServo1ISR(false);
		} else if (Servo2To6::PWMSynchronizing) {
			Servo2To6::PWMActive = true;
			Servo2To6::PWMSynchronizing = false;
			Servo2To6::PWMTimer(true);
		} else if (Servo2To6::PWMActive) {
			Servo2To6::DTZValue++;
			if (Servo2To6::DTZValue >= Servo2To6::DelayValue) {
				Servo2To6::PWMTimer(false);
				Servo2To6::PWMActive = false;
			}
			TCNT0 = 0;
			Servo2To6::SwitchTimer(true);	
			Servo2To6::SwitchSenseISR(true);
		} // if(Servo2To6::setNewValues)
	} // if(SigInSwitchInput & (1 << SigInServo1Pin))
}

/**
 * \brief
 *  set all tristates when no signal is high (Servo1, Servo2, PWM)
 *  to avoid disruptions on servo signals
 */
ISR(ThreeMsCountInterrupt) {
	Servo2To6::setNewPositions();
	Servo2To6::ThreeMsCounter(false);
	if (Servo2To6::PWMActive == false) {
		Servo2To6::PWMSynchronizing = true;
	}
	Servo2To6::SigInServo1ISR(true);
}

//#ifdef __cplusplus
	//}
//#endif

#endif /* MYISRS_H_ */