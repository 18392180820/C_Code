#include "module_engine.h"
#include "cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#define __FILENAME__S (filename(__FILE__))

#if (defined __ANDROID__)
	#include <Android/Log.h>
	#define LOG_TAG "FIKS_HAL"
	#define MPrint(x, y...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, "%s:%s<%d>" x, __FILENAME__S, __FUNCTION__,__LINE__, ##y)
#elif (defined  __linux__)
	#define MPrint(x, y...) do{	\
		printf("%s:%s<%d> " x, __FILENAME__S, __FUNCTION__, __LINE__, ##y);\
		printf("\n");\
		}while(0)
#endif
#define CHECK_POINT(p) do{	\
	if (!p)	\
	{MPrint("Warn point is NULL");return FAILURE;}	\
	}while(0)

#define print_json(js) do{	\
	char* p = cJSON_Print(js);	\
	MPrint("p=%s", p);	\
	free(p);	\
	p = NULL;}while(0)

#define F_MALLOC(len) malloc(len)

#define F_FREE(p) do{	\
		free(p);	\
		p = NULL;	\
	}while(0)

	

static cJSON* s_config_file = NULL;

static cJSON* s_attribute_list = NULL;
static cJSON* s_header = NULL;
static cJSON* s_command = NULL;			//wifi向设备发送指令的cmd
static cJSON* s_status = NULL;			//设备向wifi上报状态至云端的status

static int s_command_len = 0;			//不包含电控命令行的header、len，包含cmd、addr、data、crc，用于json_to_hex
static int s_report_status_len = 0;		//状态位的上报payload长度

static char* s_report_status = NULL;	//只存储状态位，用于缓存状态位上报的hex，大小为payload长度，用以缓存status数据，作增量上报
static char s_report_flag = 0;

static unsigned short s_device_type = 0; 


int get_content_len(const cJSON* head, const char* content);
int get_content_offset(const cJSON* head, const char* content);
char char_to_value(const char c);
int input_string_data(OUT char* buf, const char* value, const int len);
int input_big_end_data(OUT char* buf, const unsigned int value, const int len);
int put_data_to_hex(OUT char* hex, const cJSON* head, const char* content, const int input_value);
int put_payload_to_hex(OUT char* hex, const cJSON* head, const cJSON* req);
char get_crc(const char* hex, const int len);
int get_string_data(const char* hex, const int offset, const int len, OUT char* value);
unsigned int get_big_end_data(const unsigned char* hex, const int offset, const int len);
int build_json(const char* payload, OUT cJSON* result);

int load_config_file(const char* file)
{
	if (!s_config_file)
	{
		s_config_file = cJSON_CreateObject();
	}
	
	FILE* pfile = fopen(file, "r");
	CHECK_POINT(pfile);
	
	fseek(pfile, 0, SEEK_END);
	int size = ftell(pfile);
	MPrint("file size[%d]", size);
	
	char * data = F_MALLOC(size+10);
	CHECK_POINT(data);
	memset(data, 0, size+10);
	
	fseek(pfile, 0, SEEK_SET);
	int result = fread(data, 1, size, pfile);
	fclose(pfile);

	s_config_file = cJSON_Parse(data);
	CHECK_POINT(s_config_file);

	s_attribute_list = cJSON_GetObjectItem(s_config_file, "AttributeList");
	CHECK_POINT(s_attribute_list);
	
	s_header = cJSON_GetObjectItem(s_config_file, "header");
	CHECK_POINT(s_header);
	s_command = cJSON_GetObjectItem(s_header, "cmd");
	CHECK_POINT(s_command);
	s_status = cJSON_GetObjectItem(s_header, "status");
	CHECK_POINT(s_status);

#if 0
	MPrint("AttributeList = %s", cJSON_Print(s_attribute_list));
	MPrint("header = %s", cJSON_Print(s_header));
	MPrint("cmd = %s", cJSON_Print(s_command));
	MPrint("status = %s", cJSON_Print(s_status));

	MPrint("s_config_file arrysize = %d", cJSON_GetArraySize(s_config_file));
	MPrint("AttributeList arrysize = %d", cJSON_GetArraySize(s_attribute_list));
	MPrint("header arrysize = %d", cJSON_GetArraySize(s_header));
	MPrint("cmd arrysize = %d", cJSON_GetArraySize(s_command));
	MPrint("status arrysize = %d", cJSON_GetArraySize(s_status));
#endif

	s_command_len = 5 + get_content_len(s_command, "data");
	s_report_status_len = get_content_len(s_status, "payloadData");
	s_report_status = F_MALLOC(s_report_status_len);

	//imes-wifi 版本的0x02里面没有type
	//cJSON* type = cJSON_GetObjectItem(s_command,"type");
	//s_device_type = cJSON_GetObjectItem(type, "default")->valueint;
	//MPrint("s_command_len[%d] s_report_status_len[%d]type[%u]", s_command_len, s_report_status_len, s_device_type);
	CHECK_POINT(s_report_status);
	memset(s_report_status, 0, s_report_status_len);


	F_FREE(data);
	
	MPrint("load success!");
	return SUCCESS;
}

int destroy()
{
	cJSON_Delete(s_config_file);
	F_FREE(s_report_status);
	s_report_status = NULL;
	return SUCCESS;
}

int get_content_len(const cJSON* head, const char* content)
{
	cJSON* sub = cJSON_GetObjectItem(head, content);
	CHECK_POINT(sub);
	cJSON* data = cJSON_GetObjectItem(sub, "len");
	CHECK_POINT(data);
	return data->valueint;
}

int get_content_offset(const cJSON* head, const char* content)
{
	cJSON* sub = cJSON_GetObjectItem(head, content);
	CHECK_POINT(sub);
	cJSON* data = cJSON_GetObjectItem(sub, "offset");
	CHECK_POINT(data);
	return data->valueint;
}

char char_to_value(const char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else if (c >= 'a' && c <= 'f')
	{
		return c + 10 - 'a';
	}
	else if (c >= 'A' && c <= 'F')
	{
		return c + 10 - 'A';
	}
	else 
	{
		//not hex
		return 0;
	}
}

int input_string_data(OUT char* buf, const char* value, const int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		buf[i] = (char_to_value(value[2*i]) << 4) + char_to_value(value[2*i + 1]);
	}
	return len;
}

int input_big_end_data(OUT char* buf, const unsigned int value, const int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		buf[i] = (value>>(8*(len-1-i)))&0xff;	
	}
	return len;
}

/**
* @ Description:	put head len type cmd... to hex
* @ param hex:		input buf
* @ param head:  	root name
* @ param content:	key name
* @ return: 		input len
*/
int put_data_to_hex(OUT char* hex, const cJSON* head, const char* content, const int input_value)
{
	cJSON* sub = cJSON_GetObjectItem(head, content);
	int offset = cJSON_GetObjectItem(sub, "offset")->valueint;
	int len = cJSON_GetObjectItem(sub, "len")->valueint;
	cJSON* def = cJSON_GetObjectItem(sub, "default");
	//if has default value
	if (def != NULL /*&& len <= 4*/)	//预留着 万一default超过4个字节需要额外处理
	{
			return input_big_end_data(hex+offset, def->valueint, len);
	}
	else	//no default value  use input value
	{
			return input_big_end_data(hex+offset, input_value, len);
	}
}

int put_payload_to_hex(OUT char* hex, const cJSON* head, const cJSON* req)
{
	int payload_bitflag_offset = get_content_offset(head, "bitflag");
	int payload_bitflag_len = get_content_len(head, "bitflag");
	int payload_data_offset = get_content_offset(head, "data");
	int payload_data_len = get_content_len(head, "data");

	const char* payload_type = "cmd";
	int array_size = cJSON_GetArraySize(s_attribute_list);
	int req_size = cJSON_GetArraySize(req);
	//MPrint("req_size [%d] req[%s]", req_size, cJSON_Print(req));
	int i = 0;
	int j = 0;
	int bit_flag = 0;	// support 4byte
	
	for(j = 0; j < req_size; j++)
	{
		cJSON* cmd_obj = cJSON_GetArrayItem(req, j);
		const char* k1 = cmd_obj->string;
		if (!k1)
		{		
			//MPrint("Warn req type is array!");
			//兼容 send_command的两种写法 数组和非数组
			cmd_obj = cJSON_GetArrayItem(cmd_obj, 0);
		}
		const char* key = cmd_obj->string;

		for(i = 0; i < array_size; i++)
		{
			cJSON* node = cJSON_GetArrayItem(s_attribute_list, i);    //node为每个电控板上报的状态量

			const char* attribute = cJSON_GetObjectItem(node, "attr")->valuestring;
	
			if(strcmp(key, attribute) == 0)
			{
				cJSON* payload_describe = cJSON_GetObjectItem(node, payload_type);
				if (payload_describe)	//没有这个字段说明不可写 不会转成uart数据
				{
					int bit_flag_index = cJSON_GetObjectItem(payload_describe, "bit_index")->valueint;
					int offset = cJSON_GetObjectItem(payload_describe, "offset")->valueint;
					int len = cJSON_GetObjectItem(payload_describe, "len")->valueint;
					if (len <= 4)
					{
						input_big_end_data(hex + payload_data_offset + offset, (unsigned int)cmd_obj->valuedouble, len);
					}
					else
					{
						input_string_data(hex + payload_data_offset + offset, cmd_obj->valuestring, len);
					}
					bit_flag |= (1<<bit_flag_index);
				}
			}
		}
	}
	input_big_end_data(hex + payload_bitflag_offset, bit_flag, payload_bitflag_len);
	return payload_bitflag_len + payload_data_len;
}



/********************************************************/
/********************************************************/
/*
统一采用CRC16-ModBus校验，校验内容为从包头起(0xF4),至CRC前一字节的所有数据。校验结果先发送低位后发送高位。
大小端：多于一个字节的数据以大端字节序编码；如0x1234，先发0x12，后发0x34
*/
/********************************************************/
unsigned int Crc16 (unsigned char *Dat, unsigned char Start_Byte,unsigned char Length) 
{
	unsigned char i = 0;
	unsigned int Crc = 0xFFFF;
	unsigned char *prt;
	prt = Dat+Start_Byte;
	while ( Length-- ) 
	{
			Crc ^= *prt++;
			for(i = 0;i<8;i++) 
			{
					if(Crc & 0x0001) 
					{	
							Crc = (Crc>>1) ^ 0xa001;
					}
					else
					{
							Crc = Crc>>1;
					}
			}
	}

	//MPrint("CRC = %04X", (Crc));
	//MPrint("CRC = %04X", (Crc>>8)&0xFF);
	//MPrint("CRC = %04X", (Crc>>0)&0xFF);
	
	return Crc ;
}
/********************************************************/
/********************************************************/
/********************************************************/


char get_crc(const char* hex, int len)
{
	int i = 0;
	char result = 0;
	for (i = 0; i < len; i++)
	{
		result ^= hex[i];
	}
	return result;
}

int get_string_data(const char* hex, const int offset, const int len, OUT char* value)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		//强转成无符号数据，否则字符串异常
		sprintf(value, "%s%02x", value, (unsigned char)hex[offset+i]);
	}
	return len;
}

unsigned int get_big_end_data(const unsigned char* hex, const int offset, const int len)
{
	int i = 0;
	unsigned int value = 0;
	for (i = 0; i < len; i++)
	{
		//两种写法一样的
		value += (hex[offset+len-i-1] << (i * 8));
		//value += (hex[i] << ((len-1-i)* 8)) & 0xff;
	}
	return value;
}


unsigned int get_bit_data(const unsigned char value, const unsigned int bitoffset, const unsigned int bitlen)
{

	if( 8 < (bitoffset + bitlen ))
	{
		MPrint("Error Parameter in");
		return 0xFF;
	}	

	return (value>>bitoffset)&(0xff>>(8-bitlen));
}

int build_json(const char* payload, OUT cJSON* result)
{
	int i = 0;
	int j = 0;
	int array_size = cJSON_GetArraySize(s_attribute_list);
	const char* payload_type = "status";
	for (i = 0; i < s_report_status_len; i++)
	{
		if ((payload[i] == s_report_status[i]) && s_report_flag)
		{
			continue;
		}
		else
		{
			for(j = 0; j < array_size; j++)
			{
				cJSON* node = cJSON_GetArrayItem(s_attribute_list, j);
				//access_mode 暂时用不到 通过 status的有无判断效果一样
				//const char* access_mode = cJSON_GetObjectItem(node, "accessMode")->valuestring;
				const char* attribute = cJSON_GetObjectItem(node, "attr")->valuestring;			
				cJSON* payload_describe = cJSON_GetObjectItem(node, payload_type);
				if (payload_describe)
				{
					int offset = cJSON_GetObjectItem(payload_describe, "offset")->valueint;
					int len = cJSON_GetObjectItem(payload_describe, "len")->valueint;
					if (i >= offset && i < offset+len)	//识别到属性数据有变化
					{
						if (len <= 4)
						{
							//识别到没有子属性
							if( (NULL == cJSON_GetObjectItem(payload_describe, "bit_offset")) || (NULL == cJSON_GetObjectItem(payload_describe, "bit_len")) )
							{
								unsigned int value = get_big_end_data((const unsigned char*)payload, offset, len);
								cJSON_AddNumberToObject(result, attribute, value);
							}
							else	//识别到有子属性
							{
								unsigned int bit_offset = cJSON_GetObjectItem(payload_describe, "bit_offset")->valueint;
								unsigned int bit_len    = cJSON_GetObjectItem(payload_describe, "bit_len")->valueint;
								unsigned int bit_value  = get_bit_data((const unsigned char)payload[i], bit_offset, bit_len);
								//先确认子属性是否有变化，如果没有变化则continue，有则组json
								if((get_bit_data(s_report_status[i], bit_offset, bit_len) == bit_value)&&(s_report_flag))
								{
									continue;
								}
								cJSON_AddNumberToObject(result, attribute, bit_value);
							}
						}
						else	//大于4字节的是字符串类型
						{
							char* value = F_MALLOC(len*2 + 1);
							if (value)
							{
								memset(value, 0, len*2+1);
								get_string_data(payload, offset, len, value);
								cJSON_AddStringToObject(result, attribute, value);
							}
							else
							{
								MPrint("malloc fail value = NULL");
							}
							F_FREE(value);
						}
						i = offset + len - 1;	//移到最后一字节 防止添加两次json
					}
					
					//input_big_end_data(hex + payload_data_offset + offset, value, len);
					//bit_flag |= (1<<bit_flag_index);
				}
			}
		}
	}
	return SUCCESS;
}

int build_start_record_json(const char* payload, OUT cJSON* result)
{
	//建对象
	cJSON * obj = cJSON_CreateObject();

	//填数据
	cJSON_AddNumberToObject(obj, "status", payload[0]);

	char value[32] = {0};
	get_string_data(payload, 1, 12, value);
	cJSON_AddStringToObject(obj, "recipeId", value);

	//添加到父节点
	cJSON_AddItemToObject(result, "startRecord", obj);
	return SUCCESS;
}

int build_stop_record_json(const char* payload, OUT cJSON* result, const char* host)
{
	//建对象
	cJSON * obj = cJSON_CreateObject();

	//填数据
	cJSON_AddNumberToObject(obj, "status", payload[0]);
	cJSON_AddNumberToObject(obj, "interruptNodes", payload[1]);

	char value[32] = {0};
	get_string_data(payload, 2, 12, value);
	cJSON_AddStringToObject(obj, "recipeId", value);

	//添加到父节点
	cJSON_AddItemToObject(result, host, obj);	// wifi发起   host="stopRecordWiFi" 灶具发起 host="stopRecordStove"
	return SUCCESS;
}

int build_query_recipes_json(const char* payload, OUT cJSON* result, const int data_len)
{
	//建对象
	cJSON * obj = cJSON_CreateObject();
	//cJSON* arr = cJSON_CreateArray();
	int recipes_len = data_len - 8 - 14;
	int size = recipes_len % 20;
	if (size != 0)
	{
		MPrint("warn no 20*n recipes_len[%d] size[%d]", recipes_len, size);
	}

	//填数据
	cJSON_AddNumberToObject(obj, "status", payload[0]);

	char value[32] = {0};
	get_string_data(payload, 1, 12, value);
	cJSON_AddStringToObject(obj, "recipeId", value);
	
	cJSON_AddNumberToObject(obj, "interruptNodes", payload[13]);

	char recipeData[2*recipes_len+1];
	memset(recipeData, 0, sizeof(recipeData));
	get_string_data(payload, 14, recipes_len, recipeData);
	cJSON_AddStringToObject(obj, "recipeData", recipeData);

	/*
	const char *p = payload+14;		//指向菜谱数据
	int i = 0;
	for (i = 0; i < size; i++)
	{
		cJSON* sub = cJSON_CreateObject();

		cJSON_AddNumberToObject(sub, "version", p[20*i + 0]);
		cJSON_AddNumberToObject(sub, "type", p[20*i + 1]);
		cJSON_AddNumberToObject(sub, "mode", p[20*i + 2]);
		cJSON_AddNumberToObject(sub, "totalTime", p[20*i + 3] + p[20*i + 4]);
		cJSON_AddNumberToObject(sub, "currentTime", p[20*i + 5]);
		cJSON_AddNumberToObject(sub, "TNum", p[20*i + 6] + p[20*i + 7]);
		cJSON_AddNumberToObject(sub, "targetT", p[20*i + 8] + p[20*i + 9]);
		cJSON_AddNumberToObject(sub, "currentT", p[20*i + 10] + p[20*i + 11]);

		cJSON_AddItemToArray(arr, sub);
	}

	cJSON_AddItemToObject(obj, "recipeData", arr);
	*/
	
	//添加到父节点
	cJSON_AddItemToObject(result, "queryRecipes", obj);
	return SUCCESS;
}

int build_start_cook_json(const char* payload, OUT cJSON* result)
{
	//建对象
	cJSON * obj = cJSON_CreateObject();

	//填数据
	cJSON_AddNumberToObject(obj, "status", payload[0]);

	char value[32] = {0};
	get_string_data(payload, 1, 12, value);
	cJSON_AddStringToObject(obj, "recipeId", value);

	//添加到父节点
	cJSON_AddItemToObject(result, "startCook", obj);
	return SUCCESS;
}

int build_terminate_cook_json(const char* payload, OUT cJSON* result)
{
	//建对象
	cJSON * obj = cJSON_CreateObject();

	//填数据
	cJSON_AddNumberToObject(obj, "status", payload[0]);

	char value[32] = {0};
	get_string_data(payload, 1, 12, value);
	cJSON_AddStringToObject(obj, "recipeId", value);

	//添加到父节点
	cJSON_AddItemToObject(result, "terminateCook", obj);
	return SUCCESS;
}


/* imes control order: PowerSwitchAll、Light、WorkMode、GearRank、AirVolume */
COMMON_API int json_to_hex(const char* json_data, OUT char* uart_hex)
{
	CHECK_POINT(s_config_file);
	cJSON *req = cJSON_Parse(json_data);
	CHECK_POINT(req);
	//固定的
	char* hex = F_MALLOC(s_command_len + 3); 	//4字节包含head和len, 完整的电控板命令行
	CHECK_POINT(hex);
	memset(hex, 0, s_command_len + 3);
	int pos = 0;
	pos += put_data_to_hex(hex, s_command, "head", 0);		//config 文件有默认值
	pos += put_data_to_hex(hex, s_command, "len", s_command_len);
	pos += put_data_to_hex(hex, s_command, "cmd", 0);
	pos += put_data_to_hex(hex, s_command, "addr", 0);

	//MPrint("req = %s", cJSON_Print(req));
	pos += put_payload_to_hex(hex, s_command, req);

	//unsigned char crc_res = get_crc(hex, pos);   //old version

	unsigned int crc_res = Crc16(hex, 0, pos);

	//MPrint("crc_res[%u]", crc_res);

	//pos += put_data_to_hex(hex, s_command, "crcl", (crc_res>>8)&0xFF);
	//pos += put_data_to_hex(hex, s_command, "crch", (crc_res>>0)&0xFF);
	pos += put_data_to_hex(hex, s_command, "crc", crc_res);
	memcpy(uart_hex, hex, pos);
	F_FREE(hex);
	cJSON_Delete(req);
	return pos;
}

COMMON_API int clear_report_flag()
{
	s_report_flag = 0;
	MPrint("set s_report_flag 0");
	return SUCCESS;
}

COMMON_API int hex_to_json(const char* uart_hex, OUT char* json_data)
{
	CHECK_POINT(s_config_file);
	CHECK_POINT(uart_hex);
	CHECK_POINT(json_data);

	cJSON* res = cJSON_CreateObject();
	char cmd = uart_hex[3]; 
	int ret = REPORT_TYPE_NUM;
	switch (cmd)
	{
		case 0x02:		//设备控制的状态上报
		{
			int payload_offset = get_content_offset(s_status, "payloadData");
			const char * payload = uart_hex + payload_offset;
			build_json(payload, res);
			memcpy(s_report_status, payload, s_report_status_len);
			if (!s_report_flag)
			{
				s_report_flag = 1; 		//解决第一次数据为0则不上报的问题
			}
			ret = REPORT_TYPE_SYNC;
		}
		break;
	
		default:
			MPrint("unknow cmd %x", cmd);
		break;
	}


	char* out = cJSON_PrintUnformatted(res);
	memcpy(json_data, out, strlen(out));
	F_FREE(out);
	cJSON_Delete(res);
	return ret;
}

int parse_start_record(cJSON* obj, OUT char* uart_hex)
{
	unsigned char control_data[256] = {0};
	//填充了 flag和data数据 从10字节开始
	cJSON* cmd = cJSON_GetObjectItem(obj, "command");
	int cmd_len = 0;
	int all_len = 0;
	if (cmd != NULL)	//灶具不会发送command指令则cmd_len填写0
	{
		cmd_len = put_payload_to_hex((char*)control_data, s_command, cJSON_GetObjectItem(obj, "command"));
	}
	all_len = 8+12+cmd_len;

	int pos = 0;
	uart_hex[pos++] = 0xf4;
	uart_hex[pos++] = 0xf5;
	uart_hex[pos++] = (all_len>>8) & 0xff;
	uart_hex[pos++] = all_len & 0xff;
	uart_hex[pos++] = (s_device_type>>8) & 0xff;		//type
	uart_hex[pos++] = (s_device_type) & 0xff;

	uart_hex[pos++] = 0x50;
	uart_hex[pos++] = 0x01;

	uart_hex[pos++] = 0;		//flag
	uart_hex[pos++] = 0;

	const char* recipeId = cJSON_GetObjectItem(obj, "recipeId")->valuestring;
	input_string_data(uart_hex+pos, recipeId, 12);
	pos += 12;			//recipeId

	//将控制数据拷贝到服务中
	memcpy(uart_hex+pos, control_data+10, cmd_len);
	pos += cmd_len; 			//attr flag and vals

	unsigned char crc_res = get_crc(uart_hex, pos);
	pos += input_big_end_data(uart_hex+pos, crc_res, 2);
	return pos;
}

int parse_stop_record(cJSON* obj, OUT char* uart_hex, const unsigned char host)
{
	int all_len = 8+12;

	int pos = 0;
	uart_hex[pos++] = 0xf4;
	uart_hex[pos++] = 0xf5;
	uart_hex[pos++] = (all_len>>8) & 0xff;
	uart_hex[pos++] = all_len & 0xff;
	uart_hex[pos++] = (s_device_type>>8) & 0xff;		//type
	uart_hex[pos++] = (s_device_type) & 0xff;

	uart_hex[pos++] = host;		//0x51 wifi(app) 发起   0x58 灶具发起
	uart_hex[pos++] = 0x01;

	uart_hex[pos++] = 0;		//flag
	uart_hex[pos++] = 0;

	const char* recipeId = cJSON_GetObjectItem(obj, "recipeId")->valuestring;
	input_string_data(uart_hex+pos, recipeId, 12);
	pos += 12;			//recipeId

	unsigned char crc_res = get_crc(uart_hex, pos);
	pos += input_big_end_data(uart_hex+pos, crc_res, 2);
	return pos;
}

int parse_terminate_cook(cJSON* obj, OUT char* uart_hex)
{
	int all_len = 8+12;

	int pos = 0;
	uart_hex[pos++] = 0xf4;
	uart_hex[pos++] = 0xf5;
	uart_hex[pos++] = (all_len>>8) & 0xff;
	uart_hex[pos++] = all_len & 0xff;
	uart_hex[pos++] = (s_device_type>>8) & 0xff;		//type
	uart_hex[pos++] = (s_device_type) & 0xff;

	uart_hex[pos++] = 0x54;		
	uart_hex[pos++] = 0x01;

	uart_hex[pos++] = 0;		//flag
	uart_hex[pos++] = 0;

	const char* recipeId = cJSON_GetObjectItem(obj, "recipeId")->valuestring;
	input_string_data(uart_hex+pos, recipeId, 12);
	pos += 12;			//recipeId

	unsigned char crc_res = get_crc(uart_hex, pos);
	pos += input_big_end_data(uart_hex+pos, crc_res, 2);
	return pos;
}

int parse_query_recipes(cJSON* obj, OUT char* uart_hex)
{
	int all_len = 8+12;

	int pos = 0;
	uart_hex[pos++] = 0xf4;
	uart_hex[pos++] = 0xf5;
	uart_hex[pos++] = (all_len>>8) & 0xff;
	uart_hex[pos++] = all_len & 0xff;
	uart_hex[pos++] = (s_device_type>>8) & 0xff;		//type
	uart_hex[pos++] = (s_device_type) & 0xff;

	uart_hex[pos++] = 0x52;
	uart_hex[pos++] = 0x01;

	uart_hex[pos++] = 0;		//flag
	uart_hex[pos++] = 0;
	
	const char* recipeId = cJSON_GetObjectItem(obj, "recipeId")->valuestring;
	input_string_data(uart_hex+pos, recipeId, 12);
	pos += 12;			//recipeId

	unsigned char crc_res = get_crc(uart_hex, pos);
	pos += input_big_end_data(uart_hex+pos, crc_res, 2);
	return pos;
}

int parse_start_cook(cJSON* obj, OUT char* uart_hex)
{
	//填充了 flag和data数据 从10字节开始
	const char* recipe_data = cJSON_GetObjectItem(obj, "recipeData")->valuestring;
	int size = strlen(recipe_data);
	MPrint("recipe size[%d] %s", size, recipe_data);
	int recipe_len = size/2;
	/*unsigned char data[recipe_len];		//菜谱数据最多50个节点 每个20字节
	memset(data, 0, sizeof(data));
	int i = 0;
	for (i = 0; i < size; i++)
	{
		cJSON * sub = cJSON_GetArrayItem(recipeData, i);
		int ver = cJSON_GetObjectItem(sub, "version")->valueint;
		int type = cJSON_GetObjectItem(sub, "type")->valueint;
		int mode = cJSON_GetObjectItem(sub, "mode")->valueint;
		int totalTime = cJSON_GetObjectItem(sub, "totalTime")->valueint;
		int currentTime = cJSON_GetObjectItem(sub, "currentTime")->valueint;
		int Tnum = cJSON_GetObjectItem(sub, "TNum")->valueint;
		int targetT = cJSON_GetObjectItem(sub, "targetT")->valueint;
		int currentT = cJSON_GetObjectItem(sub, "currentT")->valueint;

		
		data[i*20] = ver;
		data[i*20 + 1] = type;
		data[i*20 + 2] = mode;
		data[i*20 + 3] = (totalTime >> 8) & 0xff;
		data[i*20 + 4] = (totalTime ) & 0xff;
		data[i*20 + 5] = (currentTime >> 8) & 0xff;
		data[i*20 + 6] = (currentTime ) & 0xff;
		data[i*20 + 7] = Tnum;
		data[i*20 + 8] = (targetT >> 8) & 0xff;
		data[i*20 + 9] = (targetT ) & 0xff;
		data[i*20 + 10] = (currentT >> 8) & 0xff;
		data[i*20 + 11] = (currentT ) & 0xff;
	}
	*/
	int all_len = 8 + 12 + recipe_len;

	int pos = 0;
	uart_hex[pos++] = 0xf4;
	uart_hex[pos++] = 0xf5;
	uart_hex[pos++] = (all_len>>8) & 0xff;
	uart_hex[pos++] = all_len & 0xff;
	uart_hex[pos++] = (s_device_type>>8) & 0xff;		//type
	uart_hex[pos++] = (s_device_type) & 0xff;

	uart_hex[pos++] = 0x53;
	uart_hex[pos++] = 0x01;

	uart_hex[pos++] = 0;		//flag
	uart_hex[pos++] = 0;
	const char* recipeId = cJSON_GetObjectItem(obj, "recipeId")->valuestring;
	input_string_data(uart_hex+pos, recipeId, 12);
	pos += 12;			//recipeId

	input_string_data(uart_hex+pos, recipe_data, recipe_len);
	pos += recipe_len;
	
	unsigned char crc_res = get_crc(uart_hex, pos);
	pos += input_big_end_data(uart_hex+pos, crc_res, 2);
	
	return pos;
}

int build_query_status_hex(OUT char* uart_hex)
{
	int all_len = 8;

	int pos = 0;
	uart_hex[pos++] = 0xf4;
	uart_hex[pos++] = 0xf5;
	uart_hex[pos++] = (all_len>>8) & 0xff;
	uart_hex[pos++] = all_len & 0xff;
	uart_hex[pos++] = (s_device_type>>8) & 0xff;		//type
	uart_hex[pos++] = (s_device_type) & 0xff;

	uart_hex[pos++] = 0x30;
	uart_hex[pos++] = 0x01;

	uart_hex[pos++] = 0;		//flag
	uart_hex[pos++] = 0;

	unsigned char crc_res = get_crc(uart_hex, pos);
	pos += input_big_end_data(uart_hex+pos, crc_res, 2);
	return pos;
}

COMMON_API int service_json_to_hex(const char* json_data, OUT char* uart_hex)
{
	//char js[256] = "[{\"startRecord\":\"123\"}]";
	cJSON *req = cJSON_Parse(json_data);
	cJSON* obj = cJSON_GetArrayItem(req, 0);
	const char* key = obj->string;
	int len = 0;
	//MPrint("key[%s]", key);
	if (!strcmp(key, "startRecord"))
	{
		len = parse_start_record(obj, uart_hex);
	}
	else if (!strcmp(key, "stopRecordWiFi"))
	{
		len = parse_stop_record(obj, uart_hex, 0x51);
	}
	else if (!strcmp(key, "stopRecordStove"))
	{
		len = parse_stop_record(obj, uart_hex, 0x58);
	}
	else if (!strcmp(key, "queryRecipes"))
	{
		len = parse_query_recipes(obj, uart_hex);
	}
	else if (!strcmp(key, "startCook"))
	{
		MPrint("cook");
		len = parse_start_cook(obj, uart_hex);
	}
	else if (!strcmp(key, "terminateCook"))
	{
		len = parse_terminate_cook(obj, uart_hex);
	}
	else if (!strcmp(key, "updateAllStatus"))
	{
		clear_report_flag();
		len = build_query_status_hex(uart_hex);
	}
	else
	{
		MPrint("unknow service name %s", key);
	}

	cJSON_Delete(req);
	return len;
}


