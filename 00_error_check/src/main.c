#include "log.h"
#include "my_math.h"

static const char* TAG = "main";

void log_put(void)
{
	LOG_I("###info");
	LOG_W("###warn");
	LOG_E("###error");
	//LOG_N("WOW");
}


void show(int16_t* buf, uint16_t len)
{
	uint16_t i=0;
	for(i=0;i<len;i++)
	{
		printf("%d ", buf[i]);
	}
	printf("\n");
}

int main(void)
{	
#if 1
	find_mode_test();
#else
	LOG_I("sizeof = %ld", sizeof(uint32_t));
	LOG_I("sizeof = %ld", sizeof(uint16_t));
	LOG_I("sizeof = %ld", sizeof(uint8_t));
	LOG_I("sizeof = %ld", sizeof(int32_t));
	LOG_I("sizeof = %ld", sizeof(int16_t));
	LOG_I("sizeof = %ld", sizeof(int8_t));

	int16_t dd[20];
	SSS(dd);
	LOG_I("### sizeof = %ld", sizeof(dd));
	
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
#endif

	return CHECK_OK;
}



