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

	if (NULL == pDesc)
		{
			return E4E_ERROR;
		}

	if (E4E_HAL_PWRCTRL_Device__NELEMS <= device)
		{
			return E4E_ERROR;
		}
	else if (E4E_HAL_PWRCTRL_Device_OBC == device)
		{
			E4E_Printf("Device: OBC"); //confirmation a valid device was selected
			HAL_GPIO_WritePin(SYS_PWR_GPIO_Port, SYS_PWR_Pin, GPIO_PIN_SET); //set state to ON
			E4E_Printf("Device: OBC ON"); //confirmation
			return E4E_OK;
		}
	else
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
	if (NULL == pDesc)
		{
			return E4E_ERROR;
		}

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
	if (NULL == pDesc)
		{
			return E4E_ERROR;
		}

	else
		{
			HAL_GPIO_WritePin(SYS_PWR_GPIO_Port, SYS_PWR_Pin, GPIO_PIN_RESET);//sets pin low
			E4E_Printf("Device: OBC OFF");
			pDesc = NULL;
			return E4E_OK;
		}

}
