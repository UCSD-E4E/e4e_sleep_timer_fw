/*
 * rtc.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#include <rtc.h>
#include <E4E_HAL_RTC.h>
#include <e4e_common.h>
#include <time.h>

int E4E_HAL_RTC_init(E4E_HAL_RTCDesc_t *pDesc, E4E_HAL_RTCConfig_t *pConfig)
{
	return E4E_ERROR;
}
int E4E_HAL_RTC_deinit(E4E_HAL_RTCDesc_t *pDesc)
{
	return E4E_ERROR;
}
int E4E_HAL_RTC_getTime(E4E_HAL_RTCDesc_t *pDesc, int64_t *pDatetime)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	sTime.SecondFraction = 2;

	struct tm convertedTime;

	if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		return E4E_ERROR;
	}

	if (HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		return E4E_ERROR;
	}

    convertedTime.tm_wday = sDate.WeekDay;
    convertedTime.tm_mon  = sDate.Month - 1;
    convertedTime.tm_mday = sDate.Date;
    convertedTime.tm_year = sDate.Year + 100;

    convertedTime.tm_hour = sTime.Hours;
    convertedTime.tm_min  = sTime.Minutes;
    convertedTime.tm_sec  = sTime.Seconds;

    *pDatetime = (mktime(&convertedTime) * 1000) + sTime.SubSeconds;

	return E4E_OK;
}
int E4E_HAL_RTC_setTime(E4E_HAL_RTCDesc_t *pDesc, int64_t datetime)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;

	struct tm* convertedTime = gmtime(datetime);

	sTime.Hours = convertedTime->tm_hour;
	sTime.Minutes = convertedTime->tm_min;
	sTime.Seconds = convertedTime->tm_sec;
	sTime.SubSeconds = datetime % 1000;

	sDate.Year = convertedTime->tm_year;
	sDate.Month = convertedTime->tm_mon;
	sDate.Date = convertedTime->tm_mday;
	sDate.WeekDay = convertedTime->tm_wday;

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		return E4E_ERROR;
	}
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		return E4E_ERROR;
	}


	return E4E_OK;
}
int E4E_HAL_RTC_setAlarm(E4E_HAL_RTCDesc_t *pDesc, int64_t alarm)
{
	return E4E_ERROR;
}
int E4E_HAL_RTC_clearAlarm(E4E_HAL_RTCDesc_t *pDesc)
{
	return E4E_ERROR;
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

