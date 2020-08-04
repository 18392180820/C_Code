#include "log.h"

static const char* TAG = "main";

void log_put(void)
{
	LOG_I("###info");
	LOG_W("###warn");
	LOG_E("###error");
	//LOG_N("WOW");
}

void main(void)
{	
	LOG_I("info");
	LOG_W("warn");
	LOG_E("error");
	//LOG_N("WOW");
	//log_put();

	if(-1)
	{
		LOG_I("HHHHHH");
	}

	printf("%d\n", (1>2));
	printf("%d\n", (1==2));
	printf("%d\n", (1<2));
}



