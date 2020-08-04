//结构体指针使用小例

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
}minee;

typedef struct
{
	char e[20];
	
}this_char;


typedef struct
{
	int a;
	int b;
	char c[20];
	this_char d;
}abc;


typedef struct
{
	int  j;
	char k[20];
	abc  l;
}mine;

mine mine_type = {1, "hello", {3, 4, "nihao",{"mygod"}}}; 
minee minee_type = {"hello", MINE_STR, {"hello", MINE_STR}}; 

void main(void)
{

	mine* p = &mine_type;
	minee* pe = &minee_type;

	printf("k = %s\n", p->k);
	printf("c = %s\n", p->l.c);
	printf("e = %s\n", p->l.d.e);

	printf("### %s\n", pe->ss.str_a);
	printf("### %s\n", pe->ss.str_c);

	return;
}


