#include "module_engine.h"
#include "cJSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RANGHOOD_MODULE_FILE_PATH "../../config/ranghood_module.config"
#define DISINFECTION_MODULE_FILE_PATH "../../config/xdg_module.config"
#define STEAMER_MODULE_FILE_PATH "../../config/SCD39-C2.i_module.config"

#define SCHEMA_CJSON_QUERY_DEV_VER					"queryDevVersionInfo"
#define SCHEMA_CJSON_UPDATE_FIRMWARE				"appUpdateFirmware"
#define SCHEMA_CJSON_VERSION						"version"
#define SCHEMA_CJSON_MD5							"md5"
#define SCHEMA_CJSON_URLLEN 						"urlLen"
#define SCHEMA_CJSON_URL							"url"


#define Trace() printf("%s<%d>\n", __FUNCTION__, __LINE__)

static const char* update = "{\"appUpdateFirmware\":{\"version\":\"FIKS-WA104\",\"md5\":\"1567C30D0F798918CC55968B2B0C7A9A\",\"urlLen\":\"150\",\"url\":\"http://fotile-ota.oss-cn-hangzhou.aliyuncs.com/Z-%E8%91%A3%E6%98%8E%E6%98%8A%E4%B8%B4%E6%97%B6/%E5%BD%92%E4%B8%80%E5%8C%96_C2T/FIKS-WA104.bin\"}}";
static const char* version = "{\"queryDevVersionInfo\":\"NULL\"}";

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

void RanghoodTest()
{
	int ret = load_config_file(RANGHOOD_MODULE_FILE_PATH);
	char req[256] = "[{\"PowerSwitchAll\":1},{\"Light\":2},{\"DelayShutdown\":30},{\"CookID\":\"00112233445566778899aabb\"}]";
	char hex[256] = {0};
	json_to_hex(req, hex);

	cJSON* js = cJSON_Parse(req);
	char* out = cJSON_Print(js);
	printf("request= %s\n", out);
	printf("convert result= ");
	print_hex(hex, 40);

	char status[256] = {0xf4, 0xf5, 0, 56, 4,0,0x30,2,0,0,	1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,25,0,0,0xff,0xee,0xdd,0xcc,0xbb,0xaa,
		0x99,0x88,0x77,0x66,0x55,0x44,0,0,0,0,2,50,0,0,0,0,0,0,0,0,0,0,4,1};
	//	26
	char report[1024] = {0};
	printf("\n\n uart data= ");
	print_hex(status, 62);
	hex_to_json(status, report);
	cJSON* res = cJSON_Parse(report);
	printf("convert result= %s\n", cJSON_Print(res));

	char status2[256] = {0xf4, 0xf5, 0, 56, 4,0,0x30,2,0,0,  1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,25,0,0,0xff,0xee,0xdd,0xcc,0xbb,0xaa,
		0x99,0x88,0x77,0x66,0x56,0x44,0,0,0,0,2,50,0,0,0,0,0,0,0,0,0,0,4,1};
	printf("uart data= ");
	print_hex(status2, 62);
	char report2[1024] = {0};
	hex_to_json(status2, report2);
	cJSON* res2 = cJSON_Parse(report2);
	printf("convert result= %s\n", cJSON_Print(res2));

	cJSON_Delete(js);
	free(out);
}

void XDGTest()	//消毒柜
{
	int ret = load_config_file(DISINFECTION_MODULE_FILE_PATH);
	char req[1024] = "[{\"PowerSwitchAll\":1},{\"Cancel\":2},{\"WorkMode\":30},{\"SmartMode\":30},{\"TestMode\":30},{\"WorkTime\":30},{\"AppointmentStartTime\":30},{\"Output\":30},{\"DoorMoveControl\":30},{\"AppointmentFlag\":30},{\"BreakfastTime\":286331153},{\"LunchTime\":572662306},{\"SupperTime\":858993459},{\"ExtraTime1\":1145324612},{\"ExtraTime2\":4294967278}]";
	char hex[256] = {0};
	json_to_hex(req, hex);

	cJSON* js = cJSON_Parse(req);
	char* out = cJSON_Print(js);
	printf("request= %s\n", out);
	printf("convert result= ");
	print_hex(hex, 53);
	
	cJSON_Delete(js);
	free(out);

	char status[256] = {0xf4, 0xf5, 0, 72, 6,0,0x30,2,0,0,
	2,1,4,6,3,1,0,0,2,2,								0xf,5,0,25,66,18,0xf,3,0,1,
	0xf,2,1,0x11,0x11,0x11,0x11,0x22,0x22,0x22,			0x22,0x33,0x33,0x33,0x33,0x44,0x44,0x44,0x44,0xff,
	0xff,0xff,0xee,1,1,3,0xff,0xff,1,1,					0,1,1,13,2,2,0,0,0,0,
	0,0,0,0,0, 0xfc};
	//	
	char report[1024] = {0};
	printf("\n\n uart data= ");
	print_hex(status, 62);
	hex_to_json(status, report);
	cJSON* res = cJSON_Parse(report);
	printf("convert result= %s\n", cJSON_Print(res));

	char status2[256] = {0xf4, 0xf5, 0, 72, 6,0,0x30,2,0,0,
	2,1,4,6,3,1,0,0,2,2,								0xf,5,0,25,66,18,0xb,3,0,1,
	0xf,2,1,0x11,0x11,0x11,0x11,0x22,0x22,0x22,			0x22,0x33,0x33,0x33,0x33,0x44,0x44,0x44,0x44,0xff,
	0xff,0xfe,0xee,1,1,3,0xff,0xff,1,1,					0,1,1,13,2,2,0,0,0,0,
	0,0,0,0,0, 0xfc};
	printf("uart data= ");
	print_hex(status2, 62);
	char report2[1024] = {0};
	hex_to_json(status2, report2);
	cJSON* res2 = cJSON_Parse(report2);
	printf("convert result= %s\n", cJSON_Print(res2));

}

void SteamerTest()
{
	int ret = load_config_file(STEAMER_MODULE_FILE_PATH);															// 1小时 5分
	char req[256] = "[{\"PowerSwitchAll\":1},{\"Light\":2},{\"PushRod\":0},{\"CookID\":\"00112233445566778899aabb\"},{\"SetWorkTime\":66816},{\"SetAppointmentTime\":9}]";
	//char req[256] = "{\"PowerSwitchAll\":2,\"Light\":2,\"CookID\":\"00112233445566778899aabb\"}";
	char hex[256] = {0};
	json_to_hex(req, hex);

	cJSON* js = cJSON_Parse(req);
	char* out = cJSON_Print(js);
	printf("request= %s\n", out);
	printf("convert result= ");
	print_hex(hex, 40);

	char status[256] = {0xf4, 0xf5, 0, 72, 1,0,0x30,2,0,0,  1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,120,120,120,
		255,255,119,119,119,10,20,20,1,1,1,1,1,0,0,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,  0};
	//	26
	char report[1024] = {0};
	printf("\n\n uart data= ");
	print_hex(status, 62);
	hex_to_json(status, report);
	cJSON* res = cJSON_Parse(report);
	printf("convert result= %s\n", cJSON_Print(res));

	char status2[256] = {0xf4, 0xf5, 0, 72, 1,0,0x30,2,0,0,  1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,120,120,120,
		255,255,119,119,119,10,20,21,1,1,1,1,1,0,0,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,  0};
	printf("uart data= ");
	print_hex(status2, 62);
	char report2[1024] = {0};
	hex_to_json(status2, report2);
	cJSON* res2 = cJSON_Parse(report2);
	printf("convert result= %s len[%lu]\n", cJSON_Print(res2), strlen(report2));

	cJSON_Delete(js);
	free(out);
}

void ServiceTest_start_record()
{
	int ret = load_config_file(STEAMER_MODULE_FILE_PATH);
	//char req[256] = "{\"startRecord\":{\"recipeId\":\"1234567890ab1234567890ab\",\"command\":{\"Light\":2,\"PowerSwitchAll\":1,\"PushRod\":0,\"SetAppointmentTime\":10,\"SetWorkTime\":25}}}";
	char req[256] = "{\"startRecord\":{\"recipeId\":\"1234567890ab1234567890ab\"}}";
	char hex[256] = {0};
	int ll = service_json_to_hex(req, hex);
	printf("convert len[%d]\n", ll);
	print_hex(hex, ll);

	printf("now test report\n");
	char status[256] = {0xf4, 0xf5, 0, 21, 1,0,0x50,2,0,0,  1, 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb, 0xcc,0, 13};
	char report[1024] = {0};
	print_hex(status, 26);
	hex_to_json(status, report);
	cJSON* res2 = cJSON_Parse(report);
	printf("convert result= %s\n", cJSON_Print(res2));
	cJSON_Delete(res2);
}

void ServiceTest_stop_record_wifi()
{
	int ret = load_config_file(STEAMER_MODULE_FILE_PATH);
	char req[256] = "{\"stopRecordWiFi\":{\"recipeId\":\"1234567890ab1234567890ab\"}}";
	char hex[256] = {0};
	int ll = service_json_to_hex(req, hex);
	printf("convert len[%d]\n", ll);
	print_hex(hex, ll);

	printf("now test report\n");
	char status[256] = {0xf4, 0xf5, 0, 22, 1,0,0x51,2,0,0,  1, 1,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb, 0xcc,0, 13};
	char report[1024] = {0};
	print_hex(status, 26);
	hex_to_json(status, report);
	cJSON* res2 = cJSON_Parse(report);
	printf("convert result= %s\n", cJSON_Print(res2));
	cJSON_Delete(res2);
}

void ServiceTest_stop_record_stove()
{
	int ret = load_config_file(STEAMER_MODULE_FILE_PATH);


	
	char status[256] = {0xf4, 0xf5, 0, 22, 1,0,0x58,2,0,0,  1, 1,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb, 0xcc,0, 13};
	char report[1024] = {0};
	print_hex(status, 26);
	hex_to_json(status, report);
	cJSON* res2 = cJSON_Parse(report);
	printf("convert result= %s\n", cJSON_Print(res2));

	printf("now test app report\n");
	char req[256] = "{\"stopRecordStove\":{\"recipeId\":\"1234567890ab1234567890ab\"}}";
	char hex[256] = {0};
	int ll = service_json_to_hex(req, hex);
	printf("convert len[%d]\n", ll);
	print_hex(hex, ll);

	
	cJSON_Delete(res2);
}

void ServiceTest_query_recipes()
{
	int ret = load_config_file(STEAMER_MODULE_FILE_PATH);
	char req[256] = "{\"queryRecipes\":{\"recipeId\":\"1234567890ab1234567890ab\"}}";
	char hex[256] = {0};
	int ll = service_json_to_hex(req, hex);
	printf("convert len[%d]\n", ll);
	print_hex(hex, ll);

	printf("now test report 67\n");
	char status[256] = {0xf4, 0xf5, 0, 62, 1,0,0x52,2,0,0,  0, 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb, 0xcc,    2, 
	01, 03, 01, 0, 0x0b, 00, 00, 03, 00, 0x64, 00, 0x64, 00,00, 00,00, 00,00, 00,00,
	01, 03, 01, 00,0x0b, 00,00 ,03 ,00 ,0x64 , 00, 0x64, 00,00, 00,00, 00,00, 00,00,
	0, 13};
	char report[1024] = {0};
	print_hex(status, 67);
	hex_to_json(status, report);
	cJSON* res2 = cJSON_Parse(report);
	printf("convert result= %s\n", cJSON_Print(res2));
	cJSON_Delete(res2);
}

void ServiceTest_start_cook()
{
	int ret = load_config_file(STEAMER_MODULE_FILE_PATH);
	char req[1024] = "{\"startCook\":{\"recipeId\":\"1234567890ab\",\"recipeData\":\"010302003c000003001e001e0000000000000000\"}}";
	char hex[256] = {0};
	int ll = service_json_to_hex(req, hex);
	printf("convert len[%d]\n", ll);
	print_hex(hex, ll);

	printf("now test report 26\n");
	char status[256] = {0xf4, 0xf5, 0, 21, 1,0,0x53,2,0,0,  1,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb, 0xcc,0, 13};
	char report[1024] = {0};
	print_hex(status, 26);
	hex_to_json(status, report);
	cJSON* res2 = cJSON_Parse(report);
	printf("convert result= %s\n", cJSON_Print(res2));
	cJSON_Delete(res2);
}

void ServiceTest_all_report()
{
	int ret = load_config_file(STEAMER_MODULE_FILE_PATH);
	char req[1024] = "{\"updateAllStatus\":null}";
	char hex[256] = {0};
	int ll = service_json_to_hex(req, hex);
	printf("convert len[%d]\n", ll);
	print_hex(hex, ll);
}

int main()
{
	//cJSON_TEST();
	//RanghoodTest();
	//XDGTest();
	//SteamerTest();
	//ServiceTest_start_record();
	//ServiceTest_stop_record_wifi();
	//ServiceTest_stop_record_stove();
	//ServiceTest_query_recipes();
	//ServiceTest_start_cook();
	//ServiceTest_all_report();
	
	//while(1)
	{
		//sleep(1);
	}
	cJSON* req = cJSON_Parse(version);//cJSON* req = cJSON_Parse(update);
	cJSON* obj = cJSON_GetArrayItem(req, 0);

	cJSON* sub_version	= cJSON_GetObjectItem(obj, SCHEMA_CJSON_VERSION);
    cJSON* sub_md5	 	= cJSON_GetObjectItem(obj, SCHEMA_CJSON_MD5);
    cJSON* sub_url	 	= cJSON_GetObjectItem(obj, SCHEMA_CJSON_URL);
	
	printf("convert result= \n%s\n\n", cJSON_Print(obj));

	printf("convert result= \n%s\n", obj->string);
	//printf("convert result= \n%s\n", obj->string);

	
}


