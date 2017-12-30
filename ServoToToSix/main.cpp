/*
 * ServoToToSix.cpp
 *
 * Created: 22.03.2016 17:00:49
 * Author : gert
 */ 

#include <General.h>
#include <myISRs.h>
#include <usart_helpers.h>

ServoEEPROM initValues;
myUart uart;

int main(void) {

	//DEBUG: EEPROM new
	//initValues.setAuthor((char*)AUTHOR);
	//initValues.setCompany((char*)COMPANY);
	//initValues.setVersion((char*)VERSION);
	//initValues.setSerialNr((uint16_t)SERIALNR);
	//initValues.setServoType(ServoType::analog);
	//initValues.setNullStellung((int8_t)0);
	//initValues.setAngleRange((uint8_t)180);
	//initValues.setPWMDelay((uint16_t)500, true);

	


	initValues.getInitValues(true);

	bool isActiveUart = ((UARTPin & (1 << UARTVcc)) > 0);
	if(isActiveUart) {
		uart.enable();		
		usartHelpers::MainMenu();
		uart.cls();
		uart.write((char*)"Setup per UART is inactive and Port multiplier is running .....\0");		
		uart.disable();		
	}

	sei();

	Servo2To6::init(initValues.getInitValues().servoType, initValues.getInitValues().SNullStellung, initValues.getInitValues().AngleRange, initValues.getInitValues().PWMDelay);

	DDRB |= (1 << PORTB0); //DEBUG: 
	DDRC |= (1 << PORTC5);
	DDRD |= (1 << PORTD7);

    while (1) {

    }
}








