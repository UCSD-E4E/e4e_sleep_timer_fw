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
	if (pDesc == 0) return E4E_ERROR;
	switch(device) {
	case E4E_HAL_SerialDevice_Command:
		// initialize the port for serial device
		pDesc->uartHandle = &hlpuart1;
		break;
	case E4E_HAL_SerialDevice_Debug:
		// initialize the debug port
		pDesc->uartHandle = &huart1;
		break;
	default: return E4E_ERROR;
	};
	pDesc->pAttrDesc = pConfig;
	return E4E_OK;
}

int E4E_HAL_Serial_deinit(E4E_HAL_SerialDesc_t *pDesc) {
	if (pDesc == 0) return E4E_ERROR;
	if (pDesc->uartHandle == 0) return E4E_ERROR;
	HAL_UART_MspDeInit(pDesc->uartHandle);
	return E4E_OK;
}

/**
 * @internal This function should either read `count` bytes from the buffer or
 * none at all.  It should never read less than `count` bytes.
 */
int E4E_HAL_Serial_read(E4E_HAL_SerialDesc_t *pDesc, void *pBuffer,
		size_t count, uint32_t timeout) {
	uint8_t * pBuf = (uint8_t *) pBuffer;
	if (pDesc == 0) return E4E_ERROR;
	if (pDesc->pAttrDesc == 0) return E4E_ERROR;
	if (pDesc->uartHandle == 0) return E4E_ERROR;
    switch(pDesc->pAttrDesc->serial_mode) {
    case E4E_Serial_Polling:
    	return (HAL_OK == HAL_UART_Receive(pDesc->uartHandle, pBuf, count, pDesc->pAttrDesc->timeout))
    			? E4E_OK : E4E_ERROR;
    case E4E_Serial_IT:
    	return (HAL_OK == HAL_UART_Receive_IT(pDesc->uartHandle, pBuf,count)) ? E4E_OK : E4E_ERROR;
    case E4E_Serial_DMA:
    	return (HAL_OK == HAL_UART_Receive_DMA(pDesc->uartHandle, pBuf, count)) ? E4E_OK: E4E_ERROR;
    default: return E4E_ERROR;
    }
}

int E4E_HAL_Serial_write(E4E_HAL_SerialDesc_t *pDesc, const void *pData,
		size_t nBytes, uint32_t timeout) {
	uint8_t * pBuf = (uint8_t *) pData;
	if (pDesc == 0) return E4E_ERROR;
	if (pDesc->pAttrDesc == 0) return E4E_ERROR;
	if (pDesc->uartHandle == 0) return E4E_ERROR;
	switch(pDesc->pAttrDesc->serial_mode) {
	case E4E_Serial_Polling:
		return (HAL_OK == HAL_UART_Transmit(pDesc->uartHandle, pBuf, nBytes, pDesc->pAttrDesc->timeout))
				? E4E_OK : E4E_ERROR;
	case E4E_Serial_IT:
		return (HAL_OK == HAL_UART_Transmit_IT(pDesc->uartHandle, pBuf, nBytes)) ? E4E_OK : E4E_ERROR;
	case E4E_Serial_DMA:
		return (HAL_OK == HAL_UART_Transmit_DMA(pDesc->uartHandle, pBuf, nBytes)) ? E4E_OK: E4E_ERROR;
	default: return E4E_ERROR;
	}
}

int E4E_HAL_Serial_flush(E4E_HAL_SerialDesc_t *pDesc) {
	if (pDesc == 0) return E4E_ERROR;
	if (pDesc->uartHandle == 0) return E4E_ERROR;

	uint8_t* txBuffPtr = pDesc->uartHandle->pTxBuffPtr;
	uint8_t* rxBuffPtr = pDesc->uartHandle->pRxBuffPtr;
	uint16_t txBufSize = pDesc->uartHandle->TxXferSize;
	uint16_t rxBufSize = pDesc->uartHandle->RxXferSize;

	if (txBuffPtr == 0 || rxBuffPtr == 0) return E4E_ERROR;

	// will exit when txBufSize is 0
	while (txBufSize--) {
		*txBuffPtr = 0; // write zero to the txBuffer
		txBuffPtr++;
	}

	// will exit when rxBufSize is 0
	while (rxBufSize--) {
		*rxBuffPtr = 0; // write zero to the rxBuffer
		rxBuffPtr++;
	}

}
