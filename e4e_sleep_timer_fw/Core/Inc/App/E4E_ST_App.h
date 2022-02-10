/*
 * app.h
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#ifndef INC_APP_E4E_ST_APP_H_
#define INC_APP_E4E_ST_APP_H_

#include <E4E_ST_Command.h>
#include <stdint.h>
#include <product.h>


/**
 * Application Descriptor
 */
typedef struct E4E_ST_AppDesc_
{
	/**
	 * Primary Command Module
	 */
	E4E_ST_CommandDesc_t commandModule;
} E4E_ST_AppDesc_t;

/**
 * Application Configuration
 */
typedef struct E4E_ST_AppConfig_
{
	int dummy;
} E4E_ST_AppConfig_t;

/**
 * Initializes the application
 * @param pDesc	Application Descriptor
 * @param pConfig	Application Configuration
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_ST_App_init(E4E_ST_AppDesc_t *pDesc, E4E_ST_AppConfig_t *pConfig);

/**
 * Runs the application
 * @param pDesc	Application Descriptor
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_ST_App_run(E4E_ST_AppDesc_t* pDesc);

/**
 * Callback for the Wake command
 * @param commandID	Command ID
 * @param pParams	Command Parameters
 * @param pContext	Command Context
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_ST_App_doWakeCommand(E4E_ST_CommandID_e commandID, void *pParams,
		void *pContext);
/**
 * Callback for the Set Time command
 * @param commandID	Command ID
 * @param pParams	Command parameters
 * @param pContext	Command context
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_ST_App_doSetTimeCommand(E4E_ST_CommandID_e commandID, void *pParams,
		void *pContext);
/**
 * Callback for RTC Alarms
 * @param alarmTime	Alarm Time
 * @param pContext	Callback context
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_ST_App_doAlarm(int64_t alarmTime, void *pContext);
#endif /* INC_APP_E4E_ST_APP_H_ */
