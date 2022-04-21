/*
 * power_control.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */


#include <E4E_HAL_PowerControl.h>
#include <e4e_common.h>
#include <stdio.h>
#include <main.h>


int E4E_HAL_PwrCtrl_init(E4E_HAL_PWRCTRLDesc_t *pDesc, E4E_HAL_PWRCTRLDevice_e device,E4E_HAL_PWRCTRLConfig_t *pConfig)
{//this function sets the OBC to default status ON unless user wants to initialize as OFF
	if (device == E4E_HAL_PWRCTRL_Device_OBC)
		{
			printf("Device: OBC"); //confirmation a valid device was selected
			if (pDesc == E4E_HAL_PWRCTRL_State_ON) //power descriptor is selected to be ON
			{
				E4E_HAL_PwrCtrl_setState(pDesc, E4E_HAL_PWRCTRL_State_ON); //set state to ON
				printf("Device: OBC ON"); //confirmation
				return E4E_OK;
			}

			else //if for some reason pDesc is set as OFF then proceed to set state as OFF
			{
				E4E_HAL_PwrCtrl_setState(pDesc, E4E_HAL_PWRCTRL_State_OFF); //set to OFF
				printf("Device: OBC OFF"); //confirmation
				return E4E_OK;
			}
		}

	else //if an undefined device is selected output error
		{
			return E4E_ERROR;
		}

}

int E4E_HAL_PwrCtrl_setState(E4E_HAL_PWRCTRLDesc_t *pDesc, E4E_HAL_PWRCTRL_State_e state)
{//sets state of output pin to MOSFET

	if(state == E4E_HAL_PWRCTRL_State_ON && pDesc == E4E_HAL_PWRCTRL_State_ON)
		{
			HAL_GPIO_WritePin(SYS_PWR_GPIO_Port, SYS_PWR_Pin, GPIO_PIN_SET);//sets pin high
			return E4E_OK;

		}
	else if(state == E4E_HAL_PWRCTRL_State_OFF && pDesc == E4E_HAL_PWRCTRL_State_OFF)
		{
			HAL_GPIO_WritePin(SYS_PWR_GPIO_Port, SYS_PWR_Pin, GPIO_PIN_RESET);//sets pin low
			return E4E_OK;
		}
	else
		{
			return E4E_ERROR;
		}

}



int E4E_HAL_PwrCtrl_deinit(E4E_HAL_PWRCTRLDesc_t *pDesc) //sets OBC to OFF
{

	if (pDesc == E4E_HAL_PWRCTRL_State_OFF) //defaults OBC to OFF
		{
			E4E_HAL_PwrCtrl_setState(pDesc, E4E_HAL_PWRCTRL_State_OFF);
			printf("Device: OBC OFF");
			return E4E_OK;
		}

	else
		{
			return E4E_ERROR;
		}

}
