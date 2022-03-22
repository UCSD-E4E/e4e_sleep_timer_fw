/*
 * serial.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#include <string.h>
#include <E4E_HAL_Serial.h>
#include <e4e_common.h>

E4E_UARTHandle_To_SerialDesc_t map_command, *pmap_command = &map_command;
E4E_UARTHandle_To_SerialDesc_t map_debug, *pmap_debug = &map_debug;

E4E_HAL_SerialDesc_t * get_desc_from_handle(UART_HandleTypeDef *huart) {
	if (huart == &hlpuart1) {
		return pmap_command->e4eSerialDesc;
	} else if (huart == &huart1) {
		return pmap_debug->e4eSerialDesc;
	}
	return NULL;
}

/**
 * @internal This function should initialize the serial driver descriptor with
 * any buffers, interrupts, and flags.
 */
int E4E_HAL_Serial_init(E4E_HAL_SerialDesc_t *pDesc,
		E4E_HAL_SerialDevice_e device, E4E_HAL_SerialConfig_t *pConfig) {
	// initialize ring buffer
	RBuf_Attr_t attr = {sizeof(pDesc->rbmem[0]), RING_BUF_SIZE, &(pDesc->rbmem)};
	if (ring_buffer_init(&(pDesc->ringBufDesc), &attr) != E4E_OK) return E4E_ERROR;

	if (pDesc == 0) return E4E_ERROR;
	switch(device) {
	case E4E_HAL_SerialDevice_Command:
		// initialize the port for serial device
		pDesc->uartHandle = &hlpuart1;
		pmap_command->uartHandle = &hlpuart1;
		pmap_command->e4eSerialDesc = pDesc;
		break;
	case E4E_HAL_SerialDevice_Debug:
		// initialize the debug port
		pDesc->uartHandle = &huart1;
		pmap_debug->uartHandle = &huart1;
		pmap_debug->e4eSerialDesc = pDesc;
		break;
	default: return E4E_ERROR;
	};

	pDesc->readPtr = pDesc->tempRxBuf;
	pDesc->writePtr = pDesc->tempRxBuf;
	// if transmission uses interrupt or DMA mode, start collecting data right away
	return (HAL_OK == HAL_UART_Receive_DMA(pDesc->uartHandle, pDesc->tempRxBuf, RX_BUF_SIZE)) ? E4E_OK : E4E_ERROR;
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
	if (pDesc == 0) return E4E_ERROR;
	if (pDesc->uartHandle == 0) return E4E_ERROR;

    return ring_buffer_get_multiple(pDesc->ringBufDesc, pBuffer, count);
}

int E4E_HAL_Serial_write(E4E_HAL_SerialDesc_t *pDesc, const void *pData,
		size_t nBytes, uint32_t timeout) {
	if (pDesc == 0) return E4E_ERROR;
	if (pDesc->uartHandle == 0) return E4E_ERROR;

	// TODO: verify if we need to make a separate ring buffer for TX
	return (HAL_OK == HAL_UART_Transmit_IT(pDesc->uartHandle, pData, nBytes)) ? E4E_OK: E4E_ERROR;
}

int E4E_HAL_Serial_flush(E4E_HAL_SerialDesc_t *pDesc) {
	if (pDesc == 0) return E4E_ERROR;
	if (pDesc->uartHandle == 0) return E4E_ERROR;

	memset(pDesc->rbmem, 0, RING_BUF_SIZE);

	return E4E_OK;
}

/*
 * Grab the last item(s) of the rx buffer and move them into the ring buffer, then start
 * receiving again
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	// how much to write to ring buffer? depends on how much data has been written in so far
	E4E_HAL_SerialDesc_t *serialDesc = get_desc_from_handle(huart);
	if (serialDesc == NULL) {
		// should theoretically ever get here
		return;
	}
	serialDesc->writePtr = serialDesc->tempRxBuf + RX_BUF_SIZE;

	ring_buffer_put_multiple(serialDesc->ringBufDesc, serialDesc->readPtr, serialDesc->writePtr - serialDesc->readPtr);

	// reset both pointers to the beginning
	serialDesc->readPtr = serialDesc->tempRxBuf;
	serialDesc->writePtr = serialDesc->tempRxBuf;
	// is this call necessary? or does DMA operate continuously on circular mode?
	HAL_UART_Receive_DMA(huart, huart->pRxBuffPtr, huart->RxXferSize);
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
	E4E_HAL_SerialDesc_t *serialDesc = get_desc_from_handle(huart);
	if (serialDesc == NULL) {
		// should theoretically never get here
		return;
	}
	// half complete, update writePtr to halfway point
	serialDesc->writePtr = serialDesc->tempRxBuf + (RX_BUF_SIZE >> 1);

	ring_buffer_put_multiple(serialDesc->ringBufDesc, serialDesc->readPtr, serialDesc->writePtr - serialDesc->readPtr);

	// update readPtr to writePtr position
	serialDesc->readPtr = serialDesc->writePtr;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {
	E4E_HAL_SerialDesc_t *serialDesc = get_desc_from_handle(huart);
	if (serialDesc == NULL) {
		// should theoretically never get here
		return;
	}
	// idle, increment writePtr by however many bytes have been written
	//TODO: verify that size is the correct number of bytes to be writing to the ring buffer
	serialDesc->writePtr = serialDesc->writePtr + size;
	ring_buffer_put_multiple(serialDesc->ringBufDesc, serialDesc->readPtr, serialDesc->writePtr - serialDesc->readPtr);

	// update readPtr to writePtr position
	serialDesc->readPtr = serialDesc->writePtr;
}

