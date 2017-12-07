/*
 * usart_helpers.cpp
 *
 * Created: 26.11.2017 18:46:55
 *  Author: gert
 */ 

 #include <usart_helpers.h>
 #include <Servo2To6.h>
 #include <ServoEEPROM.h>

 namespace usartHelpers {

	 void ServoTypeMenu(ServoType st)
	 {
		 switch(st) {
			 case ServoType::analog:
			 uart.write((char*)"analog  -  50Hz\0", true);
			 break;
			 case ServoType::digital100:
			 uart.write((char*)"digital - 100Hz\0", true);
			 break;
			 case ServoType::digital200:
			 uart.write((char*)"digital - 200Hz\0", true);
			 break;
			 case ServoType::digital300:
			 uart.write((char*)"digital - 300Hz\0", true);
			 break;
		 }
	 }

	 void MainMenu() {

		EEPROM_initValues actv = initValues.getInitValues(true);
		bool changesMade = false;
		unsigned char x = '0';
		while (x != 'q')  {
			uart.cls();
			uart.write((char*)"ServoPortmultiplier Configuration Menu\0", true);
			uart.LfCr();
			uart.write((char*)"SerienNr: \0"); uart.write(initValues.getInitValues().SerialNr, true); 
			uart.write((char*)"Author:   \0"); uart.write((char*)initValues.getInitValues().author, true); 
			uart.write((char*)"Company:  \0"); uart.write((char*)initValues.getInitValues().company, true); 
			uart.write((char*)"Version:  \0"); uart.write(initValues.getInitValues().version, true);
			uart.LfCr();
			uart.write((char*)"1 Servotype:  \0"); ServoTypeMenu(initValues.getInitValues().servoType);
			uart.write((char*)"2 Zeroing:    \0"); uart.write((uint16_t)initValues.getInitValues().SNullStellung); uart.write((char*)" degree\0", true);
			uart.write((char*)"3 Anglerange: \0"); uart.write((uint16_t)initValues.getInitValues().AngleRange); uart.write((char*)" degree\0", true);
			uart.write((char*)"4 Refresh menu-values\0", true);
			uart.LfCr();
			//if(actv != initValues.getInitValues(false)) changesMade = true; else changesMade = false;

			if(actv.servoType != initValues.getInitValues(false).servoType) changesMade = true;
			if(actv.SNullStellung != initValues.getInitValues(false).SNullStellung) changesMade = true;
			if(actv.AngleRange != initValues.getInitValues(false).AngleRange) changesMade = true;

			if (changesMade) uart.write((char*)"s Save changes to EEPROM\0", true);			
			uart.write((char*)"q Exit", true);
			uart.write((char*)"Select-->\0", false);
			x = uart.read();
			switch (x) {
				 case 'q':
					if (changesMade) {
						while (x != 'q') {
							uart.write((char*)"There are changes made in the setup! Should they be saved to the EEPROM or discarded?\0", true);
							uart.write((char*)"(s)ave or (d)iscard?:\0");
							x = uart.read();
							switch(x) {
								case 's': 
									initValues.setInitValues(initValues.getInitValues(false));
									actv = initValues.getInitValues(false);
									x = 'q';
									break;
								case 'd':
									initValues.getInitValues(true);
									x = 'q';
									break;
								default:
									uart.LfCr();
									uart.write((char*)"Please hit s for save or d for discard changes!\0", true);
									break;
							} // switch
						} // while
					} // if (changesMade)
					break;
				 case '1':
					ChangeServoType();
					break;
				 case '2':
					ChangeNullstellung();
					break;
				 case '3':
					ChangeAngleRange();
					break;
				 case '4':
					break;
				case 's':
					if (changesMade) {
						initValues.setInitValues(initValues.getInitValues(false));
						actv = initValues.getInitValues(false);
					}
					break;
			 } // switch
		 } // while (x != 'q')
	 }

	 void ChangeServoType() {

		unsigned char x = '0';
		
		while (x != 'q') {		
			uart.cls();
			uart.write((char*)"Servo Multiplier --> Change Servotype\0", true);
			uart.LfCr();
			uart.write((char*)"Current Servotype: \0"); ServoTypeMenu(initValues.getInitValues().servoType);
			uart.write((char*)"Select new Servotype:\0", true);
			uart.write((char *)"  1 \0"); ServoTypeMenu(ServoType::analog);
			uart.write((char *)"  2 \0"); ServoTypeMenu(ServoType::digital100);
			uart.write((char *)"  3 \0"); ServoTypeMenu(ServoType::digital200);
			uart.write((char *)"  4 \0"); ServoTypeMenu(ServoType::digital300);
			uart.LfCr();
			uart.write((char *)"  q Exit\0", true);
			uart.write((char *)"Select-->\0");

			x = uart.read();
			switch (x) {
				case '1':
					initValues.setServoType(ServoType::analog, false);
					break;
				case '2':
					initValues.setServoType(ServoType::digital100, false);
					break;
				case '3':
					initValues.setServoType(ServoType::digital200, false);
					break;
				case '4':
					initValues.setServoType(ServoType::digital300, false);
					break;				
			} // switch
			uart.LfCr();
			uart.LfCr();
		} // while
	}


	void ChangeNullstellung() {
		char dev[4];
		bool exit;
		int8_t maxMinus = -90 + (int8_t)(initValues.getInitValues(false).AngleRange/2);
		int8_t maxPlus = 90 - (int8_t)(initValues.getInitValues(false).AngleRange/2);
		do {
		uart.cls();
			exit = false;
			uart.write((char*)"Servo Multiplier --> Change zeroing\0", true);
			uart.LfCr();
			uart.write((char*)"Zeroing is the deviation in degree from the middle position (+,-)\0", true);
			uart.write((char*)"For the angle range can not exceed 180 degree the zeroing can only be changed\0", true);
			uart.write((char*)"when angle range is less than 180!", true);
			uart.LfCr();
			uart.write((char*)"Current deviation: \0"); uart.write(initValues.getInitValues().SNullStellung);
			uart.write((char*)" degree\0", true);
			uart.write((char*)"Enter integer value between \0"); uart.write((int16_t)maxMinus);  uart.write((char*)" and \0"); uart.write((int16_t)maxPlus); uart.write((char*)" or press 'q' for quit without changes: \0", true);
			uart.write((char*)"New deviation: \0");			
			uart.read((unsigned char*)dev, 4);
			uint8_t lauf = 0;
			if (dev[lauf] == 'q') {
				exit = true;	
			} else {
				int8_t newVal = (int8_t)atoi(dev);
				if (newVal < maxMinus || newVal > maxPlus) {
					uart.write((char*)"Value must be between \0"); uart.write((int16_t)maxMinus);  uart.write((char*)" and \0"); uart.write((int16_t)maxPlus, true);
					} else {
					initValues.setNullStellung(newVal);
				}
			} // if (dev[lauf] == 'q')
			uart.LfCr();
		} while (!exit);
	}


	void ChangeAngleRange() {
		char dev[4];
		bool exit;
		uint8_t maxRange = 0;
		if (initValues.getInitValues(false).SNullStellung < 0) {
			maxRange = (uint8_t)(180 + initValues.getInitValues(false).SNullStellung);
		} else	{
			maxRange = (uint8_t)(180 - initValues.getInitValues(false).SNullStellung);
		}
		do {
			uart.cls();
			exit = false;
			uart.write((char*)"Servo Multiplier --> Change Anglerange\0", true);
			uart.LfCr();
			uart.write((char*)"Anglerange in degree up to 180 degree\0", true);
			uart.write((char*)"If zeroing is not 0 the range must be below 180! \0", true);
			uart.write((char*)"Therefore the max. range is \0"); uart.write((uint16_t) maxRange); uart.write((char*)" degree\0", true);
			uart.write((char*)"The minimum Range is 2 and the value is an integer value! \0", true);
			uart.LfCr();
			uart.write((char*)"Current range: \0"); uart.write((uint16_t)initValues.getInitValues().AngleRange); uart.write((char*)" degree\0", true);
			uart.write((char*)"Enter new Range or press 'q' for quit without changes: \0", true);
			uart.write((char*)"New range: \0");
			uart.read((unsigned char*)dev, 4);
			uint8_t lauf = 0;
			if (dev[lauf] == 'q') {
				exit = true;
				} else {
				uint8_t newVal = (uint8_t)atoi(dev);
				if (newVal < 2 || newVal > maxRange) {
					uart.write((char*)"Value must be an integer between 2 and \0", true); uart.write((uint16_t)maxRange, true); 
				} else {
					initValues.setAngleRange(newVal);
				}
			} // if (dev[lauf] == 'q')
			uart.LfCr();
		} while (!exit);	
	}

 }