/*
 * alarms.c
 *
 *  Created on: 6 de jun. de 2016
 *      Author: pato
 */

#include "include.h"
#include "lpc_types.h"
#include "math.h"

#include "board.h"
#include "adcs.h"

#include "FreeRTOS.h"

#include "actuators.h"
#include "sensors.h"
#include "alarms.h"


state_t alarmState[ALARMs_NUMBER];
FunctionalState alarmControl[ALARMs_NUMBER];

extern volatile uint8_t sensorNivelAgua;
extern volatile uint8_t sensorTemperatura;
extern volatile uint8_t sensorPh;

extern uint8_t sensorValue[];
extern uint8_t sensorLimit[];

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

void setAlarmState(uint8_t alarmNum){

	alarmState[alarmNum] = ON;
}

void clearAlarmState(uint8_t alarmNum){

	alarmState[alarmNum] = OFF;
}


char* getAlarmControl(uint8_t alarmNum){

	char * AlarmControlState;

	//todo: por defecto habilita el control.  REVISAR LA SEGURIDAD
	if(DISABLE == alarmControl[alarmNum])
		return AlarmControlState = "DISABLE";
	else
		return AlarmControlState = "ENABLE";
}


void vAlarmControl(void *pvParameters){

	volatile portTickType periodo = 1000/portTICK_RATE_MS;

	volatile uint32_t data0, data1, data2;

	//float aux=1;

	uint8_t i;

	/*inicialización del estado y del control de las alarmas*/
	for (i=0; i < ALARMs_NUMBER; i++) {

		alarmState[i]= OFF;
		alarmControl[i] = DISABLE;
	}

	UBaseType_t uxHighWaterMark;

	init_ADCs();


	while(1){

		portTickType ticks = xTaskGetTickCount();

		data0 = ADC_Polling_Read(ADC_CH3);
		data1 = ADC_Polling_Read(ADC_CH3);
		data2 = ADC_Polling_Read(ADC_CH2);

		data0 = (data0*30>>10);
		data1 = (data1*30>>10);
		//aux = floor(data1*((30-16)*2-1)/1024)/2+16;
		data2 = (data2*14>>10);

		sensorValue[0] = data0;
		sensorValue[1] = data1;
		sensorValue[2] = data2;

		for (i=0; i < SENSORs_NUMBER; i++) {

			if ( sensorValue[i] > sensorLimit[2*i] ){
				setAlarmState(2*i);
			}
			else {
				clearAlarmState(2*i);
			}

			if ( sensorValue[i] < sensorLimit[(2*i)+1] ){
				setAlarmState((2*i)+1);
			}
			else {
				clearAlarmState((2*i)+1);
			}

		}

//		DEBUGOUT("data0    : %d\r\n", data0);
//		DEBUGOUT("data1    : %d\r\n", data1);
//		DEBUGOUT("data2    : %d\r\n", data2);
//		DEBUGOUT("data3    : %d\r\n", data3);

		/* Inspect our own high water mark on entering the task. */
		uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );

		vTaskDelayUntil(&ticks,periodo);




	}
	return;
}




const char *alarmsHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {

	uint8_t index;
	char tmpBuff[8];

	for(index = 0; index < ALARMs_NUMBER; index ++){

		sprintf(tmpBuff, "alarma%u", index);

		if( strncmp(pcParam[index], tmpBuff, 7) == 0)
		{
			if( strcmp(pcValue[index], "disable") == 0)
				alarmControl[index] = DISABLE;
			else if( strcmp(pcValue[index], "enable") == 0)
				alarmControl[index] = ENABLE;

		};

	}


	return "/control.shtml";
}
