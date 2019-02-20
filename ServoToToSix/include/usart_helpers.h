/*
 * usart_helpers.h
 *
 * Created: 26.11.2017 18:46:08
 *  Author: gert
 */ 


#ifndef USART_HELPERS_H_
#define USART_HELPERS_H_


#include <General.h>

namespace usartHelpers {

	void MainMenu();
	void ChangeServoType();
	void ChangeNullstellung();
	void ChangeAngleRange();
	void ChangePWMDelay();
	void ServoTypeMenu(ServoType st);
}



#endif /* USART_HELPERS_H_ */