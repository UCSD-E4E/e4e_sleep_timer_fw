/*
 * serial.h
 *
 *  Created on: Feb 9, 2022
 *      Author: ntlhui
 */

#ifndef INC_HAL_E4E_HAL_SERIAL_H_
#define INC_HAL_E4E_HAL_SERIAL_H_

#include <stddef.h>
#include <stdint.h>
#include <usart.h>
#include <E4E_Ring_Buffer.h>

#define RING_BUF_SIZE 128
#define RX_BUF_SIZE 128


/**
 * Serial Configuration Structure
 */
typedef struct E4E_HAL_SerialConfig_ {
	int dummy;
} E4E_HAL_SerialConfig_t;

/**
 * Serial Driver Descriptor
 */
typedef struct E4E_HAL_SerialDesc_ {
	/**
	 * Underlying HAL Descriptor
	 */
	UART_HandleTypeDef *uartHandle;

	/**
	 * Ring buffer descriptor
	 */
	RBuf_Desc_t ringBufDesc;

	/**
	 * Ring buffer memory
	 */
	uint16_t rbmem[RING_BUF_SIZE];

	/**
	 * Intermediate buffer for interrupts
	 */
	uint16_t tempRxBuf[RX_BUF_SIZE];

	//function pointer to
} E4E_HAL_SerialDesc_t;


/**
 * Serial Device Enumerators
 */
typedef enum E4E_HAL_SerialDevice_ {
	E4E_HAL_SerialDevice_Command,/**< Command Port - LPUART1 */
	E4E_HAL_SerialDevice_Debug,  /**< Debug Port - USART1 */
	E4E_HAL_SerialDevice__NELEMS /**< Number of Serial Ports */
} E4E_HAL_SerialDevice_e;

// TODO: add a static structure that maps UART_HandleTypeDef to its corresponding E4E_HAL_SerialDesc

/**
 * @brief Initializes the Serial Driver
 *
 */
/**
 * @brief Initializes the Serial Driver
 * @param pDesc	Descriptor to initialize
 * @param device	Device to initialize and associate
 * @param pConfig	Configuration parameters
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_Serial_init(E4E_HAL_SerialDesc_t *pDesc,
		E4E_HAL_SerialDevice_e device, E4E_HAL_SerialConfig_t *pConfig);

/**
 * @brief Deinitializes the Serial Driver
 * @param pDesc	Descriptor to deinitialize
 * @return	E4E_OK if successful, otherwise E4E_ERROR
 */
int E4E_HAL_Serial_deinit(E4E_HAL_SerialDesc_t *pDesc);

/**
 * @brief Reads buffered data from the Serial Driver
 * @param pDesc	Serial Driver Descriptor to read from
 * @param pBuffer	Buffer to read into
 * @param count	Number of bytes to read
 * @param timeout	Milliseconds before erroring out
 * @return	E4E_OK if read `count` bytes before `timeout`, otherwise E4E_ERROR
 */
int E4E_HAL_Serial_read(E4E_HAL_SerialDesc_t *pDesc, void *pBuffer,
		size_t count, uint32_t timeout);
/**
 * @brief Writes the specified data to the Serial Driver
 * @param pDesc	Serial Driver Descriptor to write to
 * @param pData	Buffer of data to write
 * @param nBytes	Number of bytes to write
 * @param timeout	Milliseconds before erroring out
 * @return	E4E_OK if wrote `nBytes` before `timeout`, otherwise E4E_ERROR
 */
int E4E_HAL_Serial_write(E4E_HAL_SerialDesc_t *pDesc, const void *pData,
		size_t nBytes, uint32_t timeout);

/**
 * @brief Flushes the input and output buffers
 * @param pDesc	Serial Driver Descriptor to flush
 * @return	E4E_OK if succesful, otherwise E4E_ERROR.
 */
int E4E_HAL_Serial_flush(E4E_HAL_SerialDesc_t *pDesc);
#endif /* INC_HAL_E4E_HAL_SERIAL_H_ */
