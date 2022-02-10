/*
 * command.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#include <E4E_ST_Command.h>
#include <e4e_common.h>

int E4E_ST_Command_init(E4E_ST_CommandDesc_t *pDesc,
		E4E_ST_CommandConfig_t *pConfig)
{
	return E4E_ERROR;
}
int E4E_ST_Command_deinit(E4E_ST_CommandDesc_t *pDesc)
{
	return E4E_ERROR;
}
/**
 * @internal Parses the input byte by byte.  This should retain state of what
 * has previously been parsed.  Once a complete and valid command is parsed,
 * this function should also automatically call the appropriate callback
 * function.
 */
int E4E_ST_Command_parse(E4E_ST_CommandDesc_t *pDesc, uint8_t byte)
{
	return E4E_ERROR;
}
int E4E_ST_Command_registerCommandCallback(E4E_ST_CommandDesc_t *pDesc,
		E4E_ST_CommandID_e command, E4E_ST_CommandCallback callback,
		void *pContext)
{
	return E4E_ERROR;
}

int E4E_ST_Command_reset(E4E_ST_CommandDesc_t *pDesc)
{
	return E4E_ERROR;
}
