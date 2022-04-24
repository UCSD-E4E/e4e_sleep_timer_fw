/*
 * serial.c
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#include <string.h>
#include <E4E_HAL_Serial.h>
#include <e4e_common.h>
#include <Debug/debug_menu.h>

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

int testWrite(void) {
	// testing command device
	E4E_HAL_SerialDesc_t *pDesc = get_desc_from_handle(&hlpuart1);
	return E4E_HAL_Serial_write(pDesc, "Test message!\n\r", 14, 1000);
}

int testRead(void) {
	uint8_t testBuf[15];
	E4E_HAL_SerialDesc_t *pDesc = get_desc_from_handle(&hlpuart1);
	if (E4E_OK != E4E_HAL_Serial_read(pDesc, testBuf, 3, 1000)) {
		E4E_Println("Unable to retrieve character!");
		return E4E_ERROR;
	}
	else {
		E4E_Println(testBuf);
		return E4E_OK;
	}
}

/**
 * @internal This function should initialize the serial driver descriptor with
 * any buffers, interrupts, and flags.
 */
int E4E_HAL_Serial_init(E4E_HAL_SerialDesc_t *pDesc,
		E4E_HAL_SerialDevice_e device, E4E_HAL_SerialConfig_t *pConfig) {
	// initialize ring buffer
	RBuf_Attr_t attr = {sizeof(pDesc->rbmem[0]), RING_BUF_SIZE, &(pDesc->rbmem)};
	if (E4E_OK != ring_buffer_init(&(pDesc->ringBufDesc), &attr)) {
		return E4E_ERROR;
	}

	if (NULL == pDesc) {
		return E4E_ERROR;
	}
	switch(device) {
	case E4E_HAL_SerialDevice_Command:
		// initialize the port for command device
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
	default:
		return E4E_ERROR;
	};

	pDesc->readPos = 0;
	pDesc->readStatus = E4E_Serial_Done;
	pDesc->writeStatus = E4E_Serial_Done;
	if (HAL_OK != HAL_UARTEx_ReceiveToIdle_DMA(pDesc->uartHandle, pDesc->tempRxBuf, RX_BUF_SIZE)) {
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

	if (E4E_ERROR == ring_buffer_get_multiple(pDesc->ringBufDesc, pBuffer, count)) {
		pDesc->readStatus = E4E_Serial_Waiting;
		uint32_t abs_timeout = HAL_GetTick() + timeout;
		while (HAL_GetTick() < abs_timeout) {

			// check if bytes have been received
			if(pDesc->readStatus == E4E_Serial_Done) {
				if (E4E_ERROR == ring_buffer_get_multiple(pDesc->ringBufDesc, pBuffer, count)) {
					pDesc->readStatus = E4E_Serial_Waiting;
				} else {
					return E4E_OK;
				}
			}
		}
		pDesc->readStatus = E4E_Serial_Done;
		return E4E_ERROR;

	} else {
		return E4E_OK;
	}
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

	return ring_buffer_clear(pDesc->ringBufDesc);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {
	E4E_HAL_SerialDesc_t *serialDesc = get_desc_from_handle(huart);

	if (NULL == serialDesc) {
		// should theoretically never get here
		return;
	}
	/* Check if number of received data in recpetion buffer has changed */
	if (size != serialDesc->readPos){

	 /* Check if position of index in reception buffer has simply be increased
	       of if end of buffer has been reached */
	    if (size > serialDesc->readPos)
	    {
	      /* Current position is higher than previous one */
	      serialDesc->numReceivedChars = size - serialDesc->readPos;
	      /* Copy received data in "User" buffer for evacuation */
	      ring_buffer_put_multiple(serialDesc->ringBufDesc,serialDesc->tempRxBuf + serialDesc->readPos, serialDesc->numReceivedChars);
	    }
	    else
	    {
	      /* Current position is lower than previous one : end of buffer has been reached */
	      /* First copy data from current position till end of buffer */
	      serialDesc->numReceivedChars = RX_BUF_SIZE - serialDesc->readPos;
	      /* Copy received data in "User" buffer for evacuation */

	      ring_buffer_put_multiple(serialDesc->ringBufDesc,serialDesc->tempRxBuf + serialDesc->readPos, serialDesc->numReceivedChars);

	      /* Check and continue with beginning of buffer */
	      if (size > 0)
	      {
	    	ring_buffer_put_multiple(serialDesc->ringBufDesc,serialDesc->tempRxBuf, size);
	        serialDesc->numReceivedChars += size;
	      }
	    }

	}
	serialDesc->readPos = size;
	serialDesc->readStatus = E4E_Serial_Done;
}

