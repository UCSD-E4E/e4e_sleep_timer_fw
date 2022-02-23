/*
 * power_control.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */


#include <E4E_HAL_PowerControl.h>
#include <e4e_common.h>


int E4E_HAL_PwrCtrl_init(E4E_HAL_PWRCTRLDesc_t *pDesc,
		E4E_HAL_PWRCTRLDevice_e device, E4E_HAL_PWRCTRLConfig_t *pConfig)
{
	 if (pDesc == pHalDesc)
	     {
	         printf("E4E_OK");
	     }
	 if (device == E4E_HAL_PWRCTRL_Device_OBC)
	 {
		 printf("Powering OBC");
	 }
}

int E4E_HAL_PwrCtrl_deinit(E4E_HAL_PWRCTRLDesc_t *pDesc)
{
	if (pDesc == 0) return E4E_ERROR;

	else return E4E_OK;
}

int E4E_HAL_PwrCtrl_setState(E4E_HAL_PWRCTRLDesc_t *pDesc,
		E4E_HAL_PWRCTRL_State_e state)
{

	if(state == E4E_HAL_PWRCTRL_State_ON)
		{
			HAL_GPIO_WritePin(SYS_PWR_GPIO_Port, SYS_PWR_Pin, SET);
			printf("PWR ON");
			return E4E_OK;
		}
	else if(state == E4E_HAL_PWRCTRL_State_OFF)
		{
			HAL_GPIO_WritePin(SYS_PWR_GPIO_Port, SYS_PWR_Pin, RESET);
			printf("PWR OFF");
			return E4E_OK;
		}
	else
		{
			return E4E_ERROR;
		}

	if (pDesc == 0) return E4E_ERROR;
}
