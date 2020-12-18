#include "log.h"
#include "my_math.h"

static const char* TAG = "main";

void LOGput(void)
{
	LOGI("###info");
	LOGW("###warn");
	LOGE("###error");
	//LOGN("WOW");
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
#if 0
	find_mode_test();
#else
	LOGI("sizeof = %ld", sizeof(uint32_t));
	LOGI("sizeof = %ld", sizeof(uint16_t));
	LOGI("sizeof = %ld", sizeof(uint8_t));
	LOGI("sizeof = %ld", sizeof(int32_t));
	LOGI("sizeof = %ld", sizeof(int16_t));
	LOGI("sizeof = %ld", sizeof(int8_t));

	int16_t dd[20];
	
	LOGI("### sizeof = %ld", sizeof(dd));
	
	LOGI("info");
	LOGW("warn");
	LOGE("error");


	if(-1)
	{
		LOGI("HHHHHH");
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



