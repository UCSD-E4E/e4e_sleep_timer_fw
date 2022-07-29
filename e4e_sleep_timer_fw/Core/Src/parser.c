/*
 * parser.c
 *
 *  Created on: July 7, 2022
 *      Author: frankc
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <parser.h>
#include <HAL/E4E_HAL_RTC.h>
#include <HAL/E4E_HAL_PowerControl.h>
#include <Debug/conio.h>
#include <e4e_common.h>
#include <main.h>
#include "callbacks.h"



uint8_t buffer[128]; 
E4E_BinaryPacket_Header_t* pHeader = (E4E_BinaryPacket_Header_t*) buffer; // points to buffer as a struct (access message like member vars)
size_t idx = 0;

E4E_BinaryPacket_State_t state = E4E_BinaryPacket_State_SEARCH; // initial state is SEARCH (0)


/**
 * Parser callback function names
 */
ParserCallback_t parserCallbackTable[E4E_BinaryPacket_CMD_NELEMS] = {0};
/**
 * @brief Parses message received and stores in buffer
 * @param char is stored in buffer[]
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */

int parse(char c) // parse chars one at a time from serial
{
	ParserCallback_t cb = NULL;
	buffer[idx] = c; // put char in buffer
//    E4E_Printf("char %x\t idx %zu\t", c, idx);
    switch(state)
    {
        case E4E_BinaryPacket_State_SEARCH:
//            E4E_Printf("SEARCH\n");
            if (buffer[0] == 0xE4)
            {
				if (idx == 0) { // checks that second byte is parsed
                    idx++;
                    return E4E_OK;
                } else if (buffer[1] == 0xEB) { // if start is 0xE4EB move to header
                    state = E4E_BinaryPacket_State_HEADER;
                    idx++;
                } else if (buffer[1] == 0xE4) { // if second byte might be start
                    buffer[0] = buffer[1];
                } else {
                    idx = 0;
                }
            }
			break;
		case E4E_BinaryPacket_State_HEADER:
                    E4E_Printf("HEADER\n");
			idx++;
            if (idx == HEADLEN) // if header has been parsed move to args
			{
				state = E4E_BinaryPacket_State_MSG;
			}
            if(pHeader->length == HEADLEN)
            {

            }
            else
            {
            	break;
            }
		case E4E_BinaryPacket_State_MSG:
//                    E4E_Printf("MSG\n");
            if (idx < pHeader->length)
                {
                idx++;
                }
			if (idx == pHeader->length) // if all args parsed
			{
//                E4E_Printf("callfunc\n");
                void* payload = (void*)((uint8_t*)buffer + sizeof(E4E_BinaryPacket_Header_t));
                size_t payloadLength = pHeader->length - sizeof(E4E_BinaryPacket_Header_t);
                cb = parserCallbackTable[pHeader->cmd_id];
                if(NULL == cb)
                {
                	break;
                }
                int retval = cb(payload, payloadLength);
                if(retval != E4E_OK) { // call callfunc() and check for error
                    E4E_Printf("Error with callfunc()");
                    return E4E_ERROR;
                }
                E4E_BinaryPacket_reset();
                return E4E_OK;
		    }
			break;
		default:
			idx = 0;
			state = E4E_BinaryPacket_State_SEARCH; 
	}
    return E4E_OK;
}

void E4E_BinaryPacket_registerCallback(E4E_BinaryPacket_CMD__t cmd_id, ParserCallback_t cb)
{
	parserCallbackTable[cmd_id] = cb;
}

int E4E_BinaryPacket_reset(void)
{
	state = E4E_BinaryPacket_State_SEARCH;
	idx = 0;
	return E4E_OK;
}
