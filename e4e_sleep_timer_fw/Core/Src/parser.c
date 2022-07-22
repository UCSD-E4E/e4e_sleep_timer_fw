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



uint8_t buffer[128]; 
E4E_BinaryPacket_Header_t* pHeader = (E4E_BinaryPacket_Header_t*) buffer; // points to buffer as a struct (access message like member vars)
size_t idx = 0;

E4E_BinaryPacket_State_t state = E4E_BinaryPacket_State_SEARCH; // initial state is SEARCH (0)

/**
 * @brief Parses message received and stores in buffer
 * @param char is stored in buffer[]
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */

int parse(char c) // parse chars one at a time from serial
{
	buffer[idx] = c; // put char in buffer
    E4E_Printf("char %x\t idx %zu\t", c, idx);
    switch(state)
    {
        case E4E_BinaryPacket_State_SEARCH:
            E4E_Printf("SEARCH\n");
            if (buffer[0] == 0xEB)
            {
				if (idx == 0) { // checks that second byte is parsed
                    idx++;
                    return E4E_OK;
                } else if (buffer[1] == 0xE4) { // if start is 0xE4EB move to header
                    state = E4E_BinaryPacket_State_HEADER;
                    idx++;
                } else if (buffer[1] == 0xEB) { // if second byte might be start
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

			break;
		case E4E_BinaryPacket_State_MSG:
                    E4E_Printf("MSG\n");
            if (idx < pHeader->length)
                {
                idx++;
                break;
                }
			if (idx == pHeader->length) // if all args parsed
			{
                            E4E_Printf("callfunc\n");
                if(callfunc() != 0) { // call callfunc() and check for error
                    E4E_Printf(stderr, "Error with callfunc()");
                    return E4E_ERROR;
                }
		    }
		default:
			idx = 0;
			state = E4E_BinaryPacket_State_SEARCH; 
	}
    return E4E_OK;
}

int callfunc() {
    E4E_BinaryPacket_extime_t *ptime; // typecast to extract seconds for setAlarm + setTime
    switch (pHeader->cmd_id) {

        case SET_ALARM:
            ptime = (E4E_BinaryPacket_extime_t *)(buffer);
            int wakeTime = (ptime->sec) * 1000;
            if (E4E_HAL_RTC_setAlarm(&pHalSystem->rtcDesc, wakeTime) != E4E_OK){ 
                E4E_Printf(stderr, "Error with setAlarm()");
                    return E4E_ERROR;
            }
            break;
        case SET_TIME: //to set desired time on tracker
            ptime = (E4E_BinaryPacket_extime_t*) buffer;
            int desiredTime = (ptime->sec) * 1000;
            if (E4E_HAL_RTC_setTime(&pHalSystem->rtcDesc, desiredTime) != E4E_OK){
                E4E_Printf(stderr, "Error with setTime()");
                    return E4E_ERROR;
            }
            break;
        case GET_TIME: //give current time to tracker
            ptime = (E4E_BinaryPacket_extime_t*) buffer;
            int Datetime = (ptime->sec) * 1000;
           if (E4E_HAL_RTC_getTime(&pHalSystem->rtcDesc, Datetime) != E4E_OK){
                E4E_Printf(stderr, "Error with getTime()");
                    return E4E_ERROR;
            } 
            break;
        case CLEAR_ALARM: //remove current alarm
            if (E4E_HAL_RTC_clearAlarm(&pHalSystem->rtcDesc) != E4E_OK){
                E4E_Printf(stderr, "Error with clearAlarm()");
                    return E4E_ERROR;
            }
            break;
        case SET_STATE: //called to turn OFF when desired
            if (E4E_HAL_PwrCtrl_setState(&pHalSystem->onboardComputerDesc,E4E_HAL_PWRCTRL_State_OFF) != E4E_OK){
                E4E_Printf(stderr, "Error with setState()");
                    return E4E_ERROR;
            }
            break;
        default:
            E4E_Printf(stderr, "Error: couldn't find cmd");
                return E4E_ERROR;
    }
    return E4E_OK;
}
