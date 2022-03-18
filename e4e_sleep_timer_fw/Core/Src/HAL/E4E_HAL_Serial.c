/*
 * serial.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#include <string.h>
#include <E4E_HAL_Serial.h>
#include <e4e_common.h>

// NOTE: need to add a lookup table to match STM HAL descriptors to E4E HAL descriptors

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
		break;
	case E4E_HAL_SerialDevice_Debug:
		// initialize the debug port
		pDesc->uartHandle = &huart1;
		break;
	default: return E4E_ERROR;
	};

	// TODO: map uartHandle to pDesc with some structure

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
	return (HAL_OK == HAL_UART_Transmit_DMA(pDesc->uartHandle, pData, nBytes)) ? E4E_OK: E4E_ERROR;
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
	// TODO: get ring buffer descriptor and ring buffer data pointer from UART_HandleTypeDef -> E4E_HAL_SerialDesc_t map
	// write to the ring buffer somehow
	// how much to write to ring buffer? depends on how much data has been written in so far
	int SHOULD_BE_RING_BUFFER_DESCRIPTOR = 0;
	void *SHOULD_BE_RING_BUFFER_PTR;
	ring_buffer_put_multiple(SHOULD_BE_RING_BUFFER_DESCRIPTOR, SHOULD_BE_RING_BUFFER_PTR, RX_BUF_SIZE >> 1);

	// is this call necessary? or does DMA operate continuously on circular mode?
	HAL_UART_Receive_DMA(huart, huart->pRxBuffPtr, huart->RxXferSize);
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
	// TODO: get ring buffer descriptor and ring buffer data pointer from UART_HandleTypeDef -> E4E_HAL_SerialDesc_t map
	// update ring buffer
	int SHOULD_BE_RING_BUFFER_DESCRIPTOR = 0;
	void *SHOULD_BE_RING_BUFFER_PTR;
	ring_buffer_put_multiple(SHOULD_BE_RING_BUFFER_DESCRIPTOR, SHOULD_BE_RING_BUFFER_PTR, RX_BUF_SIZE >> 1);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {
	// TODO: get ring buffer descriptor and ring buffer data pointer from UART_HandleTypeDef -> E4E_HAL_SerialDesc_t map
	// idle event, update ring buffer with size number of characters
	int SHOULD_BE_RING_BUFFER_DESCRIPTOR = 0;
	void *SHOULD_BE_RING_BUFFER_PTR;
	// TODO: verify that size is the correct number of bytes to be writing to the ring buffer
	ring_buffer_put_multiple(SHOULD_BE_RING_BUFFER_DESCRIPTOR, SHOULD_BE_RING_BUFFER_PTR, size);
}

