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
		if ((attr->buffer != NULL) && (attr->s_elem > 0)) {
			if (((attr->n_elem - 1) & attr->n_elem) == 0) {
				_rb[idx].head = 0;
				_rb[idx].tail = 0;
				_rb[idx].s_elem = attr->s_elem;
				_rb[idx].n_elem = attr->n_elem;
				_rb[idx].buf = attr->buffer;
				*rbd = idx++;
				status = E4E_OK;
			}
		}
	}
	return status;
}

int ring_buffer_put(RBuf_Desc_t rbd, const void *data) {
	int status = E4E_OK;

	if ((rbd < RING_BUFFER_MAX) && !(ring_buffer_full(&_rb[rbd]))) {
		const size_t offset = (_rb[rbd].head & (_rb[rbd].n_elem - 1)) * _rb[rbd].s_elem;
		memcpy(&(_rb[rbd].buf[offset]), data, _rb[rbd].s_elem);
		_rb[rbd].head++;
	} else {
		status = E4E_ERROR;
	}
	return status;
}

int ring_buffer_get(RBuf_Desc_t rbd, void *data) {
	int status = E4E_OK;
	if(_rb[rbd].head == _rb[rbd].tail){
		return E4E_ERROR;
	}
	if ((rbd < RING_BUFFER_MAX) && !(ring_buffer_empty(&_rb[rbd]))) {
		const size_t offset = (_rb[rbd].tail & (_rb[rbd].n_elem - 1)) * _rb[rbd].s_elem;
		memcpy(data, &(_rb[rbd].buf[offset]), _rb[rbd].s_elem);
		_rb[rbd].tail++;
	} else {
		status = E4E_ERROR;
	}

	return status;
}

int ring_buffer_get_multiple(RBuf_Desc_t rbd, void *data, int count) {
	// we only want to read characters if we have enough data
	size_t temp = _rb[rbd].tail;
	if (_rb[rbd].head - _rb[rbd].tail < count) {
		return E4E_ERROR;
	}
	for (int i = 0; i < count; i++) {
		if (E4E_ERROR == ring_buffer_get(rbd, data+i)) {
			// move the pointer back to initial value if there is an error reading
			_rb[rbd].tail = temp;
			// need to do anything with ring buffer tail pointer?
			return E4E_ERROR;
		}
	}
	return E4E_OK;
}

int ring_buffer_put_multiple(RBuf_Desc_t rbd, const void *data, int count) {
	for (int i = 0; i < count; i++) {
		if (E4E_ERROR == ring_buffer_put(rbd, data+i)) {
			// need to do anything with ring buffer head pointer?
			return E4E_ERROR;
		}
	}
	return E4E_OK;
}


static int ring_buffer_full(struct ring_buffer *rb) {
	return ((rb->head - rb->tail) == rb->n_elem) ? 1 : 0;
}

static int ring_buffer_empty(struct ring_buffer *rb) {
	return ((rb->head - rb->tail) == 0) ? 1 : 0;
}
