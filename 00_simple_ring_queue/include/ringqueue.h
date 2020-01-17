#ifndef __RINGQUEUE_H__
#define __RINGQUEUE_H__

#include "config.h"

#define MAX_RING_SIZE	(30)//14

typedef enum
{
	RING_WRITE_FAILURE			=-5,
	RING_WRITE_RING_ERROR		=-4,
	RING_WRITE_BUFFER_ERROR		=-3,
	RING_WRITE_RING_FULL		=-2,
	RING_WRITE_WRITE_OVERFLOW	=-1,
	RING_WRITE_OK				= 0,
	
	RING_WRITE_ERROR_MAX
}ring_write_error;

typedef enum
{
	RING_READ_FAILURE			=-5,
	RING_READ_RING_ERROR		=-4,
	RING_READ_BUFFER_ERROR		=-3,
	RING_READ_RING_EMPTY		=-2,	
	RING_READ_READ_OVERFLOW		=-1,
	RING_READ_OK				= 0,
	
	RING_READ_ERROR_MAX
}ring_read_error;

typedef struct
{
	uchar* 	buffer;
	uint   	length;		//存取的buffer长度
	uint  	rear;       //存数据的尾部，存取数据时候，+len
	uint	front;		//读数据的头部，读取数据时候，+len
	uint    size;		//buffer总大小
}ring_queue;

ring_queue* get_ring(void);

void init_ring_queue(ring_queue* ring, uint buffer_size);

void check_ring_queue_state(ring_queue* ring);

void destroy_ring_queue(ring_queue* ring);

uint get_used_queue_ring_size(ring_queue* ring);

uint get_free_queue_ring_size(ring_queue* ring);

uint write_queue_ring_op(ring_queue* ring, uchar* buf, uint len);

uint read_queue_ring_op(ring_queue* ring, uchar* buf, uint len);

uint read_all_queue_ring_op(ring_queue* ring);


#endif


