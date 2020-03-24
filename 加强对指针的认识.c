#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "print.h"

#define STRING "STRING"

typedef void * handle_t;

static handle_t hand;

void main(void)
{
	char* data_buffer = (char*)malloc(1024);

	memset(data_buffer, 0, 1024);

	memcpy(data_buffer, STRING, sizeof(STRING));

	printf("sizeof(data_buffer)  = %ld %s\n", sizeof(data_buffer), data_buffer);
	printf("sizeof(*data_buffer) = %ld %ld\n", sizeof(*data_buffer), sizeof(data_buffer));
	printf("sizeof(*data_buffer) = %c  %c  %c\n", *data_buffer, *data_buffer+1, *(data_buffer+1));

	free(data_buffer);
	data_buffer = NULL;

	if(hand)
	{
		printf("hanle != NULL");
	}
	else
	{

		printf("hanle = NULL");   //this
	}

}
