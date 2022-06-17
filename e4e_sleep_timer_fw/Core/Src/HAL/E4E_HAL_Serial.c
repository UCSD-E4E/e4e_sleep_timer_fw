/*
 * serial.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#include <string.h>
#include <E4E_HAL_Serial.h>
#include <e4e_common.h>

static E4E_UARTHandle_To_SerialDesc_t uart_handle_to_descriptor_table[NUM_PORTS];

/**
 * @brief Gets a E4E_Hal_SerialDesc_t given its corresponding UART_HandleTypeDef
 * @param huart The UART_HandleTypeDef as defined from the HAL
 * @return The corresponding E4E_UARTHandle_To_SerialDesc_t as set in E4E_Hal_Serial_init
 */
static E4E_HAL_SerialDesc_t * get_desc_from_handle(UART_HandleTypeDef *huart) {
	if (huart == &hlpuart1) {
		return uart_handle_to_descriptor_table[E4E_HAL_SerialDevice_Command].e4eSerialDesc;
	} else if (huart == &huart1) {
		return uart_handle_to_descriptor_table[E4E_HAL_SerialDevice_Debug].e4eSerialDesc;
	}
	return NULL;
}

/**
 * @internal This function should initialize the serial driver descriptor with
 * any buffers, interrupts, and flags.
 */
int E4E_HAL_Serial_init(E4E_HAL_SerialDesc_t *pDesc,
		E4E_HAL_SerialDevice_e device, E4E_HAL_SerialConfig_t *pConfig) {
	static uint8_t portNum = 0;

	// initialize ring buffer
	RBuf_Attr_t rb_attr;
	rb_attr.n_elem = RING_BUF_SIZE;
	rb_attr.buffer = &(pDesc->rbmem);

	if (E4E_OK != ring_buffer_init(&(pDesc->ringBufDesc), &rb_attr)) {
		return E4E_ERROR;
	}

	if (NULL == pDesc) {
		return E4E_ERROR;
	}

	E4E_UARTHandle_To_SerialDesc_t *tableEntry = &(uart_handle_to_descriptor_table[device]);
	portNum++;
	switch(device) {
	case E4E_HAL_SerialDevice_Command:
		// initialize the port for command device
		pDesc->uartHandle = &hlpuart1;
		tableEntry->uartHandle = &hlpuart1;
		tableEntry->e4eSerialDesc = pDesc;
		break;
	case E4E_HAL_SerialDevice_Debug:
		// initialize the debug port
		pDesc->uartHandle = &huart1;
		tableEntry->uartHandle = &huart1;
		tableEntry->e4eSerialDesc = pDesc;
		break;
	default:
		return E4E_ERROR;
	};

	pDesc->readStatus = E4E_Serial_Done;
	if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(pDesc->uartHandle, pDesc->rbmem, RING_BUF_SIZE)) {
		return E4E_ERROR;
	}
	return E4E_OK;
}

int E4E_HAL_Serial_deinit(E4E_HAL_SerialDesc_t *pDesc) {
	if (NULL == pDesc)  {
		return E4E_ERROR;
	}
	if (NULL == pDesc->uartHandle) {
		return E4E_ERROR;
	}
	HAL_UART_MspDeInit(pDesc->uartHandle);
	pDesc->uartHandle = NULL;
	return E4E_OK;
}

/**
 * @internal This function should either read `count` bytes from the buffer or
 * none at all.  It should never read less than `count` bytes.
 */
int E4E_HAL_Serial_read(E4E_HAL_SerialDesc_t *pDesc, void *pBuffer,
		size_t count, uint32_t timeout) {
	if (NULL == pDesc) {
		return E4E_ERROR;
	}
	if (NULL == pDesc->uartHandle) {
		return E4E_ERROR;
	}

	uint32_t start_time = HAL_GetTick();
	while (HAL_GetTick() - start_time < timeout) {
		// check if bytes have been received
		if(E4E_Serial_Done == pDesc->readStatus) {
			if (E4E_ERROR == ring_buffer_get(pDesc->ringBufDesc, pBuffer, count)) {
				pDesc->readStatus = E4E_Serial_Waiting;
			} else {
				return E4E_OK;
			}
		}
	}
	pDesc->readStatus = E4E_Serial_Done;
	return E4E_ERROR;
}

int E4E_HAL_Serial_write(E4E_HAL_SerialDesc_t *pDesc, const void *pData,
		size_t nBytes, uint32_t timeout) {
	if (NULL == pDesc) {
		return E4E_ERROR;
	}
	if (NULL == pDesc->uartHandle)  {
		return E4E_ERROR;
	}

	if (HAL_OK != HAL_UART_Transmit(pDesc->uartHandle, pData, nBytes, timeout)) {
		return E4E_ERROR;
	}
	return E4E_OK;
}

int E4E_HAL_Serial_flush(E4E_HAL_SerialDesc_t *pDesc) {
	if (NULL == pDesc) {
		return E4E_ERROR;
	}
	if (NULL == pDesc->uartHandle) {
		return E4E_ERROR;
	}

	if (E4E_OK != ring_buffer_clear(pDesc->ringBufDesc)) {
		return E4E_ERROR;
	}

	return E4E_OK;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {
	E4E_HAL_SerialDesc_t *serialDesc = get_desc_from_handle(huart);

	if (NULL == serialDesc) {
		// should theoretically never get here
		return;
	}

	ring_buffer_handler(serialDesc->ringBufDesc, size);

	serialDesc->readStatus = E4E_Serial_Done;
}

