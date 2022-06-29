/*
 * power_control.c
 *
 *  Created on: Feb 9, 2022
 *      Author: frankc
 */


#include <E4E_HAL_PowerControl.h>
#include <Debug/conio.h>
#include <e4e_common.h>
#include <stdio.h>
#include <main.h>


/**
 * @brief Initializes the Power Control Driver (sets to ON)
 * @param pDesc	Power Control Driver to initialize
 * @param device	Power Control Device to initialize
 * @return E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_PwrCtrl_init(E4E_HAL_PWRCTRLDesc_t *pDesc, E4E_HAL_PWRCTRLDevice_e device, E4E_HAL_PWRCTRLConfig_t *pConfig)
{
	if (device == E4E_HAL_PWRCTRL_Device_OBC)
		{
		E4E_Printf("Device: OBC"); //confirmation a valid device was selected
			if (pDesc == E4E_HAL_PWRCTRL_State_ON) //power descriptor is selected to be ON
			{
				E4E_HAL_PwrCtrl_setState(pDesc, E4E_HAL_PWRCTRL_State_ON); //set state to ON
				E4E_Printf("Device: OBC ON"); //confirmation
				return E4E_OK;
			}

			else //if for some reason pDesc is set as OFF then proceed to set state as OFF
			{
				E4E_HAL_PwrCtrl_setState(pDesc, E4E_HAL_PWRCTRL_State_OFF); //set to OFF
				E4E_Printf("Device: OBC OFF"); //confirmation
				return E4E_OK;
			}
		}

	else //if an undefined device is selected output error
		{
			return E4E_ERROR;
		}

}

/**
 * @brief Sets the Power Control device state (ON or OFF)
 * @param pDesc	Power Control Driver
 * @param state	Power Control Device state to set
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_PwrCtrl_setState(E4E_HAL_PWRCTRLDesc_t *pDesc,E4E_HAL_PWRCTRL_State_e state)

{



	if(E4E_HAL_PWRCTRL_State_ON == state)
		{
			HAL_GPIO_WritePin(SYS_PWR_GPIO_Port, SYS_PWR_Pin, GPIO_PIN_SET);//sets pin high
			return E4E_OK;

		}
	else if(E4E_HAL_PWRCTRL_State_OFF == state)
		{
			HAL_GPIO_WritePin(SYS_PWR_GPIO_Port, SYS_PWR_Pin, GPIO_PIN_RESET);//sets pin low
			return E4E_OK;
		}
	else
		{
			return E4E_ERROR;
		}

}


/**
 * @brief Deinitializes the Power Control Driver (sets to OFF)
 * @param pDesc	Power Control Driver to deinitialize
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_PwrCtrl_deinit(E4E_HAL_PWRCTRLDesc_t *pDesc)
{


	if (pDesc == E4E_HAL_PWRCTRL_State_OFF) //defaults OBC to OFF
		{
			E4E_HAL_PwrCtrl_setState(pDesc, E4E_HAL_PWRCTRL_State_OFF);
			E4E_Printf("Device: OBC OFF");
			return E4E_OK;
		}

	else //in the case the OBC fails to de-initialize
		{
			return E4E_ERROR;
		}

}
