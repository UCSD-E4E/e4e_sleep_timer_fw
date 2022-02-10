/*
 * command.h
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#ifndef INC_APP_E4E_ST_COMMAND_H_
#define INC_APP_E4E_ST_COMMAND_H_

#include <stdint.h>

/**
 * Command Module Descriptor
 */
typedef struct E4E_ST_CommandDesc_
{
	int dummy;
} E4E_ST_CommandDesc_t;

/**
 * Command Module Configuration
 */
typedef struct E4E_ST_CommandConfig_
{
	int dummy;
} E4E_ST_CommandConfig_t;

/**
 * Command IDs
 */
typedef enum E4E_ST_CommandID_
{
	E4E_ST_CommandID_Wake, /**< Wake */
	E4E_ST_CommandID_SetTime,/**< Set Time */
	E4E_ST_CommandID__NELEMS /**< E4E_ST_CommandID__NELEMS */
} E4E_ST_CommandID_e;

/**
 * Wake Command Parameters
 */
typedef struct E4E_ST_CommandWakeParams_
{
	/**
	 * Wake time in ms since Unix Epoch
	 */
	int64_t wakeTime;
} E4E_ST_CommandWakeParams_t;

/**
 * Set Time Command Parameters
 */
typedef struct E4E_ST_CommandSetTimeParams_
{
	/**
	 * Time to set in ms since Unix Epoch
	 */
	int64_t timeToSet;
} E4E_ST_CommandSetTimeParams_t;


/**
 * Command Callback Function
 * @param id	Command ID
 * @param params	Pointer to command parameters
 * @param pContext	Callback context
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
typedef int (*E4E_ST_CommandCallback)(E4E_ST_CommandID_e id, void *params,
		void *pContext);

/**
 * @brief Initializes the Command Module
 * @param pDesc	Command Module Descriptor to initialize
 * @param pConfig	Configuration
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_ST_Command_init(E4E_ST_CommandDesc_t *pDesc,
		E4E_ST_CommandConfig_t *pConfig);
/**
 * @brief Deinitializes the Command Module
 * @param pDesc	Command Module Descriptor to deinitialize
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_ST_Command_deinit(E4E_ST_CommandDesc_t *pDesc);
/**
 * @brief Parses another byte
 * @param pDesc	Command Module Descriptor
 * @param byte	Next byte of input
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_ST_Command_parse(E4E_ST_CommandDesc_t *pDesc, uint8_t byte);

/**
 * @brief Resets the parser state
 * @param pDesc	Command Module Descriptor
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_ST_Command_reset(E4E_ST_CommandDesc_t* pDesc);

/**
 * @brief Register a command callback function
 * @param pDesc	Command Module Descriptor
 * @param command	Command ID
 * @param callback	Callback function
 * @param pContext	Callback function context object
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_ST_Command_registerCommandCallback(E4E_ST_CommandDesc_t *pDesc,
		E4E_ST_CommandID_e command, E4E_ST_CommandCallback callback,
		void *pContext);
#endif /* INC_APP_E4E_ST_COMMAND_H_ */
