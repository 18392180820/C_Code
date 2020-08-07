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
const char* RES_QUERY_VERSION = 
"{\
\"queryDevVersionInfo\":\
{\
\"productType\":\"%02X%02X\",\
\"hardwareVersion\":\"%s\",\
\"softwareVersion\":\"%s\",\
\"productId\":\"%s\",\
\"secretKey\":\"%s\",\
\"mac\":\"%s\"\
}\
}";

//阶乘_递归
int Factorial(int value)
{
	if(0 >= value)
	{
		return 0;
	}

	if(1 == value)
	{
		return 1;
	}

	return value*Factorial(value - 1);
}

//可变参数_阶乘
int Factorial_recur(char* fun_name, int num, ...)
{
	va_list num_p;

	int nNum = 0;

	va_start(num_p, num);
	printf("%s[%d]:%d\n", fun_name, num, Factorial(num));

	while(0 != (nNum = va_arg(num_p, int)))
	{
		printf("%s[%d]:%d\n", fun_name, nNum, Factorial(nNum));
	}

	va_end(num_p);

	return 1;
}

//斐波那契_递归:F(1)=1，F(2)=1, F(n)=F(n - 1)+F(n - 2)（n ≥ 3，n ∈ N*）
int Fibonacci(int value)
{
	if(0 >= value)
	{
		return 0;
	}

	if( (1 == value) || (2 == value))
	{
		return 1;
	}

	return Fibonacci(value-1)+Fibonacci(value-2);
}

//可变参数_斐波那契
int Fibonacci_recur(char* fun_name, int num, ...)
{
	va_list num_p;

	int nNum = 0;

	va_start(num_p, num);
	printf("%s[%d]:%d\n", fun_name, num, Fibonacci(num));

	while(0 != (nNum = va_arg(num_p, int)))
	{
		printf("%s[%d]:%d\n", fun_name, nNum, Fibonacci(nNum));
	}

	va_end(num_p);

	return 1;
}

//可变参数_加
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

//可变参数_输出字符串
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

//指针的部分测试，数组与字符串指针的差别
void pointer_test(void)
{
	int value[8] = {9,1,2,3,4,5,6,7};
	int *p = value;
	printf("###%d\n", *(++p));
	printf("###%d\n", *(++p));
	printf("###%d\n", *value);
	//printf("###%d\n", *(++value)); -> value是数组名,类型为int[],不能进行赋值操作,所以也不能++操作

	char str[200] = "asdfghjkl";
	printf("###%s\n", str);
	//printf("###%s\n", (++str)); -> str是数组名,类型为char[],不能进行赋值操作,所以也不能++操作

	char* exam_str = "asdfghjklqwezxc";
	printf("###%c\n", *(++exam_str));
	printf("###%c\n", *(++exam_str));
	printf("###%s\n", (++exam_str));
	printf("###%c\n", *(--exam_str));
	printf("###%c\n", *(--exam_str));
	printf("###%c\n", *(--exam_str));
	printf("###%c\n", *(--exam_str));
	printf("###%c\n", *(--exam_str));
	printf("###%c\n", *(--exam_str));
	printf("###%c\n", *(--exam_str));
	return;
}

//手动字符串倒序
void test_(void)
{
	char* str = "asdfg";

	if(*(++str) != '\0')
	{
		if(*(++str) != '\0')
		{
			if(*(++str) != '\0')
			{
				if(*(++str) != '\0')
				{
					if(*(++str) != '\0')
					{	
						if(*(++str) != '\0')
						{
			
						}	
						printf("%c", *(str--));
					}
					printf("%c", *(str--));			
				}
				printf("%c", *(str--));
			}
			printf("%c", *(str--));
		}
		printf("%c", *(str--));
	}
	printf("%c%c\n", *(str),*(str-1));	
}

//字符串倒序输出_递归:递归最简单的理解方式就是去简单理解，比如到达某种状态的时候、该怎样做
void reverse(char* str)
{
	if('\0' != *(++str))	//next char
	{
		reverse(str);
	}
	printf("%c", *(str-1));
	return;
}

//可变参数_字符串倒序号
//%.*s用来指定宽度，对应一个整数
//.（点）与后面的数合起来,是指定必须输出这个宽度，如果所输出的字符串长度大于这个数，则按此宽度输出，如果小于，则输出实际长度
void reverse_recur(char* fun_name, char* str, ...)
{
	va_list P_str;

	char* nStr = NULL;

	printf("%s[%s]:", fun_name, str);
	reverse(str);
	printf("\n");

	va_start(P_str, str);

	do
	{
		nStr = va_arg(P_str, char*);
		printf("%s[%s]:", fun_name, nStr);
		reverse(nStr);
		printf("\n");
	}while(strcmp(nStr,"end"));

	va_end(P_str);

	return;
}

#define MINE_STR "there"

typedef struct
{
	char  str_a[20];
	char* str_c;
}his;

typedef struct
{
	char  str_a[20];
	char* str_c;
	his	  ss;
}mine;

mine mine_type = {"hello", MINE_STR, {"hello", MINE_STR}}; 

void main(void)
{
	printf("sum_Value = %d\n", add(1,2,3,4,5,6,7,0));
	putout("1222","2","3","4","5","end");
	Factorial_recur("阶乘",0,-1,1,2,3,4,5,6,7,8,9,0);
	Fibonacci_recur("斐波那契",0,-1,1,2,3,4,5,6,7,8,9,0);
	//pointer_test();
	//test_();
	reverse_recur("revrese","hello","thanks","end");

	//mine* p = &mine_type;

	//printf("### %s\n", p->ss.str_a);
	//printf("### %s\n", p->ss.str_c);

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

    printf("##\n%s\n", RES_QUERY_VERSION);

	return;
}


