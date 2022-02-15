/*
 * conio.h
 *
 *  Created on: Feb 14, 2022
 *      Author: ntlhui
 */

#ifndef INC_DEBUG_CONIO_H_
#define INC_DEBUG_CONIO_H_

#include <stdarg.h>
#include <usart.h>

#define CONIO_PRINTF_BUFFER_LEN	1024

#define CONIO_HANDLE	huart2

#define CONIO_MS_PER_CHAR	10

void E4E_Println(const char* fmt, ...);
void E4E_Printf(const char* fmt, ...);
int getch(void);

#endif /* INC_DEBUG_CONIO_H_ */
