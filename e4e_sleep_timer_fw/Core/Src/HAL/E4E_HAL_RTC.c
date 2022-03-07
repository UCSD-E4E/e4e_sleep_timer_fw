/*
 * rtc.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#include <time.h>
#include <E4E_HAL_RTC.h>
#include <rtc.h>
#include <e4e_common.h>
#include <stdio.h>
#include <usart.h>


#define MS_TO_SEC 1000
void (*alarmCallbackFunction)();
void (*alarmCallbackFunctionContext)();
int64_t alarmTime;


int E4E_HAL_RTC_init(E4E_HAL_RTCDesc_t *pDesc, E4E_HAL_RTCConfig_t *pConfig)
{
	//TODO: Finish Implementation
	pDesc->pHalDesc=&hrtc;

	//Attributes: secondFraction, alarm Mask(s), Daylight Savings,

	return E4E_OK;
}
int E4E_HAL_RTC_deinit(E4E_HAL_RTCDesc_t *pDesc)
{
	if(HAL_RTC_DeInit(&hrtc) != HAL_OK){
		return E4E_ERROR;
	}
	return E4E_OK;

}
int E4E_HAL_RTC_getTime(E4E_HAL_RTCDesc_t *pDesc, int64_t *pDatetime)
{
	int buf[300];
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	struct tm time;


	if(HAL_RTC_GetTime(pDesc->pHalDesc, &sTime, RTC_FORMAT_BIN) != HAL_OK){
		return E4E_ERROR;
	}

	if(HAL_RTC_GetDate(pDesc->pHalDesc, &sDate, RTC_FORMAT_BIN) != HAL_OK){
			return E4E_ERROR;
	}

	time.tm_year = (uint8_t)(sDate.Year + 2000 - 1900);
	time.tm_mon = (uint8_t)(sDate.Month - 1);
	time.tm_mday = (uint8_t)sDate.Date;
	time.tm_wday = (uint8_t)sDate.WeekDay;

	time.tm_hour = (uint8_t)sTime.Hours;
	time.tm_min = (uint8_t)sTime.Minutes;
	time.tm_sec = (uint8_t)sTime.Seconds;

	//TODO: Verify Sub-Seconds works as expected
	int sec_Fraction = (int)(((float)(sTime.SecondFraction - sTime.SubSeconds))/((float)sTime.SecondFraction+1)*MS_TO_SEC);

	*pDatetime = mktime(&time)*MS_TO_SEC + sec_Fraction;

	sprintf((char*)buf,"STM Time -  %d/%d/%d   %d:%d:%d:%d\r\n", sDate.Month, sDate.Date, sDate.Year, sTime.Hours, sTime.Minutes, sTime.Seconds,sec_Fraction);
	HAL_UART_Transmit(&huart2, buf, strlen((char*)buf),HAL_MAX_DELAY);

	return E4E_OK;
}
int E4E_HAL_RTC_setTime(E4E_HAL_RTCDesc_t *pDesc, int64_t datetime)
{
	//https://currentmillis.com/
	//Use for Generating Time
	int buf[50];

	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	time_t datetime_seconds = (time_t)(datetime/MS_TO_SEC);
	struct tm time = *gmtime(&datetime_seconds);

	sDate.Year = (uint8_t)(time.tm_year + 1900 - 2000);
	sDate.Month = (uint8_t)(time.tm_mon + 1);
	sDate.Date = (uint8_t)time.tm_mday;
	sDate.WeekDay = (uint8_t)time.tm_wday;

	sTime.Hours = (uint8_t)time.tm_hour;
	sTime.Minutes = (uint8_t)time.tm_min;
	sTime.Seconds = (uint8_t)time.tm_sec;

	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.SecondFraction = 255;

	//TODO: Verify Sub-Seconds works as expected
	int sec_Fraction = (datetime % MS_TO_SEC);

	sTime.SubSeconds = sTime.SecondFraction - (float)sec_Fraction*(sTime.SecondFraction +1)/MS_TO_SEC;

	sprintf((char*)buf,"Start Date: %d/%d/%d Time: %d:%d:%d:%d\r\n", sDate.Month, sDate.Date, sDate.Year, sTime.Hours, sTime.Minutes, sTime.Seconds, sec_Fraction);
	HAL_UART_Transmit(&huart2, buf, strlen((char*)buf),HAL_MAX_DELAY);


	if(HAL_RTC_SetTime(pDesc->pHalDesc, &sTime, RTC_FORMAT_BIN) != HAL_OK){
		return E4E_ERROR;
	}

	if(HAL_RTC_SetDate(pDesc->pHalDesc, &sDate, RTC_FORMAT_BIN) != HAL_OK){
		return E4E_ERROR;
	}

	return E4E_OK;
}

int E4E_HAL_RTC_setAlarm(E4E_HAL_RTCDesc_t *pDesc, int64_t alarm)
{
	RTC_TimeTypeDef sTime;
	RTC_AlarmTypeDef sAlarm;
	alarmTime = alarm;

	time_t alarmtime_seconds = (time_t)(alarm/MS_TO_SEC);
	struct tm time = *gmtime(&alarmtime_seconds);

	sTime.Hours = time.tm_hour;
	sTime.Minutes = time.tm_min;
	sTime.Seconds = time.tm_sec;

	int sec_Fraction = (alarm % MS_TO_SEC);
	sTime.SubSeconds = sTime.SecondFraction - (float)sec_Fraction*(sTime.SecondFraction +1)/MS_TO_SEC;

	sAlarm.Alarm = RTC_ALARM_A;
	sAlarm.AlarmTime = sTime;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = time.tm_mday;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;

	if(HAL_RTC_SetAlarm_IT(pDesc->pHalDesc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK){
		return E4E_ERROR;
	}
	return E4E_OK;

}

int E4E_HAL_RTC_clearAlarm(E4E_HAL_RTCDesc_t *pDesc)
{
	if(HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A) != HAL_OK){
		return E4E_ERROR;
	}
	return E4E_OK;
}

//TODO:
int E4E_HAL_RTC_registerAlarmCallback(E4E_HAL_RTCDesc_t *pDesc,
		E4E_HAL_RTCAlarmCallback pCallback, void* pContext)
{
	alarmCallbackFunction = pCallback;
	alarmCallbackFunctionContext = pContext;
	return E4E_OK;
}


void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	int buf[50];
	sprintf((char*)buf,"Alarm Triggered\r\n");
	HAL_UART_Transmit(&huart2, buf, strlen((char*)buf),HAL_MAX_DELAY);

	(*alarmCallbackFunction)(&alarmTime, alarmCallbackFunctionContext);
}


/**
 * @internal This function should check the value of the RTC Backup registers.
 * The RTC backup registers should be set with some specific value to hold so
 * that on the next reset without power loss, that value is still recognizable
 * and we don't reset the time.  Otherwise, if that specific value is not
 * present, then we know that we've lost the RTC time and can set some default
 * value.
 */
int E4E_HAL_RTC_initializationCheck(void)
{
	return 0;
}

