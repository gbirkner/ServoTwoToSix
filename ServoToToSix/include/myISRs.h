/*
 * myISRs.h
 *
 * Created: 12.04.2017 20:14:28
 *  Author: gert
 */ 


#ifndef MYISRS_H_
#define MYISRS_H_


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
		Servo2To6::Status = ServoStatus::SigSwitchMeassure;
		Servo2To6::SwitchTimer(true);
	} else {
		PORTB &= ~(1 << PORTB0);	//DEBUG
		if (Servo2To6::Status == ServoStatus::SigSwitchMeassure) {			
			Servo2To6::NewSwitchPosition = Servo2To6::getPosition(cValue);
			if (Servo2To6::NewSwitchPosition != Servo2To6::SwitchPosition) {
				Servo2To6::SwitchSenseISR(false);
				Servo2To6::DTZValue = 0;
				Servo2To6::Status = ServoStatus::setNewValues;
				Servo2To6::SigInServo1ISR(true);
			} else {
				Servo2To6::Status = ServoStatus::inActive;				
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
	
	if(SigInInput & (1 << SigInServo1Pin)) {
		
		if(Servo2To6::Status == ServoStatus::setNewValues) {
			PORTD ^= (1 << PORTD7); //DEBUG
			TCNT2 = 0;
			Servo2To6::ThreeMsCounter(true);
			Servo2To6::SigInServo1ISR(false);
		} else if (Servo2To6::Status == ServoStatus::PWMSynchronizing) {
			Servo2To6::Status = ServoStatus::PWMActive;
			Servo2To6::PWMTimer(true);
		} else if (Servo2To6::Status == ServoStatus::PWMActive) {
			Servo2To6::DTZValue++;
			if (Servo2To6::DTZValue >= Servo2To6::DelayValue) {
				Servo2To6::PWMTimer(false);
				PwmOnPort &= PwmON1To6Pins;
				Servo2To6::Status = ServoStatus::inActive;
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
	PORTC ^= (1 << PORTC5);
	Servo2To6::setNewPositions();
	Servo2To6::ThreeMsCounter(false);
	if (Servo2To6::Status != ServoStatus::PWMActive) {
		Servo2To6::Status = ServoStatus::PWMSynchronizing;
	}
	Servo2To6::SigInServo1ISR(true);
}

//#ifdef __cplusplus
	//}
//#endif

#endif /* MYISRS_H_ */