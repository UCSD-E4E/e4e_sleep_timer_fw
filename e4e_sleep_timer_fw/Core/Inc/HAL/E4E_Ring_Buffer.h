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

#define RING_BUFFER_MAX 1

/*
 * Note: n_elem should be restricted to a power of 2 for computational efficiency
 */
typedef struct {
	size_t s_elem;
	size_t n_elem;
	void  *buffer;
} RBuf_Attr_t;

typedef unsigned int RBuf_Desc_t;

struct ring_buffer {
	size_t s_elem;
	size_t n_elem;
	uint8_t *buf;
	volatile size_t head;
	volatile size_t tail;
};

/*
 * Initialize a ring buffer
 * Arguments:
 * rbt - index of buffer in global array of ring buffers (output, modified by function)
 * attr - attributes of this ring buffer
 *
 * Returns: E4E_ERROR or E4E_OK
 */
int ring_buffer_init(RBuf_Desc_t *rbt, RBuf_Attr_t *attr);

/*
 * Add a single element to the ring buffer
 * Arguments:
 * rbt - index of buffer in global array of ring buffers
 * data - pointer to data to add to ring buffer
 */
int ring_buffer_put(RBuf_Desc_t rbt, const void *data);

/*
 * Get a single element from the ring buffer
 * Arguments:
 * rbt - inddex of buffer in global array of ring buffers
 * data - pointer to location to store element
 */
int ring_buffer_get(RBuf_Desc_t rbt, void *data);



#endif /* INC_HAL_E4E_RING_BUFFER_H_ */
