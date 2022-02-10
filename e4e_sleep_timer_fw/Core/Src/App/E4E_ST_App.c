/*
 * app.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#include <E4E_ST_App.h>
#include <e4e_common.h>
#include <E4E_HAL_System.h>

int E4E_ST_App_init(E4E_ST_AppDesc_t *pDesc, E4E_ST_AppConfig_t *pConfig)
{
	E4E_ST_CommandConfig_t cmdConfig;
	if (NULL == pDesc)
	{
		return E4E_ERROR;
	}
	if (E4E_ERROR == E4E_ST_Command_init(&pDesc->commandModule, &cmdConfig))
	{
		return E4E_ERROR;
	}

	if (E4E_ERROR
			== E4E_HAL_RTC_registerAlarmCallback(&pHalSystem->rtcDesc,
					E4E_ST_App_doAlarm, pDesc))
	{
		return E4E_ERROR;
	}
	if (E4E_ERROR
			== E4E_ST_Command_registerCommandCallback(&pDesc->commandModule,
					E4E_ST_CommandID_SetTime, E4E_ST_App_doSetTimeCommand, pDesc))
	{
		return E4E_ERROR;
	}
	if (E4E_ERROR
			== E4E_ST_Command_registerCommandCallback(&pDesc->commandModule,
					E4E_ST_CommandID_Wake, E4E_ST_App_doWakeCommand, pDesc))
	{
		return E4E_ERROR;
	}

	return E4E_OK;
}

int E4E_ST_App_run(E4E_ST_AppDesc_t *pDesc)
{
	uint8_t nextInputByte;
	while (1)
	{
		if (E4E_ERROR
				== E4E_HAL_Serial_read(&pHalSystem->commandSerialDesc,
						&nextInputByte, 1, E4E_ST_APP_COMMMAND_TIMEOUT))
		{
			// Serial Read timed out - no response
			E4E_ST_Command_reset(&pDesc->commandModule);
			E4E_HAL_Serial_flush(&pHalSystem->commandSerialDesc);
			continue;
		}
		if (E4E_ERROR
				== E4E_ST_Command_parse(&pDesc->commandModule, nextInputByte))
		{
			// Bad command, do something...
		}
	}
}

int E4E_ST_App_doWakeCommand(E4E_ST_CommandID_e commandID, void *pParams,
		void *pContext)
{
	return E4E_ERROR;
}

int E4E_ST_App_doSetTimeCommand(E4E_ST_CommandID_e commandID, void *pParams,
		void *pContext)
{
	return E4E_ERROR;
}

int E4E_ST_App_doAlarm(int64_t alarmTime, void *pContext)
{
	return E4E_ERROR;
}
