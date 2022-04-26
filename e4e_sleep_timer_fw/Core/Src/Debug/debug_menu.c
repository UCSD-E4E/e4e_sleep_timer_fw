/*
 * debug_menu.c
 *
 *  Created on: Feb 14, 2022
 *      Author: ntlhui
 */

#include <Debug/debug_menu.h>
#include <e4e_common.h>
#include <stdint.h>
#include <stm32g0xx.h>
#include <usart.h>
#include <Debug/conio.h>
#include <vers.h>
#include <E4E_HAL_Serial.h>
#include <E4E_HAL_System.h>

static int testWrite(void);
static int testRead(void);
static int E4E_DebugMenuCom1Echo(void);

E4E_DebugMenu_t testMenu[] =
{
		{'a', E4E_DebugMenuType_Cmd, NULL, testWrite, "Test Serial Write"},
		{'b', E4E_DebugMenuType_Cmd, NULL, testRead, "Test Serial Read"},
		{'e', E4E_DebugMenuType_Cmd, NULL, E4E_DebugMenuCom1Echo, "UART1 Echo"},
		{'\0', E4E_DebugMenuType_Null, NULL, NULL, NULL}
};

E4E_DebugMenu_t debugMenu[] =
{
{ '1', E4E_DebugMenuType_Menu, testMenu, NULL, "Serial Debug Menu"},
{ '\0', E4E_DebugMenuType_Null, NULL, NULL, NULL } };

void E4E_DebugApp(void)
{
	E4E_PrintBanner();
	while (1)
	{
		E4E_DebugMenuProcess(debugMenu);
	}
}

void E4E_DebugMenuDisplay(const E4E_DebugMenu_t *const pMenu)
{
	for (const E4E_DebugMenu_t *pEntry = pMenu; pEntry->command; pEntry++)
	{
		E4E_Println(" %c: %s", pEntry->command, pEntry->description);
	}
}

int E4E_DebugMenuProcess(const E4E_DebugMenu_t *const pMenu)
{
	char input;
	do
	{
		E4E_DebugMenuDisplay(pMenu);
		E4E_Printf(">");

		if (HAL_OK
				!= HAL_UART_Receive(&huart2, (uint8_t*) &input, 1, UINT32_MAX))
		{
			return E4E_ERROR;
		}
		E4E_Println("%c", input);
		if (input == ESC_CHAR)
		{
			return E4E_OK;
		}

		for (const E4E_DebugMenu_t *pEntry = pMenu; pEntry->command; pEntry++)
		{
			if (pEntry->command != input)
			{
				continue;
			}
			switch (pEntry->menuType)
			{
			case E4E_DebugMenuType_Menu:
				return E4E_DebugMenuProcess(pEntry->pSubMenu);
			case E4E_DebugMenuType_Cmd:
				return pEntry->pCommandFunc();
			default:
				return E4E_ERROR;
			}
		}
		E4E_Println(
				"Unknown input '%c', please enter one of the options below or ESC to exit",
				input);
	} while (1);

}

static int E4E_DebugMenuCom1Echo(void)
{
	const size_t BUFSIZE = 1;
	uint8_t buffer[BUFSIZE];
	E4E_HAL_SerialDesc_t* pSerial;

	pSerial = &pHalSystem->debugSerialDesc;

	while(1)
	{
		if(E4E_OK != E4E_HAL_Serial_read(pSerial, buffer, BUFSIZE, UINT32_MAX))
		{
			continue;
		}
		E4E_Printf("%c", buffer[0]);
	}
}


static int testWrite(void) {
	// testing command device
	E4E_HAL_SerialDesc_t *pDesc = &pHalSystem->commandSerialDesc;
	return E4E_HAL_Serial_write(pDesc, "Test message!\n\r", 14, 1000);
}

static int testRead(void) {
	uint8_t testBuf[15];
	E4E_HAL_SerialDesc_t *pDesc = &pHalSystem->commandSerialDesc;
	if (E4E_OK != E4E_HAL_Serial_read(pDesc, testBuf, 3, 1000)) {
		E4E_Println("Unable to retrieve character!");
		return E4E_ERROR;
	}
	else {
		E4E_Println(testBuf);
		return E4E_OK;
	}
}
