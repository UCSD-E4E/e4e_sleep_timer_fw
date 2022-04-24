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
#include <E4E_HAL_System.h>
#include <stdio.h>

static int E4E_DebugMenu_RTCGetTime(void);
static int E4E_DebugMenu_RTCGetRollingHexTime(void);
static int E4E_DebugMenu_RTCSetTime(void);
static int E4E_DebugMenu_RTCSetAlarm(void);
static int E4E_DebugMenu_RTCAlarmCb(int64_t, void*);



E4E_DebugMenu_t RTCDebugMenu[] =
{
		{'1', E4E_DebugMenuType_Cmd, NULL, E4E_DebugMenu_RTCGetTime, "Get RTC Time"},
		{'2', E4E_DebugMenuType_Cmd, NULL, E4E_DebugMenu_RTCSetTime, "Set RTC Time"},
		{'3', E4E_DebugMenuType_Cmd, NULL, E4E_DebugMenu_RTCGetRollingHexTime, "Hex Clock"},
		{'4', E4E_DebugMenuType_Cmd, NULL, E4E_DebugMenu_RTCSetAlarm, "Set Alarm"},
		{ '\0', E4E_DebugMenuType_Null, NULL, NULL, NULL }
};

E4E_DebugMenu_t debugMenu[] =
{
		{'2', E4E_DebugMenuType_Menu, RTCDebugMenu, NULL, "RTC Debug Menu"},
		{ '\0', E4E_DebugMenuType_Null, NULL, NULL, NULL }
};

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

static int E4E_DebugMenu_RTCGetTime(void)
{
	E4E_HAL_RTCDesc_t* pRTC;
	union {
		int64_t time;
		uint32_t halves[2];
	} time;

	pRTC = &pHalSystem->rtcDesc;

	if(E4E_OK != E4E_HAL_RTC_getTime(pRTC, &time.time))
	{
		return E4E_ERROR;
	}

	E4E_Println("RTC Time: %08lx%08lx", time.halves[1], time.halves[0]);
	return E4E_OK;
}

static int E4E_DebugMenu_RTCGetRollingHexTime(void)
{
	E4E_HAL_RTCDesc_t* pRTC;
	union {
		int64_t time;
		uint32_t halves[2];
	} time;

	pRTC = &pHalSystem->rtcDesc;

	for(int i = 0; i < 1000; i++)
	{

		if(E4E_OK != E4E_HAL_RTC_getTime(pRTC, &time.time))
		{
			return E4E_ERROR;
		}

		E4E_Println("RTC Time: %08lx%08lx", time.halves[1], time.halves[0]);
	}
	return E4E_OK;
}

static int E4E_DebugMenu_RTCSetTime(void)
{
	char strBuf[64];
	union {
		int64_t time;
		uint32_t halves[2];
	} timeToSet;
	E4E_HAL_RTCDesc_t* pRTC;

	pRTC = &pHalSystem->rtcDesc;

	E4E_Printf("Upper Half Time: ");
	E4E_gets(strBuf, 64);

	if(1 != sscanf(strBuf, "%lx", &timeToSet.halves[1]))
	{
		return E4E_ERROR;
	}

	E4E_Printf("Lower Half Time: ");
	E4E_gets(strBuf, 64);

	if(1 != sscanf(strBuf, "%lx", &timeToSet.halves[0]))
	{
		return E4E_ERROR;
	}


	if(E4E_OK != E4E_HAL_RTC_setTime(pRTC, timeToSet.time))
	{
		return E4E_ERROR;
	}
	return E4E_OK;
}

static int E4E_DebugMenu_RTCSetAlarm(void)
{
	char strBuf[64];
	union {
		int64_t time;
		uint32_t halves[2];
	} timeToSet;
	E4E_HAL_RTCDesc_t* pRTC;
	volatile int alarmFlag = 0;

	pRTC = &pHalSystem->rtcDesc;

	E4E_Printf("Upper Half Alarm: ");
	E4E_gets(strBuf, 64);

	if(1 != sscanf(strBuf, "%lx", &timeToSet.halves[1]))
	{
		return E4E_ERROR;
	}

	E4E_Printf("Lower Half Alarm: ");
	E4E_gets(strBuf, 64);

	if(1 != sscanf(strBuf, "%lx", &timeToSet.halves[0]))
	{
		return E4E_ERROR;
	}

	if(E4E_OK != E4E_HAL_RTC_registerAlarmCallback(pRTC, E4E_DebugMenu_RTCAlarmCb, &alarmFlag))
	{
		return E4E_ERROR;
	}


	if(E4E_OK != E4E_HAL_RTC_setAlarm(pRTC, timeToSet.time))
	{
		return E4E_ERROR;
	}

	while(alarmFlag == 0)
	{

	}
	E4E_Println("Flag is now %d", alarmFlag);

	return E4E_OK;
}

static int E4E_DebugMenu_RTCAlarmCb(int64_t alarmTime, void* pContext)
{
	volatile int* pFlag = pContext;
	if(NULL == pFlag)
	{
		return E4E_ERROR;
	}

	(*pFlag)++;
	return E4E_OK;
}
