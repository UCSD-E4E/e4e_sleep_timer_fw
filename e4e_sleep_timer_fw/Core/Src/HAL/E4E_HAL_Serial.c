/*
 * serial.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#include <E4E_HAL_Serial.h>
#include <e4e_common.h>

/**
 * @internal This function should initialize the serial driver descriptor with
 * any buffers, interrupts, and flags.
 */
int E4E_HAL_Serial_init(E4E_HAL_SerialDesc_t *pDesc,
		E4E_HAL_SerialDevice_e device, E4E_HAL_SerialConfig_t *pConfig) {
	return E4E_ERROR;
}
int E4E_HAL_Serial_deinit(E4E_HAL_SerialDesc_t *pDesc) {
	return E4E_ERROR;
}

/**
 * @internal This function should either read `count` bytes from the buffer or
 * none at all.  It should never read less than `count` bytes.
 */
int E4E_HAL_Serial_read(E4E_HAL_SerialDesc_t *pDesc, void *pBuffer,
		size_t count, uint32_t timeout) {
	return E4E_ERROR;
}
int E4E_HAL_Serial_write(E4E_HAL_SerialDesc_t *pDesc, const void *pData,
		size_t nBytes, uint32_t timeout) {
	return E4E_ERROR;
}
int E4E_HAL_Serial_flush(E4E_HAL_SerialDesc_t *pDesc) {
	return E4E_ERROR;
}
