/*
 * E4E_HAL_System.c
 *
 *  Created on: Feb 10, 2022
 *      Author: ntlhui
 */

#include <E4E_HAL_System.h>
#include <e4e_common.h>

E4E_HAL_System_t halSystem, *pHalSystem = &halSystem;

int E4E_HAL_System_init(void)
{
	E4E_HAL_SerialConfig_t serialConfig;
	E4E_HAL_RTCConfig_t rtcConfig;
	E4E_HAL_PWRCTRLConfig_t pwrConfig;


//	if (E4E_ERROR
//			== E4E_HAL_Serial_init(&pHalSystem->commandSerialDesc,
//					E4E_HAL_SerialDevice_Command, &serialConfig))
//	{
//		return E4E_ERROR;
//	}
//	if (E4E_ERROR
//			== E4E_HAL_Serial_init(&pHalSystem->debugSerialDesc,
//					E4E_HAL_SerialDevice_Debug, &serialConfig))
//	{
//		return E4E_ERROR;
//	}
	if (E4E_ERROR == E4E_HAL_RTC_init(&pHalSystem->rtcDesc, NULL))
	{
		return E4E_ERROR;
	}
	if (E4E_ERROR
			== E4E_HAL_PwrCtrl_init(&pHalSystem->onboardComputerDesc,
					E4E_HAL_PWRCTRL_Device_OBC, &pwrConfig))
	{
		return E4E_ERROR;
	}
	return E4E_OK;
}
