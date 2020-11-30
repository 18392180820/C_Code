#include "log.h"

static const char* TAG = "main";

void log_put(void)
{
	LOG_I("###info");
	LOG_W("###warn");
	LOG_E("###error");
	//LOG_N("WOW");
}

int main(void)
{	
	LOG_I("info");
	LOG_W("warn");
	LOG_E("error");

	if(-1)
	{
		LOG_I("HHHHHH");
	}

	printf("%d\n", (1>2));
	printf("%d\n", (1==2));
	printf("%d\n", (1<2));

	printf("%s\n", __FILE__);
	
	ERROR_CHECK(1>2);
	ERROR_CHECK(2!=2);
	ERROR_CHECK(2==2);

	return CHECK_OK;
}



