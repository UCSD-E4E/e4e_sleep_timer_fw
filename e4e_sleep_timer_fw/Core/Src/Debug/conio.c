/*
 * conio.c
 *
 *  Created on: Feb 14, 2022
 *      Author: ntlhui
 */

#include <Debug/conio.h>
#include <stdint.h>
#include <stdio.h>
#include <stm32g0xx_hal_def.h>
#include <stm32g0xx_hal_uart.h>
#include <string.h>

uint8_t E4E_printfBuffer[CONIO_PRINTF_BUFFER_LEN];

void E4E_Println(const char* fmt, ...)
{
	va_list vargs;
	va_start(vargs, fmt);
	vsnprintf(E4E_printfBuffer, CONIO_PRINTF_BUFFER_LEN, fmt, vargs);
	va_end(vargs);

	if(HAL_OK != HAL_UART_Transmit(&CONIO_HANDLE, E4E_printfBuffer, strlen(E4E_printfBuffer), CONIO_MS_PER_CHAR * strlen(E4E_printfBuffer)))
	{
		return;
	}
	if(HAL_OK != HAL_UART_Transmit(&CONIO_HANDLE, "\r\n", 2, 2 * CONIO_MS_PER_CHAR))
	{
		return;
	}
}

void E4E_Printf(const char* fmt, ...)
{
	va_list vargs;
	va_start(vargs, fmt);
	vsnprintf(E4E_printfBuffer, CONIO_PRINTF_BUFFER_LEN, fmt, vargs);
	va_end(vargs);
	if(HAL_OK != HAL_UART_Transmit(&CONIO_HANDLE, E4E_printfBuffer, strlen(E4E_printfBuffer), CONIO_MS_PER_CHAR * strlen(E4E_printfBuffer)))
	{
		return;
	}
}

int getch(void)
{
	char input;
	if(HAL_OK != HAL_UART_Receive(&CONIO_HANDLE, &input, 1, UINT32_MAX))
	{
		return 0;
	}
	return input;
}

int E4E_gets(char* pBuffer, int buflen)
{
	int i;
	memset(pBuffer, 0, buflen);
	for(i = 0; i < buflen-1; i++)
	{
		pBuffer[i] = getch();
		switch(pBuffer[i])
		{
		case '\r':
		case '\n':
			pBuffer[i] = '\0';
			E4E_Printf("\r\n");
			return i;
		case '\x08':
			i--;
			E4E_Printf("\b \b");
			break;
		default:
			E4E_Printf("%c", pBuffer[i]);
			break;
		}
	}
	pBuffer[buflen - 1] = 0;
	return buflen - 1;
}
