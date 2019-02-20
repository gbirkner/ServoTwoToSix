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
	//DEBUG
	uart.enable();
	uart.cls();
	uart.write((char*)"Current EEPROM Values!\0", true);
	uart.write((char*)"SerienNr: \0"); uart.write(initValues.getInitValues().SerialNr, true);
	uart.write((char*)"Author:   \0"); uart.write((char*)initValues.getInitValues().author, true);
	uart.write((char*)"Company:  \0"); uart.write((char*)initValues.getInitValues().company, true);
	uart.write((char*)"Version:  \0"); uart.write(initValues.getInitValues().version, true);
	uart.LfCr();
	uart.write((char*)"1 Servotype:  \0"); usartHelpers::ServoTypeMenu(initValues.getInitValues().servoType);
	uart.write((char*)"2 Zeroing:    \0"); uart.write((uint16_t)initValues.getInitValues().SNullStellung); uart.write((char*)" degree\0", true);
	uart.write((char*)"3 Anglerange: \0"); uart.write((uint16_t)initValues.getInitValues().AngleRange); uart.write((char*)" degree\0", true);
	uart.write((char*)"4 PWM delay:  \0"); uart.write((uint16_t)initValues.getInitValues().PWMDelay); uart.write((char*)"ms\0", true);
	uart.LfCr();
	uart.disable();


	DDRB |= (1 << PORTB0); //DEBUG: 
	DDRC |= (1 << PORTC5);
	DDRD |= (1 << PORTD7);

    while (1) {

    }
}








