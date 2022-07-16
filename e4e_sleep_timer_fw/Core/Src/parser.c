#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include "parser.h"

uint8_t buffer[128]; 
header_t* pHeader = (header_t*) buffer; // points to buffer as a struct (access message like member vars)
size_t idx = 0;

state_t state = SEARCH; // initial state is SEARCH (0)


int parse(char c) // parse chars one at a time from serial
{
	buffer[idx] = c; // put char in buffer
    printf("char %x\t idx %zu\t", c, idx);
    switch(state)
    {
        case SEARCH:
            printf("SEARCH\n");
            if (buffer[0] == 0xEB)
            {
				if (idx == 0) { // checks that second byte is parsed
                    idx++;
                    return 0;
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
                    printf("HEADER\n");
			idx++;
            if (idx == HEADLEN) // if header has been parsed move to args
			{
				state = MSG;
			}

			break;
		case MSG:
                    printf("MSG\n");
            if (idx < pHeader->length)
                {
                idx++;
                break;
                }
			if (idx == pHeader->length) // if all args parsed
			{
                            printf("callfunc\n");
                if(callfunc() != 0) { // call callfunc() and check for error
                    fprintf(stderr, "Error with callfunc()");
                    return 1;
                }
		    }
		default:
			idx = 0;
			state = SEARCH; 
	}
    return 0;
}

int callfunc() {
    extime_t *ptime; // typecast to extract seconds for setAlarm + setTime
    switch (pHeader->cmd_id) {
        case SET_ALARM:
            ptime = (extime_t *)(buffer);
            if (setAlarm(ptime->sec) != 0){ 
                fprintf(stderr, "Error with setAlarm()");
                    return 1;
            }
            break;
        case SET_TIME:
            ptime = (extime_t*) buffer;
            if (setTime(ptime->sec) != 0){
                fprintf(stderr, "Error with setTime()");
                    return 1;
            }
            break;
        case GET_TIME:
           if (getTime() != 0){
                fprintf(stderr, "Error with getTime()");
                    return 1;
            } 
            break;
        default:
            fprintf(stderr, "Error: couldn't find cmd");
    }
    return 0;
}
