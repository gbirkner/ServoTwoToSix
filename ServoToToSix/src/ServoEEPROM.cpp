/* 
* ServoEEPROM.cpp
*
* Created: 21.03.2016 21:37:31
* Author: gert
*/


#include <ServoEEPROM.h>
#include <string.h>

/**
 * \brief 
 *  gets the content of the struct EEPROM_initValues
 *  
 * \param readAnew
 *  true ... class struct is filled from EEPROM first
 *  false .. data for struct is filled from class (without EEPROM read)
 *
 * \return EEPROM_initValues
 *  struct which contains all data original from EEPROM
 */
EEPROM_initValues ServoEEPROM::getInitValues(bool readAnew) {
	if (readAnew) {
		readData();
	}
	return m_initValues;
}

/**
 * \brief 
 *  sets all struct data in class and saves them to EEPROM
 *
 * \param newInitValues
 *  struct to persist
 * 
 * \return void
 */
void ServoEEPROM::setInitValues(EEPROM_initValues newInitValues)
{
	//TODO: validate parameter
	m_initValues.SerialNr = (uint16_t)SERIALNR;
	m_initValues.servoType = newInitValues.servoType;
	m_initValues.SNullStellung = newInitValues.SNullStellung;
	m_initValues.AngleRange = newInitValues.AngleRange;
	m_initValues.PWMDelay = newInitValues.PWMDelay;
	writeData();
}

/**
 * \brief 
 *  sets the SerialNr in class/EEPROM
 *
 * \param s
 *  char array which contains the SerialNr with \0
 * \param write
 *  true ... the struct is saved to EEPROM
 *  false .. the struct is not saved to EEPROM (only class)
 *
 * \return void
 */
void ServoEEPROM::setSerialNr(uint16_t s, bool write /*= false*/)
{
	m_initValues.SerialNr = s;
	if (write) writeData();
	
}

/**
 * \brief 
 *  sets the Author to class/EEPROM
 *
 * \param a
 *  char array which contains the Author with \0
 *
 * \param write
 *  true ... the struct is saved to EEPROM
 *  false .. the struct is not saved to EEPROM (only class)
 *
 * \return void
 */
void ServoEEPROM::setAuthor(char* a, bool write /*= false*/)
{
	uint8_t actLength = strlen(a);
	if (actLength > 20) actLength = 20;
	memcpy(m_initValues.author, a, actLength + 1);
	if(write) writeData();
}

void ServoEEPROM::setCompany(char* c, bool write /*= false*/)
{
	uint8_t actLength = strlen(c);
	if (actLength > 20) actLength = 20;
	memcpy(m_initValues.company, c, actLength + 1);
	if(write) writeData();
}

void ServoEEPROM::setVersion(char* v, bool write /*= false*/)
{
	uint8_t actLength = strlen(v);
	if (actLength > 5) actLength = 5;
	memcpy(m_initValues.version, v, actLength + 1);
	if(write) writeData();
}

void ServoEEPROM::setServoType(ServoType st, bool write /*= false*/)
{
	m_initValues.servoType = st;
	if(write) writeData();
}

void ServoEEPROM::setNullStellung(int8_t ns, bool write /*= false*/)
{
	m_initValues.SNullStellung = ns;
	if(write) writeData();
}

void ServoEEPROM::setAngleRange(uint8_t ar, bool write /*= false*/)
{
	m_initValues.AngleRange = ar;
	if(write) writeData();
}



void ServoEEPROM::setPWMDelay( uint16_t del, bool write /*= false*/ )
{
	if(del > 100) {
		m_initValues.PWMDelay = del;
		if(write) writeData();
	}
}

// default constructor
ServoEEPROM::ServoEEPROM()
{
	readData();
} //ServoEEPROM

void ServoEEPROM::readData() {
	eeprom_read_block(&m_initValues, (void*)EEPROMADRESS, sizeof(m_initValues));
	if (m_initValues.SerialNr != (uint16_t)SERIALNR) {
		m_initValues.SerialNr = (uint16_t)SERIALNR;

		memset(m_initValues.author, ' ', 20);
		memset(m_initValues.company, ' ', 20);
		memset(m_initValues.version, ' ', 5);
		uint8_t actLen;
		actLen = strlen(AUTHOR);
		if (actLen > 20) actLen = 20;
		memcpy(m_initValues.author, AUTHOR, actLen + 1);
		actLen = strlen(COMPANY);
		if (actLen > 20) actLen = 20;
		memcpy(m_initValues.company, COMPANY, actLen + 1);
		actLen = strlen(VERSION);
		if (actLen > 5) actLen = 5;
		memcpy(m_initValues.version, VERSION, actLen + 1);
		m_initValues.servoType = ServoType::analog;
		m_initValues.SNullStellung = 0;
		m_initValues.AngleRange = 180;
		m_initValues.PWMDelay = 500;
		writeData();
	}
}

void ServoEEPROM::writeData() {
	eeprom_update_block(&m_initValues, (void*)EEPROMADRESS, sizeof(m_initValues));
}

// default destructor
ServoEEPROM::~ServoEEPROM()
{
} //~ServoEEPROM
