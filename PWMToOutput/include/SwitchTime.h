/* 
* SwitchTime.h
*
* Created: 10.07.2018 17:53:36
* Author: gert
*/


#ifndef __SWITCHTIME_H__
#define __SWITCHTIME_H__

#include <General.h>


class SwitchTime
{
//variables
public:
	static bool isRunning;				// switch sensing is active
	static bool isToPosition;			// position as to be set next rising edge in Sign1 IN
	static bool isToSynchonize;			
	static uint8_t nextPosition;

private:
	static int8_t ZeroPos;
	static uint8_t LowPos;
	static uint8_t HighPos;
	

//functions
public:
	static void init(int8_t zeroing, uint8_t anglerange);
	static void setPosition(uint8_t tv);
	static void startSwitchSense();

private:
	SwitchTime() {};


}; //SwitchTime

#endif //__SWITCHTIME_H__
