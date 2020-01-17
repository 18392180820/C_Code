#include "ringqueue.h"

/*如何判断环形队列为空，为满有两种判断方法。
	一.是附加一个标志位tag
	当head赶上tail，队列空，则令tag=0,
	当tail赶上head，队列满，则令tag=1,
	
	二.限制tail赶上head，即队尾结点与队首结点之间至少留有一个元素的空间。
	队列空：   head==tail
	队列满：   (tail+1)% MAXN ==head

	现使用第一种方式

	本文的变量表识
	uint  	rear;       //存数据的尾部，存取数据时候，+len
	uint	front;		//读数据的头部，读取数据时候，+len
*/


static ring_queue ring;

ring_queue* get_ring(void)
{
	return &ring;
}

void init_ring_queue(ring_queue* ring, uint buffer_size)	//初始化队列，注意初始化的清零
{
	ring->front  = ring->rear = 0;
	ring->length = 0;				//队列已存数据长度
	ring->size   = buffer_size;		//队列总长度

	ring->buffer = (uchar*)malloc(sizeof(uchar)*buffer_size);

	if(NULL == ring->buffer)
	{
		MPrint("Malloc_RingBuffer_Failure");
	}
	
	memset(ring->buffer, 0x00, (sizeof(uchar)*buffer_size));	
}

void check_ring_queue_state(ring_queue* ring)			//需不需要加入一个状态标志位，标志环形队列状态
{
	
}

void destroy_ring_queue(ring_queue* ring)				//销毁队列
{
	ring->front  = ring->rear = 0;
	ring->length = 0;
	ring->size   = 0;

	if(NULL == ring->buffer)
	{
		free(ring->buffer);
		ring->buffer = NULL;
	}	
}

uint get_used_queue_ring_size(ring_queue* ring)			//获取已使用环形队列长度
{	
	return ring->length;	
}

uint get_free_queue_ring_size(ring_queue* ring)			//获取剩余环形队列长度
{	
	return (ring->size - ring->length); 
}

uint write_queue_ring_op(ring_queue* ring, uchar* buf, uint len)		//写入队列数据
{
	if(NULL == ring) 
	{
		MPrint("Ring_Error");
		return RING_WRITE_RING_ERROR;
	}

	if((NULL == buf) || (0 == len))
	{
		MPrint("Buffer_Error");
		return RING_WRITE_BUFFER_ERROR;
	}

	if(ring->size == ring->length)
	{
		MPrint("Ring_Full");
		return RING_WRITE_RING_FULL;
	}
	
	/* 依据具体情况，选择写入最大长度、还是不写入，目前是不写入 */
	if(len > (ring->size - ring->length))			
	{	
		MPrint("Write_Len_OverFlow");
		return RING_WRITE_WRITE_OVERFLOW;
	}

	uint p_len  = ring->size - ring->rear;

	if(p_len > 0)
	{
		if(p_len >= len)
		{
			memcpy(&ring->buffer[ring->rear], buf, len);
		}
		else
		{
			memcpy(&ring->buffer[ring->rear], &buf[0], p_len);

			memcpy(&ring->buffer[0], &buf[p_len], (len-p_len));
		}
	}
	else
	{
		memcpy(&ring->buffer[0], &buf[0], len);
	}

	
	ring->rear   = (ring->rear + len)%ring->size;
	ring->length = ring->length + len;


	MPrint("Queue Buffer = %s", ring->buffer);
	MPrint("Queue Length = %d", ring->length);
	
	return len;
}

uint read_queue_ring_op(ring_queue* ring, uchar* buf, uint len)		//读出队列数据
{
	if(NULL == ring) 
	{
		MPrint("Ring_Error");
		return RING_READ_RING_ERROR;
	}

	if((NULL == buf) || (0 == len))
	{
		MPrint("Buffer_Error");
		return RING_READ_BUFFER_ERROR;
	}
	
	if((0 == ring->length))
	{
		MPrint("Ring_Empty");
		return RING_READ_RING_EMPTY;
	}

	/* 依据具体情况，选择读取最大长度、还是不读取，目前是不读取 */
	if(len > ring->length)
	{
		MPrint("Read_Len_OverFlow");
		return RING_READ_READ_OVERFLOW;
	}

	uint p_len = ring->size - ring->front;
	
	if(p_len > 0)
	{
		if(p_len >= len)
		{
			memcpy(buf, &ring->buffer[ring->front], len);
		}
		else
		{
			memcpy(&buf[0], &ring->buffer[ring->front], p_len);

			memcpy(&buf[p_len], &ring->buffer[0], (len-p_len));
		}
	}
	else
	{
		memcpy(&buf[0], &ring->buffer[0], len);
	}
	
	ring->length = ring->length - len;
	ring->front  = (ring->front + len)%ring->size;
	
	MPrint("Read Buffer = %s", buf);
	MPrint("Ring Length = %d", ring->length);
}

uint read_all_queue_ring_op(ring_queue* ring)			//读出现有队列所有数据
{
	if(NULL == ring) 
	{
		MPrint("RING_ERROR");
		return RING_READ_RING_ERROR;
	}
		
	if((0 == ring->length))
	{
		MPrint("Ring_Is_Empty");
		return RING_READ_RING_EMPTY;
	}

	uint p_len	= ring->size - ring->front;
	
	uchar p_buffer[ring->size];
	memset(p_buffer, 0x00, ring->size);

	MPrint("Ring_Queue_Length = %d", ring->length);
	
	if(ring->rear > ring->front)
	{
		memcpy(p_buffer, &ring->buffer[ring->front], ring->length);
	}
	else
	{
		memcpy(&p_buffer[0], &ring->buffer[ring->front], p_len);
		memcpy(&p_buffer[p_len], &ring->buffer[0], (ring->length - p_len));
	}

	MPrint("Ring_All_Buffer = %s", p_buffer);
	
}

