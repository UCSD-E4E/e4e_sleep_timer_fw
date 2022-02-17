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


int E4E_HAL_RTC_init(E4E_HAL_RTCDesc_t *pDesc, E4E_HAL_RTCConfig_t *pConfig)
{
	pDesc->pHalDesc=&hrtc;

//	if (HAL_RTC_Init(pDesc->pHalDesc) != HAL_OK)
//	{
//	    return E4E_ERROR;
//	}
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

	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	struct tm time;


	if(HAL_RTC_GetTime(pDesc->pHalDesc, &sTime, RTC_FORMAT_BIN) != HAL_OK){
		return E4E_ERROR;
	}

	if(HAL_RTC_GetDate(pDesc->pHalDesc, &sDate, RTC_FORMAT_BIN) != HAL_OK){
			return E4E_ERROR;
	}

	time.tm_year = sDate.Year;
	time.tm_mon = sDate.Month;
	time.tm_mday = sDate.Date;

	time.tm_hour = sTime.Hours;
	time.tm_min = sTime.Minutes;
	time.tm_sec = sTime.Seconds;

	//Add Sub-Seconds


	*pDatetime = mktime(&time)*1000;

	return E4E_OK;
}
int E4E_HAL_RTC_setTime(E4E_HAL_RTCDesc_t *pDesc, int64_t datetime)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	struct tm time = *gmtime(datetime/1000);

	sDate.Year = time.tm_year;
	sDate.Month = time.tm_mon;
	sDate.Date = time.tm_mday;

	sTime.Hours = time.tm_hour;
	sTime.Minutes = time.tm_min;
	sTime.Seconds = time.tm_sec;

	//Add Sub-Seconds

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
	if(HAL_RTC_SetAlarm(&hrtc, alarm, RTC_FORMAT_BIN) != HAL_OK){
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
int E4E_HAL_RTC_registerAlarmCallback(E4E_HAL_RTCDesc_t *pDesc,
		E4E_HAL_RTCAlarmCallback pCallback, void* pContext)
{
	return E4E_ERROR;
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

