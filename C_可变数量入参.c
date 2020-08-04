#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/******************************************************************************************************************
stdarg.h是C语言中C标准函数库的头文件，stdarg是由standard（标准） arguments（参数）简化而来，主要目的为让函数能够接收可变参数。
C++的cstdarg头文件中也提供这样的功能；虽然与C的头文件是兼容的，但是也有冲突存在。

stdarg.h 头文件定义了一个变量类型 va_list 和三个宏，这三个宏可用于在参数个数未知（即参数个数可变）时获取函数中的参数。
可变参数的函数通在参数列表的末尾是使用省略号(,...)定义的。

va_list：这是一个适用于 va_start()、va_arg() 和 va_end() 这三个宏存储信息的类型。

void va_start(va_list ap, last_arg)：这个宏初始化 ap 变量，它与 va_arg 和 va_end 宏是一起使用的。
last_arg 是最后一个传递给函数的已知的固定参数，即省略号之前的参数；
如果没有last_arg则编译失败，如下面函数add(int num, ...)，如果改为add(...)则编译失败。

type va_arg(va_list ap, type)：这个宏检索函数参数列表中类型为 type 的下一个参数。

void va_end(va_list ap)：这个宏允许使用了 va_start 宏的带有可变参数的函数返回。如果在从函数返回之前没有调用 va_end，则结果为未定义。
******************************************************************************************************************/

int add(int num, ...)
{
	va_list num_p;

	int sum = 0;
	int nNum = 0;

	sum += num;

	va_start(num_p, num);

	do{
		nNum = va_arg(num_p, int);
		sum += nNum;

	}while(nNum != 0);

	va_end(num_p);

	return sum;
}

void putout(char* str, ...)
{
	int count = 0;
	char* nStr = NULL;

	va_list P_str;

	va_start(P_str, str);

	printf("str[%d]:%s\n", count, str);

	do{
		
		nStr = va_arg(P_str, char*);
		printf("str[%d]:%s\n", ++count, nStr);

	}while(strcmp(nStr, "end"));

	va_end(P_str);

	return;
}

void main(void)
{
	printf("sum_Value = %d\n", add(1,2,3,4,5,6,7,0));
	putout("1222","2","3","4","5","end");
	return;
}
