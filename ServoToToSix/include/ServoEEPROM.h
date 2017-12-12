/* 
* ServoEEPROM.h
*
* Created: 21.03.2016 21:37:31
* Author: gert
*/


#ifndef __SERVOEEPROM_H__
#define __SERVOEEPROM_H__

#include <avr/io.h>
#include <avr/eeprom.h>
#include <General.h>


 struct EEPROM_initValues {
	 uint16_t SerialNr;
	 char author[20];
	 char company[20];
	 char  version[5]; 
	 ServoType servoType;
	 int8_t SNullStellung;
	 uint8_t AngleRange;
	 uint16_t PWMDelay;
	 bool operator==(const EEPROM_initValues& other) const {		
		if(SerialNr != other.SerialNr) return false;
		if(author != other.author) return false;
		if(company != other.company) return false;
		if(version != other.company) return false;
		if(servoType != other.servoType) return false;
		if(SNullStellung != other.SNullStellung) return false;
		if(AngleRange != other.AngleRange) return false;
		return true;
	 }
	 bool operator!=(const EEPROM_initValues& other) const {
		if(SerialNr != other.SerialNr) return true;
		if(author != other.author) return true;
		if(company != other.company) return true;
		if(version != other.company) return true;
		if(servoType != other.servoType) return true;
		if(SNullStellung != other.SNullStellung) return true;
		if(AngleRange != other.AngleRange) return true;
		return false;
	}
};
 

class ServoEEPROM
{
//variables
public:
protected:
private:
	EEPROM_initValues m_initValues;

//functions
public:
	EEPROM_initValues getInitValues(bool readAnew = false);
	void setInitValues(EEPROM_initValues newInitValues);
	void setSerialNr(uint16_t s, bool write = false);
	void setAuthor(char* a, bool write = false);
	void setCompany(char* c, bool write = false);
	void setVersion(char* v, bool write = false);
	void setServoType(ServoType st, bool write = false);
	void setNullStellung(int8_t ns, bool write = false);
	void setAngleRange(uint8_t ar, bool write = false);
	void setPWMDelay(uint16_t del, bool write = false);
	ServoEEPROM();
	~ServoEEPROM();
protected:
private:
	ServoEEPROM( const ServoEEPROM &c );
	ServoEEPROM& operator=( const ServoEEPROM &c );

	void readData();
	void writeData();

}; //ServoEEPROM

extern ServoEEPROM initValues;

#endif //__SERVOEEPROM_H__

