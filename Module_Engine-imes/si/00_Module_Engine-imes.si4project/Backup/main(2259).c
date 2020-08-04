#include "module_engine.h"
#include "cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RANGHOOD_MODULE_FILE_PATH "../../config/ranghood_module.config"
#define DISINFECTION_MODULE_FILE_PATH "../../config/xdg_module.config"
#define STEAMER_MODULE_FILE_PATH "../../config/SCD39-C2.i_module.config"
#define IMES_MODULE_FILE_PATH "../../config/imes_module-v0.0.5.config"


#define Trace() printf("%s<%d>\n", __FUNCTION__, __LINE__)

static const char* imes_wifi_schema_n = "{\
\"AttributeList\": [\
{\
\"attr\": \"PowerSwitchAll\",\
\"cmd\": {\
\"offset\": 0,\
\"len\": 1,\
\"bit_index\": 0\
},\
\"status\": {\
\"offset\": 2,\
\"len\": 1\
}\
},\
{\
\"attr\": \"WorkMode\",\
\"cmd\": {\
\"offset\": 1,\
\"len\": 1,\
\"bit_index\": 1\
},\
\"status\": {\
\"offset\": 3,\
\"len\": 1\
}\
},\
{\
\"attr\": \"Light\",\
\"cmd\": {\
\"offset\": 2,\
\"len\": 1,\
\"bit_index\": 2\
},\
\"status\": {\
\"offset\": 4,\
\"len\": 1\
}\
},\
{\
\"attr\": \"Delay\",\
\"cmd\": {\
\"offset\": 3,\
\"len\": 1,\
\"bit_index\": 3\
},\
\"status\": {\
\"offset\": 5,\
\"len\": 1\
}\
},\
{\
\"attr\": \"GearRank\",\
\"cmd\": {\
\"offset\": 4,\
\"len\": 1,\
\"bit_index\": 4\
},\
\"status\": {\
\"offset\": 6,\
\"len\": 1\
}\
},\
{\
\"attr\": \"AirQul\",\
\"status\": {\
\"offset\": 10,\
\"len\": 1,\
\"bit_offset\": 0,\
\"bit_len\": 4\
}\
},\
{\
\"attr\": \"AirButlerState\",\
\"status\": {\
\"offset\": 10,\
\"len\": 1,\
\"bit_offset\": 4,\
\"bit_len\": 4\
}\
},\
{\
\"attr\": \"GearSpeedH\",\
\"status\": {\
\"offset\": 11,\
\"len\": 1\
}\
},\
{\
\"attr\": \"GearSpeedL\",\
\"status\": {\
\"offset\": 12,\
\"len\": 1\
}\
},\
{\
\"attr\": \"GearAmH\",\
\"status\": {\
\"offset\": 13,\
\"len\": 1\
}\
},\
{\
\"attr\": \"GearAmL\",\
\"status\": {\
\"offset\": 14,\
\"len\": 1\
}\
},\
{\
\"attr\": \"RunTimeH\",\
\"status\": {\
\"offset\": 15,\
\"len\": 1\
}\
},\
{\
\"attr\": \"RunTimeL\",\
\"status\": {\
\"offset\": 16,\
\"len\": 1\
}\
},\
{\
\"attr\": \"AirVolumeH\",\
\"status\": {\
\"offset\": 17,\
\"len\": 1\
}\
},\
{\
\"attr\": \"AirVolumeL\",\
\"status\": {\
\"offset\": 18,\
\"len\": 1\
}\
},\
{\
\"attr\": \"AmRank\",\
\"status\": {\
\"offset\": 19,\
\"len\": 1\
}\
},\
{\
\"attr\": \"ErrorF1\",\
\"status\": {\
\"offset\": 22,\
\"len\": 1,\
\"bit_offset\": 0,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"ErrorF2\",\
\"status\": {\
\"offset\": 22,\
\"len\": 1,\
\"bit_offset\": 1,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"ErrorF5\",\
\"status\": {\
\"offset\": 22,\
\"len\": 1,\
\"bit_offset\": 4,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"ErrorF6\",\
\"status\": {\
\"offset\": 22,\
\"len\": 1,\
\"bit_offset\": 5,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"ErrorF8\",\
\"status\": {\
\"offset\": 22,\
\"len\": 1,\
\"bit_offset\": 7,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"ErrorF9\",\
\"status\": {\
\"offset\": 23,\
\"len\": 1,\
\"bit_offset\": 0,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"ErrorF10\",\
\"status\": {\
\"offset\": 23,\
\"len\": 1,\
\"bit_offset\": 1,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"ErrorF11\",\
\"status\": {\
\"offset\": 23,\
\"len\": 1,\
\"bit_offset\": 2,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"ErrorF12\",\
\"status\": {\
\"offset\": 23,\
\"len\": 1,\
\"bit_offset\": 3,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"ErrorF13\",\
\"status\": {\
\"offset\": 23,\
\"len\": 1,\
\"bit_offset\": 4,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"ErrorF14\",\
\"status\": {\
\"offset\": 23,\
\"len\": 1,\
\"bit_offset\": 5,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"GeneratorError\",\
\"status\": {\
\"offset\": 24,\
\"len\": 1,\
\"bit_offset\": 0,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"PusnRodError\",\
\"status\": {\
\"offset\": 24,\
\"len\": 1,\
\"bit_offset\": 1,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"GasSensorError\",\
\"status\": {\
\"offset\": 24,\
\"len\": 1,\
\"bit_offset\": 2,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"PurifierError\",\
\"status\": {\
\"offset\": 24,\
\"len\": 1,\
\"bit_offset\": 3,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"TempSensorError\",\
\"status\": {\
\"offset\": 24,\
\"len\": 1,\
\"bit_offset\": 6,\
\"bit_len\": 2\
}\
},\
{\
\"attr\": \"AcousticSensorError\",\
\"status\": {\
\"offset\": 25,\
\"len\": 1,\
\"bit_offset\": 0,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"GasSensorError\",\
\"status\": {\
\"offset\": 25,\
\"len\": 1,\
\"bit_offset\": 1,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"RSLinkError\",\
\"status\": {\
\"offset\": 25,\
\"len\": 1,\
\"bit_offset\": 2,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"FanHealthNotice\",\
\"status\": {\
\"offset\": 26,\
\"len\": 1,\
\"bit_offset\": 0,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"PurifierHealthNotice\",\
\"status\": {\
\"offset\": 26,\
\"len\": 1,\
\"bit_offset\": 1,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"VoiceBoxState\",\
\"status\": {\
\"offset\": 26,\
\"len\": 1,\
\"bit_offset\": 2,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"PublicFlueState\",\
\"status\": {\
\"offset\": 26,\
\"len\": 1,\
\"bit_offset\": 3,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"DenoiseModuleState\",\
\"status\": {\
\"offset\": 26,\
\"len\": 1,\
\"bit_offset\": 4,\
\"bit_len\": 1\
}\
},\
{\
\"attr\": \"GasSensorCleanNotice\",\
\"status\": {\
\"offset\": 26,\
\"len\": 1,\
\"bit_offset\": 7,\
\"bit_len\": 1\
}\
}\
],\
\"version\": \"1.0.0\",\
\"header\": {\
\"cmd\": {\
\"head\": {\
\"offset\": 0,\
\"len\": 2,\
\"default\": 62709\
},\
\"len\": {\
\"offset\": 2,\
\"len\": 1\
},\
\"cmd\": {\
\"offset\": 3,\
\"len\": 1,\
\"default\": 2\
},\
\"addr\": {\
\"offset\": 4,\
\"len\": 1,\
\"default\":200\
},\
\"bitflag\": {\
\"offset\": 5,\
\"len\": 1\
},\
\"data\": {\
\"offset\": 6,\
\"len\": 28\
},\
\"crc\": {\
\"offset\": 34,\
\"len\": 2\
}\
},\
\"status\": {\
\"payloadData\": {\
\"offset\": 5,\
\"len\": 29\
}\
}\
}\
}";

static const char* imes_wifi_schema = "{\
  \"AttributeList\": [\
    {\
      \"attr\": \"PowerSwitchAll\",\
      \"cmd\": {\
        \"offset\": 0,\
        \"len\": 1,\
        \"bit_index\": 0\
      },\
      \"status\": {\
        \"offset\": 2,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"WorkMode\",\
      \"cmd\": {\
        \"offset\": 1,\
        \"len\": 1,\
        \"bit_index\": 1\
      },\
      \"status\": {\
        \"offset\": 3,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"Light\",\
      \"cmd\": {\
        \"offset\": 2,\
        \"len\": 1,\
        \"bit_index\": 2\
      },\
      \"status\": {\
        \"offset\": 4,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"Delay\",\
      \"cmd\": {\
        \"offset\": 3,\
        \"len\": 1,\
        \"bit_index\": 3\
      },\
      \"status\": {\
        \"offset\": 5,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"GearRank\",\
      \"cmd\": {\
        \"offset\": 4,\
        \"len\": 1,\
        \"bit_index\": 4\
      },\
      \"status\": {\
        \"offset\": 6,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"AirQul\",\
      \"status\": {\
        \"offset\": 10,\
        \"len\": 1,\
		\"bit_offset\": 0,\
		\"bit_len\": 4\
	  }\
    },\
    {\
      \"attr\": \"AirButlerState\",\
      \"status\": {\
        \"offset\": 10,\
        \"len\": 1,\
		\"bit_offset\": 4,\
		\"bit_len\": 4\
	  }\
    },\
    {\
      \"attr\": \"GearSpeedH\",\
      \"status\": {\
        \"offset\": 11,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"GearSpeedL\",\
      \"status\": {\
        \"offset\": 12,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"GearAmH\",\
      \"status\": {\
        \"offset\": 13,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"GearAmL\",\
      \"status\": {\
        \"offset\": 14,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"RunTimeH\",\
      \"status\": {\
        \"offset\": 15,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"RunTimeL\",\
      \"status\": {\
        \"offset\": 16,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"AirVolumeH\",\
      \"status\": {\
        \"offset\": 17,\
        \"len\": 1\
      }\
    },\
	{\
      \"attr\": \"AirVolumeL\",\
      \"status\": {\
        \"offset\": 18,\
        \"len\": 1\
      }\
    },\
	{\
      \"attr\": \"AmRank\",\
      \"status\": {\
        \"offset\": 19,\
        \"len\": 1\
      }\
    },\
    {\
      \"attr\": \"ErrorF1\",\
      \"status\": {\
        \"offset\": 22,\
        \"len\": 1,\
		\"bit_offset\": 0,\
		\"bit_len\": 1\
	  }\
    },\
    {\
      \"attr\": \"ErrorF2\",\
      \"status\": {\
        \"offset\": 22,\
        \"len\": 1,\
		\"bit_offset\": 1,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"ErrorF5\",\
      \"status\": {\
        \"offset\": 22,\
        \"len\": 1,\
		\"bit_offset\": 4,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"ErrorF6\",\
      \"status\": {\
        \"offset\": 22,\
        \"len\": 1,\
		\"bit_offset\": 5,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"ErrorF8\",\
      \"status\": {\
        \"offset\": 22,\
        \"len\": 1,\
		\"bit_offset\": 7,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"ErrorF9\",\
      \"status\": {\
        \"offset\": 23,\
        \"len\": 1,\
		\"bit_offset\": 0,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"ErrorF10\",\
      \"status\": {\
        \"offset\": 23,\
        \"len\": 1,\
		\"bit_offset\": 1,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"ErrorF11\",\
      \"status\": {\
        \"offset\": 23,\
        \"len\": 1,\
		\"bit_offset\": 2,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"ErrorF12\",\
      \"status\": {\
        \"offset\": 23,\
        \"len\": 1,\
		\"bit_offset\": 3,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"ErrorF13\",\
      \"status\": {\
        \"offset\": 23,\
        \"len\": 1,\
		\"bit_offset\": 4,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"ErrorF14\",\
      \"status\": {\
        \"offset\": 23,\
        \"len\": 1,\
		\"bit_offset\": 5,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"GeneratorError\",\
      \"status\": {\
        \"offset\": 24,\
        \"len\": 1,\
		\"bit_offset\": 0,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"PusnRodError\",\
      \"status\": {\
        \"offset\": 24,\
        \"len\": 1,\
		\"bit_offset\": 1,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"GasSensorError\",\
      \"status\": {\
        \"offset\": 24,\
        \"len\": 1,\
		\"bit_offset\": 2,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"PurifierError\",\
      \"status\": {\
        \"offset\": 24,\
        \"len\": 1,\
		\"bit_offset\": 3,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"TempSensorError\",\
      \"status\": {\
        \"offset\": 24,\
        \"len\": 1,\
		\"bit_offset\": 6,\
		\"bit_len\": 2\
	  }\
    },\
	{\
      \"attr\": \"AcousticSensorError\",\
      \"status\": {\
        \"offset\": 25,\
        \"len\": 1,\
		\"bit_offset\": 0,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"GasSensorError\",\
      \"status\": {\
        \"offset\": 25,\
        \"len\": 1,\
		\"bit_offset\": 1,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"RSLinkError\",\
      \"status\": {\
        \"offset\": 25,\
        \"len\": 1,\
		\"bit_offset\": 2,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"FanHealthNotice\",\
      \"status\": {\
        \"offset\": 26,\
        \"len\": 1,\
		\"bit_offset\": 0,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"PurifierHealthNotice\",\
      \"status\": {\
        \"offset\": 26,\
        \"len\": 1,\
		\"bit_offset\": 1,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"VoiceBoxState\",\
      \"status\": {\
        \"offset\": 26,\
        \"len\": 1,\
		\"bit_offset\": 2,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"PublicFlueState\",\
      \"status\": {\
        \"offset\": 26,\
        \"len\": 1,\
		\"bit_offset\": 3,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"DenoiseModuleState\",\
      \"status\": {\
        \"offset\": 26,\
        \"len\": 1,\
		\"bit_offset\": 4,\
		\"bit_len\": 1\
	  }\
    },\
	{\
      \"attr\": \"GasSensorCleanNotice\",\
      \"status\": {\
        \"offset\": 26,\
        \"len\": 1,\
		\"bit_offset\": 7,\
		\"bit_len\": 1\
	  }\
    }\
  ],\
  \"version\": \"1.0.0\",\
  \"header\": {\
    \"cmd\": {\
      \"head\": {\
        \"offset\": 0,\
        \"len\": 2,\
        \"default\": 62709\
      },\
      \"len\": {\
        \"offset\": 2,\
        \"len\": 1\
      },\
      \"cmd\": {\
        \"offset\": 3,\
        \"len\": 1,\
        \"default\": 2\
      },\
      \"addr\": {\
        \"offset\": 4,\
        \"len\": 1,\
		\"default\":200\
      },\
	  \"bitflag\": {\
        \"offset\": 5,\
        \"len\": 1\
      },\
      \"data\": {\
        \"offset\": 6,\
        \"len\": 28\
      },\
	  \"crc\": {\
        \"offset\": 34,\
        \"len\": 2\
      }\
    },\
    \"status\": {\
      \"payloadData\": {\
        \"offset\": 5,\
        \"len\": 29\
      }\
    }\
  }\
}";

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
	int i = 1;
	
	char req[256] = "{\"PowerSwitchAll\":2,\"Light\":1}";
	char hex[256] = {0};
	int res = json_to_hex(req, hex);
	print_hex(hex, (hex[2]+3));
	printf("\n");
	printf("res = %d\n", res);

#if 1

	const char payload_status[256] = {0xf4, 0xf5, 0x21, 0x02, 0x08, 
						0x02, 0x02, 0x11, 0x11, 0x11, 14, 15, 14, 15, 
						0, 0x12, 0, 0, 0, 0, 0, 0, 
						0x98, 0, 0, 0x11, 0x12, 0x11, 0, 0, 0, 0, 
						0x02, 0, 0x11, 0x11, 0, 0, 0, 0, 0, 0};

/*
	char payload_status[256] = {0xf4, 0xf5, 0x30, 0x02, 0x02, 
						0x02, 0x02, 0x00, 0x33, 0x00, 14, 15, 14, 15, 
						0, 0, 0, 0, 0, 0, 0, 0, 
						0x98, 0, 0, 0x11, 0x12, 0x11, 0, 0, 0, 0, 
						0x02, 0, 0x11, 0x11, 0, 0, 0, 0, 0, 0};
*/
	
	char string[856];

	while(i--)
	{		
		memset(string, 0x00, sizeof(string));
		hex_to_json(payload_status, string);
														
		printf("###### Json Value = %s\n", cJSON_Print(cJSON_Parse(string)));

		//payload_status[5]++;
		//payload_status[22]++;

		//sleep(1);
	}
#endif
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

	//IMES_Test();
	//while(1)
	{
		//sleep(1);
	}

	//printf("strlen of schema   = %ld\n", strlen(imes_wifi_schema));
	//printf("strlen of schema_n = %ld\n", strlen(imes_wifi_schema_n));

	//unsigned char payload[38] = {0xF4, 0xF5, 0x21, 0x02, 0x08, 00, 00, 0x01, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 0x79, 0xB3};

	//int res = Crc16(payload, 0, 34);

	//printf("低8位 = %02X\n", (res>>0)&0xFF);
	//printf("高8位 = %02X\n", (res>>8)&0xFF);

	const char* url = "{\"PowerSwitchAll\":1,\"WorkMode\":0,\"Light\":0,\"Delay\":0,\"GearRank\":0,\"AirQul\":0,\"AirButlerState\":0,\"ErrF1\":0,\"ErrF2\":0,\"ErrF5\":0,\"ErrF6\":0,\"ErrF8\":0,\"ErrF9\":0,\"ErrF10\":0,\"ErrF11\":0,\"ErrF12\":0,\"ErrF13\":0,\"ErrF14\":0,\"GeneratorE";

	char str[128];
	memset(str, 0x00, sizeof(str));

	//sprintf(str, url);

	//memcpy(str, url, strlen(url));

	printf("strlen = %ld\n", strlen(url));

	
	printf("str = %s\n", url);
	
}


