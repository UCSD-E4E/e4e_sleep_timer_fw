/*
 * rtc.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#include <E4E_HAL_RTC.h>
#include <e4e_common.h>

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
	return E4E_ERROR;
}
int E4E_HAL_RTC_setTime(E4E_HAL_RTCDesc_t *pDesc, int64_t datetime)
{
	return E4E_ERROR;
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

