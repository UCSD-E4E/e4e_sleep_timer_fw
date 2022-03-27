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

E4E_DebugMenu_t testMenu[] =
{
		{'a', E4E_DebugMenuType_Cmd, NULL, testWrite, "Test Serial Write"},
		{'b', E4E_DebugMenuType_Cmd, NULL, testRead, "Test Serial Read"},
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
