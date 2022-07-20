
#ifndef INC_DEBUG_E4E_RTC_TESTER_H_
#define INC_DEBUG_E4E_RTC_TESTER_H_

#include <stdint.h>
#include <E4E_HAL_RTC.h>

/**
 * Alarm Callback Test Function
 *
 * @param alarmTime	RTC Alarm Time
 * @param pContext Callback Context
 * @return
 */
int testAlarmCallback(int64_t alarmTime, void* pContext);

/**
 * Test RTC Functionality by initializing the RTC time and setting
 * an Alarm.
 *
 * @param pDesc	RTC Descriptor
 */
void setTimeAndAlarm(E4E_HAL_RTCDesc_t *pDesc);

#endif
