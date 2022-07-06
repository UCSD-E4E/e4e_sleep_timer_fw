/*
 * serial.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#include <string.h>
#include <E4E_HAL_Serial.h>
#include <e4e_common.h>

static E4E_UARTHandle_To_SerialDesc_t uart_handle_to_descriptor_table[NUM_PORTS] =
{
		{&hlpuart1, NULL},
		{&huart1, NULL}
};

/**
 * @brief Gets a E4E_Hal_SerialDesc_t given its corresponding UART_HandleTypeDef
 * @param huart The UART_HandleTypeDef as defined from the HAL
 * @return The corresponding E4E_UARTHandle_To_SerialDesc_t as set in E4E_Hal_Serial_init
 */
static E4E_HAL_SerialDesc_t * get_desc_from_handle(UART_HandleTypeDef *huart) {
	for (int i = 0; i < NUM_PORTS; i++) {
		if (huart == uart_handle_to_descriptor_table[i].uartHandle) {
			return uart_handle_to_descriptor_table[i].e4eSerialDesc;
		}
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
	RBuf_Attr_t rb_attr;
	rb_attr.n_elem = RING_BUF_SIZE;
	rb_attr.buffer = &(pDesc->rbmem);

	if (E4E_OK != ring_buffer_init(&(pDesc->ringBufDesc), &rb_attr)) {
		return E4E_ERROR;
	}

	if (NULL == pDesc) {
		return E4E_ERROR;
	}

	if (device >= NUM_PORTS) {
		return E4E_ERROR;
	} else {
		E4E_UARTHandle_To_SerialDesc_t *tableEntry = &(uart_handle_to_descriptor_table[device]);
		pDesc->uartHandle = tableEntry->uartHandle;
		tableEntry->e4eSerialDesc = pDesc;
	}

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
	uint32_t total_chars_read = 0;
	uint8_t * pCharBuffer = (uint8_t *) pBuffer;
	while (HAL_GetTick() - start_time < timeout) {
		// check if bytes have been received
		if(E4E_Serial_Done == pDesc->readStatus) {
			// we read any available bytes in the ring buffer, reading a max of count bytes
			size_t chars_to_get = get_ring_buffer_available_chars(pDesc->ringBufDesc);
			if (chars_to_get > count - total_chars_read) {
				chars_to_get = count - total_chars_read;
			}

			if (E4E_OK == ring_buffer_get(pDesc->ringBufDesc, pCharBuffer + total_chars_read, chars_to_get)) {
				// read is successful, add number of characters read to total
				total_chars_read += chars_to_get;
				if (total_chars_read < count) {
					pDesc->readStatus = E4E_Serial_Waiting;
				} else {
					return E4E_OK;
				}
			} else {
				pDesc->readStatus = E4E_Serial_Waiting;
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

	if (HAL_OK != HAL_UART_Transmit(pDesc->uartHandle, (void *) pData, nBytes, timeout)) {
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

