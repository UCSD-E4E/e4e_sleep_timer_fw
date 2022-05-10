#include <Debug/E4E_RTC_TESTER.h>
#include <Debug/conio.h>
#include <e4e_common.h>
#include <stdio.h>


  int64_t testStartTime = 1649194687000; 		//Values for Testing
  int64_t testAlarmTime = 1649194697000;


int testAlarmCallback(int64_t alarmTime, void* pContext)
{
	//Test Alarm Callback
	char* buf = "Alarm Callback\r\n";
	E4E_Printf(buf);

	return E4E_OK;
}


void setTimeAndAlarm(E4E_HAL_RTCDesc_t *pDesc){

	  //Test All RTC Functionality
	  E4E_HAL_RTC_clearAlarm(pDesc);
	  E4E_HAL_RTC_setTime(pDesc, testStartTime);

	  E4E_HAL_RTC_setAlarm(pDesc, testAlarmTime);
	  E4E_HAL_RTC_registerAlarmCallback(pDesc, &testAlarmCallback, NULL);


}
