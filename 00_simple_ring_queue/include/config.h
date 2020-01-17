#ifndef __CONFIG_H__
#define __CONFIG_H__


#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "print.h"
#include "cJSON.h"
#include <unistd.h>
#include <time.h>
#include <unistd.h>

#define MAX(a,b)(a>b?a:b)

#define MIN(a,b)(a<b?a:b)

#define ABS(a)(a>0?a:(-a))



#define MPrint(x, y...) do{	\
				printf("%s:%s<%d> " x, __FILENAME__S, __FUNCTION__, __LINE__, ##y);\
				printf("\n");\
				}while(0)


#endif

