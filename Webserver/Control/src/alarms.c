/*
 * alarms.c
 *
 *  Created on: 6 de jun. de 2016
 *      Author: pato
 */

#include <sensors.h>
#include "include.h"
#include "lpc_types.h"
#include "alarms.h"
#include "actuators.h"

state_t alarmState[4] = {OFF,OFF,OFF,OFF};
FunctionalState alarmControl[4] = {DISABLE, DISABLE, DISABLE, DISABLE};

extern volatile uint8_t debugInt1;
extern volatile uint8_t debugInt2;
extern volatile uint8_t debugInt3;


char* getAlarmState(uint8_t alarmNum){

	char *ptrAlarmState;

	if(OFF == alarmState[alarmNum])
		return ptrAlarmState = "NORMAL";
	else
		return ptrAlarmState = "ALARMA";

}

void toggleAlarmState(uint8_t alarmNum){

	if( OFF == alarmState[alarmNum])
		alarmState[alarmNum] = ON;

	else if(ON == alarmState[alarmNum])
		alarmState[alarmNum] = OFF;

}

void setAlarm(uint8_t alarmNum){

	alarmState[alarmNum] = ON;
}

void clearAlarm(uint8_t alarmNum){

	alarmState[alarmNum] = OFF;
}



void vAlarmHandler(void *pvParameters){

	while(1){

		if (debugInt1 >= ALARM1_THRESHOLD){
			setAlarm(alarmNum_1);
		};
	}
	return;
}
