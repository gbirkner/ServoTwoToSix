/*
 * ServoTestUnit.cpp
 *
 * Created: 29.12.2016 18:07:52
 * Author : gert
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU		16000000UL			// Controller clock frequency
#define F_Servo	           50			// Servo PWM frequency
#define N_Servo			    8			// Prescaler Servo
#define ServoMinTime	  600			// µs 0° Stellung des Servos (1200)
#define ServoMaxTime	 2400			// µs 180° Stellung des Servos (4800)
#define ServoPeriod 1/F_Servo			// ms Servo Period e.g. 1/50Hz * 1000
#define SwitchDelay	       25			// reaction delay for a switch to be reactivated (45 ... 1s)

#define servo1		OCR1A				// OC1A - PB1
#define servo2		OCR1B				// OC1B - PB2
#define servoSwitch OCR1B				// OC1B - PB2 (0% - 50% - 100%) 

#define SwitchInPort	PORTD
#define SwitchInDir		DDRD
#define SwitchInPins	PIND
#define SwitchInUpPin	PORTD0	
#define SwitchInDownPin	PORTD1
#define SwitchInUpLED	PORTD2
#define SwitchInMidLED	PORTD3
#define SwitchInLowLED	PORTD4

volatile uint16_t SwitchPos[3];			// act. OCR1n values for servo duty cycle
volatile uint8_t  SwitchIdx = 0;		// act. SwitchPos array index 
volatile uint16_t SwitchCount = 0;		// Delaycounter for SwitchServo to switch to next position

int main(void)
{
	// PWM Ports as output
	DDRB |= (1 << PORTB1) | (1 << PORTB2);

	//Pinconfig
	SwitchInDir |= (1 << SwitchInUpLED) | (1 << SwitchInMidLED) | (1 << SwitchInLowLED);	// ouptut for LEDs
	SwitchInPort |= (1 << SwitchInUpPin) | (1 << SwitchInDownPin);							// Pullup Resistors for Switches

	// Einstellungen für Analog Servo (600µs - 1500µs - 2400µs) 50Hz
	SwitchPos[0] = 1500;// - 375;
	SwitchPos[2] = 4500;// - 1125;
	SwitchPos[1] = 3000;// - 750;

	// init PWM Timer 1
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);		// Timer Mode FastPWM

	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);	// enable Timeroutputpins (non-inverting mode)
	ICR1 = 39999; // - 10000;					// set Frequency for analog Servomotors (50Hz) => (16MHz / N=8 / 50 Hz) - 1 = 39999 
	
	// init PWM Timer0 (switch query)
	TIMSK0 |= (1 << TOIE0);					// ~61 times per second

	servoSwitch = SwitchPos[0];				// Switchservo to default (0°)
	SwitchIdx = 0;
	servo1 = SwitchPos[1];							// Servo 1 to default (90°)
	servo2 = SwitchPos[1];
	SwitchCount = 0; 
	TCCR0B |= (1 << CS02) | (1 << CS00);
	TCCR1B |= (1 << CS11);					//set prescaler N = 8 for 16MHz and start Timer 1
	
	sei();
    while (1) {    }
}

ISR(TIMER0_OVF_vect) {
	uint8_t newPosition = (0b00000011) & (SwitchInPins & ((1 << SwitchInUpPin) | (1 << SwitchInDownPin)));
	if((SwitchCount < SwitchDelay)) {
		SwitchCount++;
	} else {
		switch (newPosition) {
			case 1:
				SwitchCount = 0;
				if (SwitchIdx > 0) {
					SwitchIdx--;
					servoSwitch = SwitchPos[SwitchIdx];
				}
				break;
			case 2:
				SwitchCount = 0;
				if (SwitchIdx < 2) {
					SwitchIdx++;
					servoSwitch = SwitchPos[SwitchIdx];
				}
				break;
			default:
				if (SwitchCount < SwitchDelay)
					SwitchCount++;
				break;
		} // switch

		switch (SwitchIdx) {
			case 0:
				SwitchInPort &= ~((1 << SwitchInMidLED) | (1 << SwitchInUpLED));
				SwitchInPort |= (1 << SwitchInLowLED);
				break;
			case 1:
				SwitchInPort &= ~((1 << SwitchInLowLED) | (1 << SwitchInUpLED));
				SwitchInPort |= (1 << SwitchInMidLED);
				break;
			default:
				SwitchInPort &= ~((1 << SwitchInMidLED) | (1 << SwitchInLowLED));
				SwitchInPort |= (1 << SwitchInUpLED);
				break;	
		}
	}
} 