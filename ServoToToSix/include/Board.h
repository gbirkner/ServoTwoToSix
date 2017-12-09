#ifndef _General
#define _General


#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define	true	1
#define	false	0

#define F_CPU	16000000UL

//////////////////////////////////////////////////////////////////////////
//                   EEPROM Settings                                    //
//////////////////////////////////////////////////////////////////////////
#define EEPROMADRESS 0x50
#define AUTHOR   "gert birkner       "
#define COMPANY  "bSd electronics    "
#define VERSION  "1.0 "
#define SERIALNR 12345

//////////////////////////////////////////////////////////////////////////
//						Port - Pin - Definitions						//
//////////////////////////////////////////////////////////////////////////

// Free Pins:  PB0, PB2, PC5, PD7, ADC6, ADC7

#define SigInSwitchPort			PORTD		// Switch-Servo 3steps
#define SigInSwitchDir			DDRD
#define SigInSwitchInput		PIND
#define SigInSwitchPin			PORTD2		// on INT0, falling and raising edge sense

#define SigInPort				PORTC		// Input for signal Servos1 + 2 to synchronize with generated output PWM
#define SigInDir				DDRC
#define SigInInput				PINC
#define SigInServo1Pin			PORTC3		// Servo 1 signal in
#define SigInServo2Pin			PORTC4		// Servo 2 signal in
#define SigInServo1Interrupt	PCINT1_vect	// PinChange interrupt for servo 1 signal

#define ISPPort					PORTB		// ISP
#define MosiPin					PORTB3
#define MisoPin					PORTB4
#define SckPin					PORTB5
#define ResetPort				PORTC
#define ResetPin				PORTC6

#define XTAL1					PORTB6		//Crystal
#define XTAL2					PORTB7

#define UARTPort				PORTD		// Serial Communcation USART0
#define UARTDir					DDRD
#define UARTPin					PIND
#define UARTRxD					PORTD0
#define UARTTxD					PORTD1
#define UARTVcc					PORTD6

#define PWMOutPort				PORTB		// PWM for 90° (zero-position) after ServoPWM
#define PWMOutDir				DDRB
#define PWMOut1Pin				PORTB1		// standard PWM output Pin
#define PWMOUT2Pin				PORTB2		// actually not needed

#define PwmOnPort				PORTC		// to get the PWM Input signals12 to the correct servo-pair
#define PwmOnDir				DDRC
#define PwmOn12Pin				PORTC0
#define PwmOn34Pin				PORTC1
#define PwmOn56Pin				PORTC2
#define PWMRunning				5
#define ServoToChange			4

#define ServoOpenPort			PORTD		// to set the correct 2 tristates on high impedance to be able to feed the 90° PWM signal
#define ServoOpenDir			DDRD
#define SV12OpenPin				PORTD3
#define SV34OpenPin				PORTD4
#define SV56OpenPin				PORTD5

#define SwitchSenseInterrupt	INT0_vect			// INT0 Interrupt falling and rising edge to meassure the switch position
								
#define ThreeMsCountInterrupt	TIMER2_COMPA_vect	// set all tristates when no signal is high (Servo1, Servo2, PWM)


enum class ServoType {
	analog = 50,
	digital100 = 100,
	digital200 = 200,
	digital300 = 300
};


#endif

