
#include "fotile_debug.h"

void main(void)
{	
	
	create_debug_adb_handler();
	create_debug_handler();
}


#if 0
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

void main(void)
{

	mine* p = &mine_type;

	CONSOLE_DBG_I("k = %s", p->k);
	CONSOLE_DBG_I("c = %s", p->l.c);
	CONSOLE_DBG_I("e = %s", p->l.d.e);

	//create_debug_adb_handler();
}
#endif
