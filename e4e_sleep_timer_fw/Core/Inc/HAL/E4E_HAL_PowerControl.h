/*
 * power_control.h
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#ifndef INC_HAL_E4E_HAL_POWERCONTROL_H_
#define INC_HAL_E4E_HAL_POWERCONTROL_H_

/**
 * Power Control Descriptor
 */
typedef struct E4E_HAL_PWRCTRLDesc_
{
	/**
	 * HAL Descriptor
	 */
	void *pHalDesc;
	/**
	 * Attributes
	 */
	void *pAttrDesc;
} E4E_HAL_PWRCTRLDesc_t;

/**
 * Power Control Configuration
 */
typedef struct E4E_HAL_PWRCTRLConfig_
{
	int dummy;
} E4E_HAL_PWRCTRLConfig_t;

/**
 * Power Control Devices
 */
typedef enum E4E_HAL_PWRCTRLDevice_
{
	E4E_HAL_PWRCTRL_Device_OBC,   /**< On-board Computer */
	E4E_HAL_PWRCTRL_Device__NELEMS/**< Number of devices */
} E4E_HAL_PWRCTRLDevice_e;

/**
 * Power Control States
 */
typedef enum E4E_HAL_PWRCTRLState_
{
	E4E_HAL_PWRCTRL_State_ON,     /**< On */
	E4E_HAL_PWRCTRL_State_OFF,    /**< Off */
	E4E_HAL_PWRCTRL_State__NELEMS,/**< Number of states */
} E4E_HAL_PWRCTRL_State_e;

/**
 * @brief Initializes the Power Control Driver
 * @param pDesc	Power Control Driver to initialize
 * @param device	Power Control Device to initialize
 * @param pConfig	Power Control Device Configuration
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_PwrCtrl_init(E4E_HAL_PWRCTRLDesc_t *pDesc,
		E4E_HAL_PWRCTRLDevice_e device, E4E_HAL_PWRCTRLConfig_t *pConfig);
/**
 * @brief Deinitializes the Power Control Driver
 * @param pDesc	Power Control Driver to deinitialize
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_PwrCtrl_deinit(E4E_HAL_PWRCTRLDesc_t *pDesc);
/**
 * @brief Sets the Power Control device state
 * @param pDesc	Power Control Driver
 * @param state	Power Control Device state to set
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_PwrCtrl_setState(E4E_HAL_PWRCTRLDesc_t *pDesc,
		E4E_HAL_PWRCTRL_State_e state);

#endif /* INC_HAL_E4E_HAL_POWERCONTROL_H_ */
