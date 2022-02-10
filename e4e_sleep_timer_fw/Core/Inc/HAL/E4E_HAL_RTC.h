/*
 * rtc.h
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#ifndef INC_HAL_E4E_HAL_RTC_H_
#define INC_HAL_E4E_HAL_RTC_H_

#include <stdint.h>

/**
 * RTC Descriptor
 */
typedef struct E4E_HAL_RTCDesc_
{
	void *pHalDesc;
	void *pAttrDesc;
} E4E_HAL_RTCDesc_t;

/**
 * RTC Configuration
 */
typedef struct E4E_HAL_RTCConfig_
{
	int dummy;
} E4E_HAL_RTCConfig_t;

/**
 * RTC Alarm Callback Function Pointer Type
 * @param alarmTime	RTC Alarm Time
 * @param pContext	Callback Context
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
typedef int (*E4E_HAL_RTCAlarmCallback)(int64_t alarmTime, void* pContext);

/**
 * @brief Initializes the RTC
 * @param pDesc	RTC Descriptor to initialize
 * @param pConfig	Configuration struct
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_RTC_init(E4E_HAL_RTCDesc_t *pDesc, E4E_HAL_RTCConfig_t *pConfig);
/**
 * @brief Deinitializes the RTC
 * @param pDesc	RTC Descriptor to deinitialize
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_RTC_deinit(E4E_HAL_RTCDesc_t *pDesc);
/**
 * @brief Retrieves the currently set time in the RTC as milliseconds since
 * the Unix Epoch
 * @param pDesc	RTC Descriptor
 * @param pDatetime	Variable to populate with current time
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_RTC_getTime(E4E_HAL_RTCDesc_t *pDesc, int64_t *pDatetime);
/**
 * @brief Sets the current time in the RTC as milliseconds since the Unix Epoch
 * @param pDesc	RTC Descriptor
 * @param datetime	Time to set
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_RTC_setTime(E4E_HAL_RTCDesc_t *pDesc, int64_t datetime);
/**
 * @brief Sets an alarm at the specified time in milliseconds since the Unix
 * Epoch
 * @param pDesc	RTC Descriptor
 * @param alarm	Alarm time
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_RTC_setAlarm(E4E_HAL_RTCDesc_t *pDesc, int64_t alarm);
/**
 * @brief Clears the current alarm
 * @param pDesc	RTC Descriptor
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_RTC_clearAlarm(E4E_HAL_RTCDesc_t *pDesc);
/**
 * @brief Registers the callback as the callback to execute when an alarm occurs
 * @param pDesc	RTC Descriptor
 * @param pCallback	Callback function pointer
 * @param pContext Callback context
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_RTC_registerAlarmCallback(E4E_HAL_RTCDesc_t *pDesc,
		E4E_HAL_RTCAlarmCallback pCallback, void* pContext);
/**
 * Determines whether or not the RTC was previously set.
 * @return	1 if the RTC was not previously set and needs to be initialized,
 * otherwise 0 if the RTC has a valid time
 */
int E4E_HAL_RTC_initializationCheck(void);
#endif /* INC_HAL_E4E_HAL_RTC_H_ */
