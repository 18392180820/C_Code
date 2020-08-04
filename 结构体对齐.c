#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#pragma pack(n) 作用：C编译器将按照n个字节对齐。
//#pragma pack() 作用：取消自定义字节对齐方式。

/*	不使用gma pack (n)，三个结构体的size分别为24、24、16，需要保持是long的整数倍
	struct_1字节占用情况 4、4、8、8
	struct_2字节占用情况 8、8、4、4
	struct_3字节占用情况 2、2、4、8
*/

/*
	使用pragma pack(1)，三个结构头的size都为15
*/

//#pragma pack(1)
struct _struct_1
{
	char    d;
	int 	a;
	long 	b;
	short   c;
}struct_1;


struct _struct_2
{
	char    d;
	long 	a;
	int 	b;
	short   c;
}struct_2;

struct _struct_3
{
	char    d;
	short 	a;
	int 	b;
	long    c;
}struct_3;
//#pragma pack()

int main()
{
	printf("sizeof char  = %ld\n", sizeof(char));
	printf("sizeof short = %ld\n", sizeof(short));
	printf("sizeof int   = %ld\n", sizeof(int));
	printf("sizeof long  = %ld\n", sizeof(long));

	printf("sizeof struct_1   = %ld\n", sizeof(struct_1));
	printf("sizeof struct_2   = %ld\n", sizeof(struct_2));
	printf("sizeof struct_3   = %ld\n", sizeof(struct_3));
}



