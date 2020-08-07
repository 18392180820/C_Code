#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "print.h"

#define STRING "STRING"

typedef void * handle_t;

static handle_t hand;

//check handle address
void handle_address_check(handle_t* hand_in)
{
	if(hand_in)
	{
		printf("handler address = %p\n", hand_in);  
	}
	else
	{
		printf("handler address\n");
	}
}

//check handle null
void handle_null_check(handle_t hand_in)
{
	if(hand_in)
	{
		printf("handler != NULL\n");  
	}
	else
	{
		printf("handler = NULL\n");
	}
}

//字符数组（可引申为其他数组）和字符串不一样，char[]和char*
#define 

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

	handle_address_check(&hand);
	printf("%p\n",&hand);			//check again

    int a[5];
    printf("%p\n", a);			//数组名a表示“数组首个元素的地质”
    printf("%p\n", a+1);		//数组名a表示“数组首个元素的地质”，是int型，因此a+1理解为a+1*sizeof(int)
    printf("%p\n", &a);			//&a表示int a[5]这个数组的地址，理解为&a[0],地址位置等同于a
    printf("%p\n", &a+1);		//&a表示int a[5]这个数组的地址，&a是数组、被看成int(*)[5]，所以sizeof(a)是5，也就是5*sizeof(int)，a+5*sizeof(int)


   	char* stri = "hello";		
    printf("%p\n", stri);		//"hello"首地址
    printf("%p\n", stri+1);		//"hello"首地址+1*sizeof(char)
    printf("%p\n", &stri);		//指针的地址
    printf("%p\n", &stri+1);	//指针的地址+1*sizeof(char*)



	handle_null_check(hand);

}

