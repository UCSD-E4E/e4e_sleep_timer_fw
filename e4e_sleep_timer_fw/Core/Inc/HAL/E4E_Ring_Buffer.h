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

#define RING_BUFFER_MAX 2

/*
 * Note: n_elem should be restricted to a power of 2 for computational efficiency
 */
typedef struct {
	size_t n_elem;
	void  *buffer;
} RBuf_Attr_t;

typedef unsigned int RBuf_Desc_t;

struct ring_buffer {
	size_t n_elem;
	uint8_t *buf;
	volatile size_t head;
	volatile size_t tail;
};

/*
 * Initialize a ring buffer
 * Arguments:
 * rbd - index of buffer in global array of ring buffers (output, modified by function)
 * attr - attributes of this ring buffer
 *
 * Returns: E4E_ERROR or E4E_OK
 */
int ring_buffer_init(RBuf_Desc_t *rbd, RBuf_Attr_t *attr);

/*
 * Get elements from the ring buffer
 * Arguments:
 * rbd - index of buffer in global array of ring buffers
 * data - pointer to location to store the data to
 * count - number of elements to get from the ring buffer
 */
int ring_buffer_get(RBuf_Desc_t rbd, void *data, int count);

/*
 * Put elements in the ring buffer
 * Arguments:
 * rbd - index of buffer in global array of ring buffers
 * data - pointer to location to pull data from
 * count - number of elements to add to ring buffer
 */
int ring_buffer_put(RBuf_Desc_t rbd, int count);

/*
 * Clear all items from the ring buffer
 * Arguments:
 * rbd - index of buffer in global array of ring buffers
 */
int ring_buffer_clear(RBuf_Desc_t rbd);

#endif /* INC_HAL_E4E_RING_BUFFER_H_ */
