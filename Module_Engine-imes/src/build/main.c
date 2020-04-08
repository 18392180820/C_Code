#include "module_engine.h"
#include "cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RANGHOOD_MODULE_FILE_PATH "../../config/ranghood_module.config"
#define DISINFECTION_MODULE_FILE_PATH "../../config/xdg_module.config"
#define STEAMER_MODULE_FILE_PATH "../../config/SCD39-C2.i_module.config"
#define IMES_MODULE_FILE_PATH "../../config/imes_module.config"


#define Trace() printf("%s<%d>\n", __FUNCTION__, __LINE__)

void print_hex(const unsigned char* hex, const int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		printf("%02x ", hex[i]);
	}
	printf("\n");
}

void cJSON_TEST()
{
	cJSON* arr = cJSON_CreateArray();
	cJSON* obj = cJSON_CreateObject();
	cJSON* sub = cJSON_CreateObject();
	cJSON* sub2 = cJSON_CreateObject();
	cJSON_AddNumberToObject(obj, "state", 123);

	cJSON_AddItemToObject(obj, "recipes", arr);

	cJSON_AddStringToObject(sub, "temp", "444");
	cJSON_AddStringToObject(sub, "temp2", "44444");
	cJSON_AddItemToArray(arr, sub);
	
	cJSON_AddStringToObject(sub2, "temp",  "444");
	cJSON_AddStringToObject(sub2, "temp2", "44444");
	cJSON_AddItemToArray(arr, sub2);
	
	
	printf("key %s \n", cJSON_Print(obj));
}

void IMES_Test()
{	

	int ret = load_config_file(IMES_MODULE_FILE_PATH);
	int i = 5;
	
	char req[256] = "{\"PowerSwitchAll\":2,\"Light\":1}";
	char hex[256] = {0};
	json_to_hex(req, hex);
	print_hex(hex, 30);
	printf("\n");

#if 0
	const char payload_status[256] = {0xf4, 0xf5, 0x30, 0x02, 0x02, 
						0x02, 0x02, 0x00, 0x33, 0x00, 14, 15, 14, 15, 
						0, 0, 0, 0, 0, 0, 0, 0, 
						0x98, 0, 0, 0x11, 0x12, 0x11, 0, 0, 0, 0, 
						0x02, 0, 0x11, 0x11, 0, 0, 0, 0, 0, 0};
#endif

	char payload_status[256] = {0xf4, 0xf5, 0x30, 0x02, 0x02, 
						0x02, 0x02, 0x00, 0x33, 0x00, 14, 15, 14, 15, 
						0, 0, 0, 0, 0, 0, 0, 0, 
						0x98, 0, 0, 0x11, 0x12, 0x11, 0, 0, 0, 0, 
						0x02, 0, 0x11, 0x11, 0, 0, 0, 0, 0, 0};

	char string[856];

	while(i--)
	{		
		memset(string, 0x00, sizeof(string));
		hex_to_json(payload_status, string);
														
		printf("###### Json Value = %s\n", cJSON_Print(cJSON_Parse(string)));

		payload_status[5]++;
		payload_status[22]++;

		sleep(1);
	}
}

//对单字节的取位函数，右移offset，取len的长度
char bit_test(char value, unsigned int offset, unsigned int len)
{
	if(8 < (offset + len))
	{
		printf("Error Parameter in\n");
		return -1;
	}
	
	//int vars   = 0xff>>(8-len);
	//int result = (value>>offset)&(vars);

	//printf("vars = %d\n", vars);
	//printf("result = %d\n\n", result);

	return (value>>offset)&(0xff>>(8-len));
}




int main()
{
	//cJSON_TEST();

	IMES_Test();
	//while(1)
	{
		//sleep(1);
	}
#if 0
	int i = 0xDD;
	int offset = 0;
	int len = 4;

	bit_test(i, 1, 3);
	bit_test(i, 3, 1);
	bit_test(i, 4, 3);	
	bit_test(i, 5, 2);

	printf("### %d\n", bit_test(i, 1, 3));
	printf("### %d\n", bit_test(i, 3, 1));
	printf("### %d\n", bit_test(i, 4, 3));
	printf("### %d\n", bit_test(i, 5, 2));	
#endif

}


