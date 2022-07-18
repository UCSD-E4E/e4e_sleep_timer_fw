#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <parser.h>
#include <Debug/conio.h>
#include <e4e_common.h>
#include <main.h>

uint8_t buffer[128]; 
header_t* pHeader = (header_t*) buffer; // points to buffer as a struct (access message like member vars)
size_t idx = 0;

state_t state = SEARCH; // initial state is SEARCH (0)


int parse(char c) // parse chars one at a time from serial
{
	buffer[idx] = c; // put char in buffer
    E4E_Printf("char %x\t idx %zu\t", c, idx);
    switch(state)
    {
        case SEARCH:
            E4E_Printf("SEARCH\n");
            if (buffer[0] == 0xEB)
            {
				if (idx == 0) { // checks that second byte is parsed
                    idx++;
                    return E4E_OK;
                } else if (buffer[1] == 0xE4) { // if start is 0xE4EB move to header
                    state = HEADER;
                    idx++;
                } else if (buffer[1] == 0xEB) { // if second byte might be start
                    buffer[0] = buffer[1];
                } else {
                    idx = 0;
                }
            }
			break;
		case HEADER:
                    E4E_Printf("HEADER\n");
			idx++;
            if (idx == HEADLEN) // if header has been parsed move to args
			{
				state = MSG;
			}

			break;
		case MSG:
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
			state = SEARCH; 
	}
    return E4E_OK;
}

int callfunc() {
    extime_t *ptime; // typecast to extract seconds for setAlarm + setTime
    switch (pHeader->cmd_id) {
        case SET_ALARM:
            ptime = (extime_t *)(buffer);
            int wakeTime = (ptime->sec) * 1000;
            if (E4E_HAL_RTC_setAlarm(&pHalSystem->rtcDesc, wakeTime) != E4E_OK){ 
                E4E_Printf(stderr, "Error with setAlarm()");
                    return E4E_ERROR;
            }
            break;
        case SET_TIME:
            ptime = (extime_t*) buffer;
            int desiredTime = (ptime->sec) * 1000;
            if (E4E_HAL_RTC_setTime(&pHalSystem->rtcDesc, desiredTime) != E4E_OK){
                E4E_Printf(stderr, "Error with setTime()");
                    return E4E_ERROR;
            }
            break;
        case GET_TIME:
            ptime = (extime_t*) buffer;
            int Datetime = (ptime->sec) * 1000;
           if (E4E_HAL_RTC_getTime(&pHalSystem->rtcDesc, Datetime) != E4E_OK){
                E4E_Printf(stderr, "Error with getTime()");
                    return E4E_ERROR;
            } 
            break;
        case CLEAR_ALARM:
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
