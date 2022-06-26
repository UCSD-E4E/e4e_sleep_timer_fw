/*
 * E4E_Ring_Buffer.c
 *
 *  Created on: Mar 2, 2022
 *      Author: kylehu
 */

#include <stddef.h>
#include <string.h>
#include <e4e_common.h>
#include <E4E_Ring_Buffer.h>

static int ring_buffer_full(struct ring_buffer *rb);
static int ring_buffer_empty(struct ring_buffer *rb);

static struct ring_buffer _rb[RING_BUFFER_MAX];

int ring_buffer_init(RBuf_Desc_t *rbd, RBuf_Attr_t *attr) {
	static int idx = 0;
	int status = E4E_ERROR;

	if ((idx < RING_BUFFER_MAX) && (rbd != NULL) && (attr != NULL)) {
		if (attr->buffer != NULL) {
			if (((attr->n_elem - 1) & attr->n_elem) == 0) {
				_rb[idx].head = 0;
				_rb[idx].tail = 0;
				_rb[idx].n_elem = attr->n_elem;
				_rb[idx].buf = attr->buffer;
				*rbd = idx++;
				status = E4E_OK;
			}
		}
	}
	return status;
}

int ring_buffer_get(RBuf_Desc_t rbd, void *data, int count) {
	// we only want to read characters if we have enough data
	if (_rb[rbd].head - _rb[rbd].tail < count) {
		return E4E_ERROR;
	}
	char* temp = (char*) data;
	if ((rbd < RING_BUFFER_MAX) && !(ring_buffer_empty(&_rb[rbd]))) {
		const size_t offset = _rb[rbd].tail & (_rb[rbd].n_elem - 1);
		// check that we are not reading off the end of the ring buffer
		if (count > _rb[rbd].n_elem - offset) {
			// reading from both end and beginning of ring buffer
			memcpy(temp, &(_rb[rbd].buf[offset]), _rb[rbd].n_elem - offset);
			memcpy(temp + (_rb[rbd].n_elem - offset), &(_rb[rbd].buf[0]), count - (_rb[rbd].n_elem - offset));
		}
		else {
			memcpy(temp, &(_rb[rbd].buf[offset]), count);
		}
		_rb[rbd].tail += count;;
	} else {
		 return E4E_ERROR;
	}

	return E4E_OK;
}

int ring_buffer_put(RBuf_Desc_t rbd, int count) {
	int status = E4E_OK;

	if ((rbd < RING_BUFFER_MAX) && !(ring_buffer_full(&_rb[rbd]))) {
		_rb[rbd].head += count;
	} else {
		status = E4E_ERROR;
	}
	return status;
}

int ring_buffer_handler(RBuf_Desc_t rbd, int new_local_head_pos) {
	int status = E4E_OK;
	const size_t curr_local_head_pos = _rb[rbd].head & (_rb[rbd].n_elem - 1);

	if (new_local_head_pos != curr_local_head_pos) {
		/* Check if position of index in reception buffer has simply increased
		   or if end of buffer has been reached */
		if (new_local_head_pos > curr_local_head_pos){
			/* Current position is higher than previous one */
			status = ring_buffer_put(rbd, new_local_head_pos - curr_local_head_pos);
		}
		else {
			/* Current position is lower than previous one : end of buffer has been reached */
			/* Update head pointer of the ring buffer to the end */
			status = ring_buffer_put(rbd, _rb[rbd].n_elem - curr_local_head_pos);

			/* Check and continue with beginning of buffer */
			if (new_local_head_pos > 0) {
				status = ring_buffer_put(rbd, new_local_head_pos);
			}
		}
	}

	return status;
}

int ring_buffer_clear(RBuf_Desc_t rbd) {
	_rb[rbd].head = 0;
	_rb[rbd].tail = 0;
	memset(_rb[rbd].buf, 0, _rb[rbd].n_elem);
	return E4E_OK;
}


static int ring_buffer_full(struct ring_buffer *rb) {
	return ((rb->head - rb->tail) == rb->n_elem) ? 1 : 0;
}

static int ring_buffer_empty(struct ring_buffer *rb) {
	return ((rb->head - rb->tail) == 0) ? 1 : 0;
}
