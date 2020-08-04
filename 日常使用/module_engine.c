#include "module_engine.h"
#include "cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#define __FILENAME__ (filename(__FILE__))
#define MPrint(x, y...) do{	\
	printf("%s:%s<%d> " x, __FILENAME__, __FUNCTION__, __LINE__, ##y);\
	printf("\n");\
	}while(0)

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
static cJSON* s_command = NULL;
static cJSON* s_status = NULL;

static int s_command_len = 0;			//控制指令len    字段数值，包含bitflag和bitdata cmd stat crc 等
static int s_report_status_len = 0;		//payload长度

static char* s_report_status = NULL;	//用以缓存status数据，作增量上报


int get_content_len(const cJSON* head, const char* content);
int get_content_offset(const cJSON* head, const char* content);
char char_to_value(const char c);
int input_string_data(OUT char* buf, const char* value, const int len);
int input_big_end_data(OUT char* buf, const int value, const int len);
int put_data_to_hex(OUT char* hex, const cJSON* head, const char* content, const int input_value);
int put_payload_to_hex(OUT char* hex, const cJSON* head, const cJSON* req);
int get_crc(const char* hex, const int len);
int get_string_data(const char* hex, const int offset, const int len, OUT char* value);
int get_big_end_data(const char* hex, const int offset, const int len);
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
	s_command = cJSON_GetObjectItem(s_header, "command");
	CHECK_POINT(s_command);
	s_status = cJSON_GetObjectItem(s_header, "status");
	CHECK_POINT(s_status);
	
	s_command_len = 8 + get_content_len(s_command, "payload_bitflag") + get_content_len(s_command, "payload_data");
	s_report_status_len = get_content_len(s_status, "payload_data");
	MPrint("command_len[%d] report_len[%d]", s_command_len, s_report_status_len);
	s_report_status = F_MALLOC(s_report_status_len);
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

int input_big_end_data(OUT char* buf, const int value, const int len)
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
	int payload_bitflag_offset = get_content_offset(head, "payload_bitflag");
	int payload_bitflag_len = get_content_len(head, "payload_bitflag");
	int payload_data_offset = get_content_offset(head, "payload_data");
	int payload_data_len = get_content_len(head, "payload_data");
//	MPrint("bitof[%d]bitlen[%d]dataof[%d]datalen[%d]", payload_bitflag_offset,
//	payload_bitflag_len, payload_data_offset, payload_data_len);

	const char* payload_type = "command";
	int array_size = cJSON_GetArraySize(s_attribute_list);
	int req_size = cJSON_GetArraySize(req);
	int i = 0;
	int j = 0;
	int bit_flag = 0;	// support 4byte
	for(j = 0; j < req_size; j++)
	{
		cJSON* cmd_obj = cJSON_GetArrayItem(req, j);
		const char* key = cJSON_GetArrayItem(cmd_obj, 0)->string;
		
		for(i = 0; i < array_size; i++)
		{
			cJSON* node = cJSON_GetArrayItem(s_attribute_list, i);
			//access_mode 暂时用不到 通过 command的有无判断效果一样
			//const char* access_mode = cJSON_GetObjectItem(node, "accessMode")->valuestring;
			const char* attribute = cJSON_GetObjectItem(node, "attribute")->valuestring;
			if(strcmp(key, attribute) == 0)
			{
				cJSON* payload_describe = cJSON_GetObjectItem(node, payload_type);
				if (payload_describe)	//没有这个字段说明不可写 不会转成uart数据
				{
					int bit_flag_index = cJSON_GetObjectItem(payload_describe, "bit_flag_index")->valueint;
					int offset = cJSON_GetObjectItem(payload_describe, "offset")->valueint;
					int len = cJSON_GetObjectItem(payload_describe, "len")->valueint;
					if (len <= 4)
					{
						input_big_end_data(hex + payload_data_offset + offset, cJSON_GetArrayItem(cmd_obj, 0)->valueint, len);
					}
					else
					{
						input_string_data(hex + payload_data_offset + offset, cJSON_GetArrayItem(cmd_obj, 0)->valuestring, len);
					}
					bit_flag |= (1<<bit_flag_index);
				}
			}
		}
	}
	input_big_end_data(hex + payload_bitflag_offset, bit_flag, payload_bitflag_len);
	return payload_bitflag_len + payload_data_len;
}

int get_crc(const char* hex, int len)
{
	int i = 0;
	int result = 0;
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
		sprintf(value, "%s%2x", value, (unsigned char)hex[offset+i]);
	}
	return len;
}

int get_big_end_data(const char* hex, const int offset, const int len)
{
	int i = 0;
	int value = 0;
	for (i = 0; i < len; i++)
	{
		//两种写法一样的
		value += (hex[offset+len-i-1] << (i * 8)) & 0xff;
		//value += (hex[i] << ((len-1-i)* 8)) & 0xff;
	}
	return value;
}

int build_json(const char* payload, OUT cJSON* result)
{
	int i = 0;
	int j = 0;
	int array_size = cJSON_GetArraySize(s_attribute_list);
	const char* payload_type = "status";
	for (i = 0; i < s_report_status_len; i++)
	{
		if (payload[i] == s_report_status[i])
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
				const char* attribute = cJSON_GetObjectItem(node, "attribute")->valuestring;			
				cJSON* payload_describe = cJSON_GetObjectItem(node, payload_type);
				if (payload_describe)
				{
					int offset = cJSON_GetObjectItem(payload_describe, "offset")->valueint;
					int len = cJSON_GetObjectItem(payload_describe, "len")->valueint;
					if (i >= offset && i < offset+len)	//识别到属性数据有变化
					{
						if (len <= 4)
						{
							int value = get_big_end_data(payload, offset, len);
							cJSON* sub = cJSON_CreateObject();
							cJSON_AddNumberToObject(sub, attribute, value);
							cJSON_AddItemToArray(result, sub);
						}
						else	//大于4字节的是字符串类型
						{
							char* value = F_MALLOC(len*2 + 1);
							if (value)
							{
								memset(value, 0, len*2+1);
								get_string_data(payload, offset, len, value);
								cJSON* sub = cJSON_CreateObject();
								cJSON_AddStringToObject(sub, attribute, value);
								cJSON_AddItemToArray(result, sub);
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
}

int json_to_hex(const char* json_data, OUT char* uart_hex)
{
	CHECK_POINT(s_config_file);
	cJSON *req = cJSON_Parse(json_data);
	CHECK_POINT(req);

	//固定的
	char* hex = F_MALLOC(s_command_len + 4); 	//4字节包含head和len
	CHECK_POINT(hex);
	memset(hex, 0, s_command_len + 4);
	int pos = 0;

	pos += put_data_to_hex(hex, s_command, "head", 0);
	pos += put_data_to_hex(hex, s_command, "len", s_command_len);
	pos += put_data_to_hex(hex, s_command, "type", 0);
	pos += put_data_to_hex(hex, s_command, "cmd", 0);
	pos += put_data_to_hex(hex, s_command, "stat", 0);
	pos += put_data_to_hex(hex, s_command, "flags", 0);
	pos += put_payload_to_hex(hex, s_command, req);
	int crc_res = get_crc(hex, pos);
	pos += put_data_to_hex(hex, s_command, "crc", crc_res);

	memcpy(uart_hex, hex, pos);
	F_FREE(hex);
	cJSON_Delete(req);
	return SUCCESS;
}

int hex_to_json(const char* uart_hex, OUT char* json_data)
{
	CHECK_POINT(s_config_file);
	cJSON* res = cJSON_CreateArray();
	int payload_offset = get_content_offset(s_status, "payload_data");
	char * payload = F_MALLOC(s_report_status_len);
	CHECK_POINT(payload);
	memcpy(payload, uart_hex + payload_offset, s_report_status_len);
	build_json(payload, res);
	memcpy(s_report_status, payload, s_report_status_len);

	char* out = cJSON_PrintUnformatted(res);
	memcpy(json_data, out, strlen(out));
	F_FREE(out);
	F_FREE(payload);
	cJSON_Delete(res);
	return SUCCESS;
}

