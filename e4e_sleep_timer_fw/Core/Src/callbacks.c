#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <parser.h>
#include <HAL/E4E_HAL_RTC.h>
#include <HAL/E4E_HAL_PowerControl.h>
#include <Debug/conio.h>
#include <e4e_common.h>
#include <main.h>
#include <HAL/E4E_HAL_System.h>

int setAlarmCallback(void* payload, size_t payloadLength)
{
    struct setAlarmArgs {
        uint32_t secs;
    };
    struct setAlarmArgs* pArgs = (struct setAlarmArgs*) payload;
    if(payloadLength != sizeof(struct setAlarmArgs))
        return E4E_ERROR;
    return E4E_HAL_RTC_setAlarm(&pHalSystem->rtcDesc, pArgs ->secs* 1000);
}

int getTimeCallback(void* payload, size_t payloadLength)
{
    struct getTimeArgs {
        uint32_t secs;
    };
    struct getTimeArgs* pArgs = (struct getTimeArgs*) payload;
    if(payloadLength != sizeof(struct getTimeArgs))
        return E4E_ERROR;
    return E4E_HAL_RTC_getTime(&pHalSystem->rtcDesc, pArgs ->secs* 1000);
}

int clearAlarmCallback(void* payload, size_t payloadLength)
{
    if(payloadLength != 0)
        return E4E_ERROR;
    return E4E_HAL_RTC_clearAlarm(&pHalSystem->rtcDesc);
}

int E4E_App_AlarmExecuteCallback(int64_t alarmTime, void* pContext)
{
	return E4E_HAL_PwrCtrl_setState(&pHalSystem->onboardComputerDesc, E4E_HAL_PWRCTRL_State_ON);
}
