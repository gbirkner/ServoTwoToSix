/*
 * ISRs.h
 *
 * Created: 10.07.2018 12:04:42
 *  Author: gert
 */ 


#ifndef ISRS_H_
#define ISRS_H_

#include <General.h>

/**
 * \brief 
 * stops intern PWM timer1 when the delay timer is over
 * ... when actDelayCounter reaches maxDelay
 * and clears all intern pwm tristate control pins
 */
ISR(PWMOutCompAInterrupt) {
	cli();
	PWMOut::actDelayCounter++;
	if(PWMOut::actDelayCounter >= PWMOut::maxDelay) {
		TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));	// stopp PWM Timer 1
		PWMOut::isToStopp = false;
		PWMOut::actDelayCounter = 0;
		PwmOnPort &= ~PwmON1To6Pins;
	}
	sei();
}

/**
 * \brief 
 * meassures the time of the switch signal dutycycle to calc. the transmitted switch position
 * on rising edge: the timer0 is starting at zero
 * on falling edge: the timer0 stops and the nextPosition is calculated
 * if something is wrong, the status isRunning is set to false to enable a new meassurecycle
 * 
 */
ISR(SwitchSenseInterrupt) {
	cli();
	if (((SigInSwitchInput & (1 << SigInSwitchPin)) > 0) && (SwitchTime::isRunning == false)) {			// rising edge an not already running meassure
			TCNT0 = 0;
			TCCR0B |= (1 << CS02);		// start timer with TOV ISR (3ms) (failure: propably edge not recognized)
			SwitchTime::isRunning = true;
	} else if (((SigInSwitchInput & (1 << SigInSwitchPin)) == 0) && (SwitchTime::isRunning == true)) {	// falling edge an meassurement active
		SwitchTime::isRunning = false;
		TCCR0B &= ~(1 << CS02);			// stop timer
		EIMSK &= ~(1 << INT0);			// stop switch sense
		SwitchTime::setPosition(TCNT0);	// preserve positionchange for synchronizing
	} else {
		SwitchTime::isRunning = false;
	}
	sei();
}

/**
 * \brief 
 * this ISR is only called, when the switching time is far more than allowed by servos (>2,4ms) here 3ms
 * therefore the timer0 is halted, switch sensing is turned off and isRunning gets false toi enable new meassurement cycles
 * 
 */
ISR(SwitchTimerOVF) {
	cli();
	TCCR0B &= ~(1 << CS02);			// stop timer
	TIMSK0 &= ~(1 << INT0);			// stop sensing switch ISR
	SwitchTime::isRunning = false;
	sei();
}

/*
* \brief 
* PCINT11 on PC3 (signal of Servo 1) is changing the pin level 
* at first hit it starts the 3ms timer which sets the position on output pins
* at second hit it starts PWM and deactivates itself
*/
ISR(SigInServo1Interrupt) {
	cli();	
	if ((SigInInput & ( 1 << SigInServo1Pin)) > 0) {		// rising edge
		if (SwitchTime::isToPosition == true) {
			TCNT2 = 0;
			TCCR2B |= (1 << CS22) | (1 << CS21);	// start Timer2 with prescaler 256 (3ms == 187) => (F_CPU / N / 1000 (ms)) * 3ms
			SwitchTime::isToPosition = false;
		} else if (SwitchTime::isToSynchonize == true) {			
			PCMSK1 &= ~(1 << PCINT11);		
			PWMOut::PWMStart();
			SwitchTime::isToSynchonize = false;
		}
	}
	sei();
}


/*
* \brief
* is called when Timer2 is activated and 3ms after rising edge from the input signal servo 1
* to avoid interrupting the Servo signals when changing the position outputs
*/
ISR(ThreeMsCountInterrupt) {
	cli();
	
	PWMOut::setPosition(SwitchTime::nextPosition);		// set output to the right position
	TCCR2B &= ~((1 << CS22) | (1 << CS21) | (CS20));	// stop Timer
	SwitchTime::isToSynchonize = true;
	sei();
}

#endif /* ISRS_H_ */