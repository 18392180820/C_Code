#include "callback.h"

uchar max_compare(uchar a, uchar b)
{
	return MAX(a, b);
}

void callback_test_op(uchar a, uchar b, callback_fun fun)
{
	MPrint("a = %c", a);
	MPrint("b = %c", b);
	MPrint("result = %c", fun(a, b));
}

void callback_test(void)
{
	uchar A = 'A';
	uchar B = 'B';
	callback_test_op(A, B, max_compare);
}

int fibonaci(int i)
{
   if(i == 0)
   {
      return 0;
   }
   if(i == 1)
   {
      return 1;
   }
   return fibonaci(i-1) + fibonaci(i-2);
}

void fibonaci_test()
{
	uint num = 20;
	uint j   = 0;

	MPrint("斐波那契队列开始,NUM = %d", num); 	
	for(j = 0; j < num; j++)
	{
		printf("%d\t\n", fibonaci(j));
	}
	MPrint("斐波那契队列结束");
}
