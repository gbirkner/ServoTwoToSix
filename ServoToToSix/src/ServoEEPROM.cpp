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
	return eepromValues;
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
	eepromValues.SerialNr = (uint16_t)SERIALNR;
	eepromValues.servoType = newInitValues.servoType;
	eepromValues.SNullStellung = newInitValues.SNullStellung;
	eepromValues.AngleRange = newInitValues.AngleRange;
	eepromValues.PWMDelay = newInitValues.PWMDelay;
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
	eepromValues.SerialNr = s;
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
	memcpy(eepromValues.author, a, actLength + 1);
	if(write) writeData();
}

/**
 * \brief 
 *  Company as string[20] is written to eepromvalue and EEPROM if wished
 *
 * \param c
 *  string (pointer to char) to be saved
 *
 * \param write
 *  0 .. write only to eepromvalue
 *  1 .. write to eepromvalue and save to EEPROM
 * 
 * \return void
 */
void ServoEEPROM::setCompany(char* c, bool write /*= false*/)
{
	uint8_t actLength = strlen(c);
	if (actLength > 20) actLength = 20;
	memcpy(eepromValues.company, c, actLength + 1);
	if(write) writeData();
}

/**
 * \brief 
 *  Version as string[5] is written to eepromvalue and EEPROM if wished
 *
 * \param v
 *  string (pointer to char) to be saved
 *
 * \param write
 *  0 .. write only to eepromvalue
 *  1 .. write to eepromvalue and save to EEPROM
 * 
 * \return void
 */
void ServoEEPROM::setVersion(char* v, bool write /*= false*/)
{
	uint8_t actLength = strlen(v);
	if (actLength > 5) actLength = 5;
	memcpy(eepromValues.version, v, actLength + 1);
	if(write) writeData();
}

/**
 * \brief 
 *  ServoTypoe as enum ServoType is written to eepromvalue and EEPROM if wished
 *
 * \param st
 *  ServoType enum value to be saved
 *
 * \param write
 *  0 .. write only to eepromvalue
 *  1 .. write to eepromvalue and save to EEPROM
 * 
 * \return void
 */
void ServoEEPROM::setServoType(ServoType st, bool write /*= false*/)
{
	eepromValues.servoType = st;
	if(write) writeData();
}

/**
 * \brief 
 *  Zeroing Position as int8_t is written to eepromvalue and EEPROM if wished
 *
 * \param ns
 *  Zero Position (-+ abreviation to 90° == 0 to be saved)
 *
 * \param write
 *  0 .. write only to eepromvalue
 *  1 .. write to eepromvalue and save to EEPROM
 * 
 * \return void
 */
void ServoEEPROM::setNullStellung(int8_t ns, bool write /*= false*/)
{
	eepromValues.SNullStellung = ns;
	if(write) writeData();
}

/**
 * \brief 
 *  Angle range as uint8_t is written to eepromvalue and EEPROM if wished
 *
 * \param ar
 *  Angle Range to be saved)
 *
 * \param write
 *  0 .. write only to eepromvalue
 *  1 .. write to eepromvalue and save to EEPROM
 * 
 * \return void
 */
void ServoEEPROM::setAngleRange(uint8_t ar, bool write /*= false*/)
{
	eepromValues.AngleRange = ar;
	if(write) writeData();
}


/**
 * \brief 
 *  PWM delay in ms as uin16_t is written to eepromvalue and EEPROM if wished
 *
 * \param del
 *  PWM delay in ms to be saved
 *
 * \param write
 *  0 .. write only to eepromvalue
 *  1 .. write to eepromvalue and save to EEPROM
 * 
 * \return void
 */
void ServoEEPROM::setPWMDelay( uint16_t del, bool write /*= false*/ )
{
	if(del > 100) {
		eepromValues.PWMDelay = del;
		if(write) writeData();
	}
}

// default constructor
ServoEEPROM::ServoEEPROM()
{
	readData();
} //ServoEEPROM



/**
 * \brief 
 *  reads the EEPROM Data as block (struct) and saves it in eepromvalue
 *  if the SERIALNR is not the same as in board.h then all variables are
 *  replaced with default/constant values and written to EEPROM as block (struct)
 * 
 * \return void
 */
void ServoEEPROM::readData() {
	eeprom_read_block(&eepromValues, (void*)EEPROMADRESS, sizeof(eepromValues));
	//if (eepromValues.SerialNr != (uint16_t)SERIALNR) {
		//eepromValues.SerialNr = (uint16_t)SERIALNR;
		//memset(eepromValues.author, ' ', 20);
		//memset(eepromValues.company, ' ', 20);
		//memset(eepromValues.version, ' ', 5);
		//uint8_t actLen;
		//actLen = strlen(AUTHOR);
		//if (actLen > 20) actLen = 20;
		//memcpy(eepromValues.author, AUTHOR, actLen + 1);
		//actLen = strlen(COMPANY);
		//if (actLen > 20) actLen = 20;
		//memcpy(eepromValues.company, COMPANY, actLen + 1);
		//actLen = strlen(VERSION);
		//if (actLen > 5) actLen = 5;
		//memcpy(eepromValues.version, VERSION, actLen + 1);
		//eepromValues.servoType = ServoType::analog;
		//eepromValues.SNullStellung = 0;
		//eepromValues.AngleRange = 180;
		//eepromValues.PWMDelay = 500;
		//writeData();
	//}
}

/**
 * \brief 
 *  content of eepromvalues is saved to EEPROM as block (struct) 
 *  to constant EEPROMADress
 * 
 * \return void
 */
void ServoEEPROM::writeData() {
	eeprom_update_block(&eepromValues, (void*)EEPROMADRESS, sizeof(eepromValues));
}

// default destructor
ServoEEPROM::~ServoEEPROM()
{
} //~ServoEEPROM
