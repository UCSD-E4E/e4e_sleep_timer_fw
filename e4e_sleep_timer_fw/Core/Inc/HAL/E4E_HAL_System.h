/*
 * E4E_HAL_System.h
 *
 *  Created on: Feb 10, 2022
 *      Author: ntlhui
 */

#ifndef INC_HAL_E4E_HAL_SYSTEM_H_
#define INC_HAL_E4E_HAL_SYSTEM_H_

#include <E4E_HAL_RTC.h>
#include <E4E_HAL_Serial.h>
#include <E4E_HAL_PowerControl.h>

typedef struct E4E_HAL_System_
{
	E4E_HAL_SerialDesc_t commandSerialDesc;
	E4E_HAL_SerialDesc_t debugSerialDesc;
	E4E_HAL_RTCDesc_t rtcDesc;
	E4E_HAL_PWRCTRLDesc_t onboardComputerDesc;
}E4E_HAL_System_t;

extern E4E_HAL_System_t* pHalSystem;

/**
 * Initializes the Hardware Abstraction Layer
 * @return	E4E_OK if successful, otherwise E4E_ERROR;
 */
int E4E_HAL_System_init(void);

#endif /* INC_HAL_E4E_HAL_SYSTEM_H_ */
