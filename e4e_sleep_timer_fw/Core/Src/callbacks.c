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
        uint32_t msecs;
    }__attribute__((packed));
    int64_t currentTime;

    struct setAlarmArgs* pArgs = (struct setAlarmArgs*) payload;
    if(payloadLength != sizeof(struct setAlarmArgs))
        return E4E_ERROR;
    E4E_HAL_RTC_getTime(&pHalSystem->rtcDesc, &currentTime);
    currentTime += pArgs->msecs;
    E4E_HAL_PwrCtrl_setState(&pHalSystem->onboardComputerDesc, E4E_HAL_PWRCTRL_State_OFF);
    return E4E_HAL_RTC_setAlarm(&pHalSystem->rtcDesc, currentTime);
}

int getTimeCallback(void* payload, size_t payloadLength)
{
	int64_t currentTime;
	uint8_t msgBuffer[14] = {0};

	struct msgLayout{
		E4E_BinaryPacket_Header_t header;
		int64_t currentTime;
	} __attribute__((packed));

	struct msgLayout* msg = (struct msgLayout*)msgBuffer;

    if(E4E_ERROR == E4E_HAL_RTC_getTime(&pHalSystem->rtcDesc, &currentTime))
    {
    	currentTime = 0;
    }

    msg->header.start = 0xEBE4;
    msg->header.length = 14;
    msg->header.cmd_id = E4E_BinaryPacket_CCMD_GET_TIME_ACK;
    memcpy(&msg->currentTime, &currentTime, sizeof(int64_t));
    assert(sizeof(struct msgLayout) == 14);

    return E4E_HAL_Serial_write(&pHalSystem->debugSerialDesc, msgBuffer, 14, 1000);
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
