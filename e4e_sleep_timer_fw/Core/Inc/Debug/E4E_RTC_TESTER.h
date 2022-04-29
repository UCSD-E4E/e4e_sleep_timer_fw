
#ifndef INC_DEBUG_E4E_RTC_TESTER_H_
#define INC_DEBUG_E4E_RTC_TESTER_H_

#include <stdint.h>
#include <E4E_HAL_RTC.h>


int testAlarmCallback(int64_t alarmTime, void* pContext);
void setTimeAndAlarm(E4E_HAL_RTCDesc_t *pDesc);

#endif
