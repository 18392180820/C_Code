#include "ringqueue.h"
#include "callback.h"

	const char * msg = "[{\
		\"clientId\":	\"Fotile_DEV_1144513846\",\
		\"ip\":	\"10.49.2.153\",\
		\"port\":	1883,\
		\"topics\":	[{\
				\"indentifier\":	\"service\",\
				\"isSubPub\":	1,\
				\"level\":	1,\
				\"name\":	\"service/{product_id}/{device_id}\"\
			}, {\
				\"indentifier\":	\"control\",\
				\"isSubPub\":	1,\
				\"level\":	1,\
				\"name\":	\"control/{product_id}/{device_id}\"\
			}, {\
				\"indentifier\":	\"sync\",\
				\"isSubPub\":	2,\
				\"level\":	1,\
				\"name\":	\"sync/{product_id}/{device_id}\"\
			}, {\
				\"indentifier\":	\"CustomEvent\",\
				\"isSubPub\":	2,\
				\"level\":	1,\
				\"name\":	\"CustomEvent/{device_id}\"\
			}],\
		\"vendorId\":	\"Fotile\"\
	}]";



/* base64 encode-decode part*/
const char* base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* for AES test */
void hex16(unsigned char * buf) 
{
	int i;
  	for(i=0; i<16; i++)
	{
   		printf(" %02X",buf[i]);
  	}
  		printf("\n");
}

/* for AES test */
void hexall(unsigned char * buf) 
{
	int i = 0;
	int length = strlen((char*)buf);
  	for(i=0; i<length; i++)
	{
   		printf(" %02X",buf[i]);
  	}
  		printf("\n\n");
}

/* for AES-CBC */
void setiv(unsigned char * buf) {
  int i;
  for(i=0; i<16; i++){
    buf[i] = 0x00;
  }
}

char * base64_encode(const unsigned char * bindata, char * base64, int binlength)
{
    int i, j;
    unsigned char current;

    for (i = 0, j = 0; i < binlength; i += 3)
    {
        current = (bindata[i] >> 2);
        current &= (unsigned char) 0x3F;
        base64[j++] = base64char[(int) current];

        current = ((unsigned char)(bindata[i] << 4)) & ((unsigned char) 0x30);

        if (i + 1 >= binlength)
        {
            base64[j++] = base64char[(int) current];
            base64[j++] = '=';
            base64[j++] = '=';
            break;
        }

        current |= ((unsigned char)(bindata[i + 1] >> 4)) & ((unsigned char) 0x0F);
        base64[j++] = base64char[(int) current];

        current = ((unsigned char)(bindata[i + 1] << 2)) & ((unsigned char) 0x3C);

        if (i + 2 >= binlength)
        {
            base64[j++] = base64char[(int) current];
            base64[j++] = '=';
            break;
        }

        current |= ((unsigned char)(bindata[i + 2] >> 6)) & ((unsigned char) 0x03);
        base64[j++] = base64char[(int) current];

        current = ((unsigned char) bindata[i + 2]) & ((unsigned char) 0x3F);
        base64[j++] = base64char[(int) current];
    }

    base64[j] = '\0';
    return base64;
}

void del_empty(char * base64)
{
	char * p = base64;
	int i = 0;
	int j = 0;
	while(base64[i] != '\0')
	{
		if (base64[i] != 0x20)
		{
			p[j++] = base64[i++];
		}
		else
		{
			++i;
		}
	}
}

int base64_decode(const char * base64, unsigned char * bindata)
{
    int i, j;
    unsigned char k;
    unsigned char temp[4];
	del_empty((char*)base64);

    for (i = 0, j = 0; base64[i] != '\0'; i += 4)
    {
        memset(temp, 0xFF, sizeof(temp));

        for (k = 0; k < 64; k++)
        {
            if (base64char[k] == base64[i])
            {
                temp[0] = k;
            }
        }

        for (k = 0; k < 64; k++)
        {
            if (base64char[k] == base64[i + 1])
            {
                temp[1] = k;
            }
        }

        for (k = 0; k < 64; k++)
        {
            if (base64char[k] == base64[i + 2])
            {
                temp[2] = k;
            }
        }

        for (k = 0; k < 64; k++)
        {
            if (base64char[k] == base64[i + 3])
            {
                temp[3] = k;
            }
        }

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2)) & 0xFC)) | ((unsigned char)((unsigned char)(temp[1] >> 4) & 0x03));

        if (base64[i + 2] == '=')
        {
            break;
        }

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4)) & 0xF0)) | ((unsigned char)((unsigned char)(temp[2] >> 2) & 0x0F));

        if (base64[i + 3] == '=')
        {
            break;
        }

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6)) & 0xF0)) | ((unsigned char)(temp[3] & 0x3F));
    }

    return j;
}



const char* STEAMER_CONFIG = "{\
  \"AttributeList\": [\
    {\
      \"name\": \"电源开关\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"PowerSwitchAll\",\
      \"command\": {\
        \"bit_flag_index\": 0,\
        \"offset\": 0,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 0,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"工作状态\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"WorkState\",\
      \"command\": {\
        \"bit_flag_index\": 1,\
        \"offset\": 1,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 1,\
        \"len\": 1\
      }\
    },\
    {\
      \"name\": \"照明灯\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"Light\",\
      \"command\": {\
        \"bit_flag_index\": 2,\
        \"offset\": 2,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 2,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"水箱推杆\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"PushRod\",\
      \"command\": {\
        \"bit_flag_index\": 3,\
        \"offset\": 3,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 3,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"工作模式\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"WorkMode\",\
      \"command\": {\
        \"bit_flag_index\": 4,\
        \"offset\": 4,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 4,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"菜谱ID\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"CookID\",\
      \"command\": {\
        \"bit_flag_index\": 5,\
        \"offset\": 5,\
        \"len\": 12\
      },\
      \"status\": {\
        \"offset\": 5,\
        \"len\": 12\
      }\
    },\
	{\
      \"name\": \"设定工作时间_时\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"SetWorkTimeH\",\
      \"command\": {\
        \"bit_flag_index\": 6,\
        \"offset\": 17,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 17,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"设定工作时间_分\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"SetWorkTimeM\",\
      \"command\": {\
        \"bit_flag_index\": 6,\
        \"offset\": 18,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 18,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"设定工作时间_秒\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"SetWorkTimeS\",\
      \"command\": {\
        \"bit_flag_index\": 6,\
        \"offset\": 19,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 19,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"设定预约时间_时\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"SetAppointmentTimeH\",\
      \"command\": {\
        \"bit_flag_index\": 7,\
        \"offset\": 20,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 20,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"设定预约时间_分\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"SetAppointmentTimeM\",\
      \"command\": {\
        \"bit_flag_index\": 7,\
        \"offset\": 21,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 21,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"设定预约时间_秒\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"SetAppointmentTimeS\",\
      \"command\": {\
        \"bit_flag_index\": 7,\
        \"offset\": 22,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 22,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"设定温度值1\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"SetTemperature1\",\
      \"command\": {\
        \"bit_flag_index\": 8,\
        \"offset\": 23,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 29,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"设定温度值2\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"SetTemperature2\",\
      \"command\": {\
        \"bit_flag_index\": 9,\
        \"offset\": 24,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 30,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"设定温度值3\",\
      \"accessMode\": \"READ_WRITE\",\
      \"attribute\": \"SetTemperature3\",\
      \"command\": {\
        \"bit_flag_index\": 10,\
        \"offset\": 25,\
        \"len\": 1\
      },\
      \"status\": {\
        \"offset\": 31,\
        \"len\": 1\
      }\
    },\
    {\
      \"name\": \"工作剩余时间_时\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"RemainingTimeH\",\
      \"status\": {\
        \"offset\": 23,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"工作剩余时间_分\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"RemainingTimeM\",\
      \"status\": {\
        \"offset\": 24,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"工作剩余时间_秒\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"RemainingTimeS\",\
      \"status\": {\
        \"offset\": 25,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"多段烹饪剩余时间_时\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"MultiCookRemainingTimeH\",\
      \"status\": {\
        \"offset\": 26,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"多段烹饪剩余时间_分\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"MultiCookRemainingTimeM\",\
      \"status\": {\
        \"offset\": 27,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"多段烹饪剩余时间_秒\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"MultiCookRemainingTimeS\",\
      \"status\": {\
        \"offset\": 28,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"加热盘功率\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"HeatPlatePower\",\
      \"status\": {\
        \"offset\": 32,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"二级加热功率\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"SecondHeatPower\",\
      \"status\": {\
        \"offset\": 33,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"温度传感器1\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"TemperatureSensor1\",\
      \"status\": {\
        \"offset\": 34,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"温度传感器2\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"TemperatureSensor2\",\
      \"status\": {\
        \"offset\": 35,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"温度传感器3\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"TemperatureSensor3\",\
      \"status\": {\
        \"offset\": 36,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"水位传感器1\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"WaterLevelSensor1\",\
      \"status\": {\
        \"offset\": 37,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"水位传感器2\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"WaterLevelSensor2\",\
      \"status\": {\
        \"offset\": 38,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"压力传感器\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"PressureSensor\",\
      \"status\": {\
        \"offset\": 39,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"加热膜\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"HeatFilm\",\
      \"status\": {\
        \"offset\": 40,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"贯流热风机\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"Fan\",\
      \"status\": {\
        \"offset\": 41,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"门电子锁\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"DoorLock\",\
      \"status\": {\
        \"offset\": 42,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"门控开关\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"DoorSwitch\",\
      \"status\": {\
        \"offset\": 43,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"食物位置\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"FoodPlace\",\
      \"status\": {\
        \"offset\": 44,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"蒸箱故障号\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"SteamerErrorNum\",\
      \"status\": {\
        \"offset\": 45,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"远程升级\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"OTA\",\
      \"status\": {\
        \"offset\": 46,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"菜谱录制状态位\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"RecipeRecordStat\",\
      \"status\": {\
        \"offset\": 47,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"菜谱播放当前步骤\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"RecipePlayStat\",\
      \"status\": {\
        \"offset\": 48,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"缺水提示\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"WaterShortage\",\
      \"status\": {\
        \"offset\": 49,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"水箱位置\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"TankPlace\",\
      \"status\": {\
        \"offset\": 50,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"电源板OTA标志位\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"BoardOTAFlag\",\
      \"status\": {\
        \"offset\": 51,\
        \"len\": 1\
      }\
    },\
	{\
      \"name\": \"远程控制\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"RemoteControl\",\
      \"status\": {\
        \"offset\": 52,\
        \"len\": 1\
      }\
    },\
    {\
      \"name\": \"录制状态\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"RecordState\",\
      \"status\": {\
        \"offset\": 53,\
        \"len\": 1\
      }\
    }\
  ],\
  \"version\": \"xxx\",\
  \"header\": {\
    \"command\": {\
      \"head\": {\
        \"offset\": 0,\
        \"len\": 2,\
        \"default\": 62709\
      },\
      \"len\": {\
        \"offset\": 2,\
        \"len\": 2\
      },\
      \"type\": {\
        \"offset\": 4,\
        \"len\": 2,\
        \"default\": 256\
      },\
      \"cmd\": {\
        \"offset\": 6,\
        \"len\": 1,\
        \"default\": 49\
      },\
      \"stat\": {\
        \"offset\": 7,\
        \"len\": 1,\
        \"default\": 1\
      },\
      \"flags\": {\
        \"offset\": 8,\
        \"len\": 2,\
        \"default\": 0\
      },\
      \"payload_bitflag\": {\
        \"offset\": 10,\
        \"len\": 2,\
        \"explain\": \"payload_bitflag 在数据上报给云的时候没有此项内容，将len定义成0即可忽略\"\
      },\
      \"payload_data\": {\
        \"offset\": 12,\
        \"len\": 26\
      },\
      \"crc\": {\
        \"offset\": 38,\
        \"prev\": \"payload_data\",\
        \"len\": 2,\
        \"explain\": \"定义好具体的crc算法\"\
      }\
    },\
    \"status\": {\
      \"head\": {\
        \"offset\": 0,\
        \"len\": 2,\
        \"default\": 62709\
      },\
      \"len\": {\
        \"offset\": 2,\
        \"len\": 2\
      },\
      \"type\": {\
        \"offset\": 4,\
        \"len\": 2,\
        \"default\": 1024\
      },\
      \"cmd\": {\
        \"offset\": 6,\
        \"len\": 1,\
        \"default\": 48\
      },\
      \"stat\": {\
        \"offset\": 7,\
        \"len\": 1,\
        \"default\": 2\
      },\
      \"flags\": {\
        \"offset\": 8,\
        \"len\": 2,\
        \"default\": 0\
      },\
      \"payload_bitflag\": {\
        \"offset\": 10,\
        \"len\": 0,\
        \"explain\": \"payload_bitflag 在数据上报给云的时候没有此项内容，将len定义成0即可忽略\"\
      },\
      \"payload_data\": {\
        \"offset\": 10,\
        \"len\": 64\
      },\
      \"crc\": {\
        \"offset\": 74,\
        \"len\": 2,\
        \"explain\": \"定义好具体的crc算法\"\
      }\
    }\
  }\
}";





const char* STEAMER_CONFIG2 = "{\
  \"AttributeList\": [\
	{\
      \"name\": \"远程控制\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"RemoteControl\",\
      \"status\": {\
        \"offset\": 52,\
        \"len\": 1\
      }\
    },\
    {\
      \"name\": \"录制状态\",\
      \"accessMode\": \"READ_ONLY\",\
      \"attribute\": \"RecordState\",\
      \"status\": {\
        \"offset\": 53,\
        \"len\": 1\
      }\
    }\
  ],\
  \"version\": \"xxx\",\
  \"header\": {\
    \"command\": {\
      \"head\": {\
        \"offset\": 0,\
        \"len\": 2,\
        \"default\": 62709\
      },\
      \"len\": {\
        \"offset\": 2,\
        \"len\": 2\
      },\
      \"type\": {\
        \"offset\": 4,\
        \"len\": 2,\
        \"default\": 256\
      },\
      \"cmd\": {\
        \"offset\": 6,\
        \"len\": 1,\
        \"default\": 49\
      },\
      \"stat\": {\
        \"offset\": 7,\
        \"len\": 1,\
        \"default\": 1\
      },\
      \"flags\": {\
        \"offset\": 8,\
        \"len\": 2,\
        \"default\": 0\
      },\
      \"payload_bitflag\": {\
        \"offset\": 10,\
        \"len\": 2,\
        \"explain\": \"payload_bitflag 在数据上报给云的时候没有此项内容，将len定义成0即可忽略\"\
      },\
      \"payload_data\": {\
        \"offset\": 12,\
        \"len\": 26\
      },\
      \"crc\": {\
        \"offset\": 38,\
        \"prev\": \"payload_data\",\
        \"len\": 2,\
        \"explain\": \"定义好具体的crc算法\"\
      }\
    },\
    \"status\": {\
      \"head\": {\
        \"offset\": 0,\
        \"len\": 2,\
        \"default\": 62709\
      },\
      \"len\": {\
        \"offset\": 2,\
        \"len\": 2\
      },\
      \"type\": {\
        \"offset\": 4,\
        \"len\": 2,\
        \"default\": 1024\
      },\
      \"cmd\": {\
        \"offset\": 6,\
        \"len\": 1,\
        \"default\": 48\
      },\
      \"stat\": {\
        \"offset\": 7,\
        \"len\": 1,\
        \"default\": 2\
      },\
      \"flags\": {\
        \"offset\": 8,\
        \"len\": 2,\
        \"default\": 0\
      },\
      \"payload_bitflag\": {\
        \"offset\": 10,\
        \"len\": 0,\
        \"explain\": \"payload_bitflag 在数据上报给云的时候没有此项内容，将len定义成0即可忽略\"\
      },\
      \"payload_data\": {\
        \"offset\": 10,\
        \"len\": 64\
      },\
      \"crc\": {\
        \"offset\": 74,\
        \"len\": 2,\
        \"explain\": \"定义好具体的crc算法\"\
      }\
    }\
  }\
}";

#define MQTT_TOPIC 	"mqtt_topic"

typedef struct _mqttTopic
{
	int  	subpub;     //1-sub, 2-pub
	int     qos;
	char    topic[60];
	
}mqttTopic;

typedef struct _mqttInfor
{
	int  httpconnect;
	char clientID[20];
	char brokerIP[20];
	int  brokerPort;
	int  keepalive;
	mqttTopic topic[3];
}mqttInfor;

typedef struct
{
	uint	i;
	uint 	j;
	uint 	k;
	uint	l;
}s_uint;

typedef struct
{
	s_uint	i;
	s_uint 	j;
}g_uint;

typedef struct
{
	uint a_1:1;
	uint a_2:2;
	uint a_3:3;
}L_TEST;

#define MESSAGE_CXT			("ABCDEFGHI")
#define MESSAGE_WRITE_LEN	(9)
#define MESSAGE_READ_LEN	(8)
 
uchar buffer[MAX_RING_SIZE];

void refresh_buffer(void)
{
	memset(buffer, 0x00, MAX_RING_SIZE);
}

uchar* get_buffer(void)
{
	return buffer;
}

mqttInfor g_mqttInfor;

const  g_uint g_common_chan[2] = {
	{
		{
			1,
			11,
			111,
			1111,
		},
		{
			2,
			22,
			222,
			2222,
		}
	},
	{
		{
			3,
			33,
			333,
			3333,
		},
		{
			4,
			44,
			444,
			4444,
		}
	},
};

void transfer(void)
{
	printf("数组第一个数字, NUM = %d\n", g_common_chan[0].i.i);		

	char dev_mac[18] = {0};

	sprintf(dev_mac, "%02X%02X%02X%02X%02X%02X", 'A', 'B', 'C', 'D', 'E', 'F');

	printf(dev_mac, "%02X%02X%02X%02X%02X%02X", 0x12, 0x34, 0x56, 0x78, 0x90, 0x12);

	printf("len = %ld\n", strlen(dev_mac));
}

void main(void)
{
	
	int i = 0;
	int j = 0;

	L_TEST test = {1,2,3};

	MPrint("L_TEST A_1 = %d", test.a_1);
	MPrint("L_TEST A_2 = %d", test.a_2);
	MPrint("L_TEST A_3 = %d", test.a_3);
	
	init_ring_queue(get_ring(), MAX_RING_SIZE);

	MPrint("Max_Ring_Length  = %d", get_ring()->size);
	MPrint("Used_Ring_Length = %d", get_used_queue_ring_size(get_ring()));
	MPrint("Free_Ring_Length = %d", get_free_queue_ring_size(get_ring()));

	while(1)
	{
		write_queue_ring_op(get_ring(), MESSAGE_CXT, MESSAGE_WRITE_LEN);

		refresh_buffer();
			
		read_queue_ring_op(get_ring(), get_buffer(), MESSAGE_WRITE_LEN);

		read_all_queue_ring_op(get_ring());

		MPrint("Queue_Front = %d", get_ring()->front);
		MPrint("Queue_Rear  = %d", get_ring()->rear);
				
		printf("\n");
		
		sleep(1);		
	}
}

