/*
 * E4E_Ring_Buffer.h
 *
 *  Created on: Mar 2, 2022
 *      Author: kylehu
 *
 *  Implementation of a ring buffer based on this blog post: http://www.simplyembedded.org/tutorials/interrupt-free-ring-buffer/
 */

#ifndef INC_HAL_E4E_RING_BUFFER_H_
#define INC_HAL_E4E_RING_BUFFER_H_

#include <stdint.h>

/** Maximum number of ring buffer instances (should be one for each serial port) */
#define RING_BUFFER_MAX 2

/**
 * Ring buffer configuration structure
 */
typedef struct {
	size_t n_elem;
	void  *buffer;
} RBuf_Attr_t;

/**
 * Ring buffer descriptor (an index in a global array)
 */
typedef unsigned int RBuf_Desc_t;

/**
 *	Ring buffer struct
 */
struct ring_buffer {
	size_t n_elem;
	uint8_t *buf;
	volatile size_t head;
	volatile size_t tail;
};

/**
 * @brief Initialize a ring buffer
 * @param rbd Index of buffer in global array of ring buffers (output, modified by function)
 * @param attr Attributes of this ring buffer
 * @return E4E_OK if ring buffer successfully initialized, otherwise E4E_ERROR
 */
int ring_buffer_init(RBuf_Desc_t *rbd, RBuf_Attr_t *attr);

/**
 * @brief Get elements from the ring buffer
 * @param rbd Index of buffer in global array of ring buffers
 * @param data Pointer to location to store the data to
 * @param count Number of elements to get from the ring buffer
 * @return E4E_OK if `count` bytes successfully retrieved from ring buffer and written into `data`, else E4E_ERROR
 */
int ring_buffer_get(RBuf_Desc_t rbd, void *data, int count);

/**
 * @brief Put elements in the ring buffer by incrementing the head pointer
 * @param rbd Index of buffer in global array of ring buffers
 * @param count Number of elements to add to ring buffer
 * @return E4E_OK if `count` bytes added to ring buffer, else E4E_ERROR
 */
int ring_buffer_put(RBuf_Desc_t rbd, int count);

/**
 * @brief Handles updating the head pointer for the ring buffer during event callbacks
 * @param rbd Index of buffer in global array of ring buffers
 * @param new_local_head_pos The new location of the head pointer inside the ring buffer
 * @return E4E_OK if head pointer incremented successfully, else E4E_ERROR
 */
int ring_buffer_handler(RBuf_Desc_t rbd, int new_local_head_pos);

/**
 * @brief Clear all items from the ring buffer
 * @param rbd Index of buffer in global array of ring buffers
 * @return E4E_OK if ring buffer successfully cleared, else E4E_ERROR
 */
int ring_buffer_clear(RBuf_Desc_t rbd);

#endif /* INC_HAL_E4E_RING_BUFFER_H_ */
