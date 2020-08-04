

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "print.h"
#include "cJSON.h"

#define STRING "STRING"


#define DEFINE_CHECK	0

#if DEFINE_CHECK
#define HELLO           2
#else
#define HELLO           3
#endif


#define HONG 0

#ifdef HONG 
#define NUMBER (4000)
#else
#define NUMBER (3000)
#endif


typedef void * handle_t;

static handle_t hand;

#define FAILURE (-1)

#define F_MALLOC(dst,len, size) do{\
					dst = malloc(len*size);\
					if(NULL ==  dst){return FAILURE;}\
				}while(0);

			
#define F_FREE(p) do{\
					if(p!=NULL){free(p);p = NULL;}\
				}while(0);


//check handle address
void handle_address_check(handle_t* hand_in)
{
	if(hand_in)
	{
		printf("handler address = %p\n", hand_in);  
	}
	else
	{
		printf("handler address\n");
	}
}

//check handle null
void handle_null_check(handle_t hand_in)
{
	if(hand_in)
	{
		printf("handler != NULL\n");  
	}
	else
	{
		printf("handler = NULL\n");
	}
}

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

#define TOPIC_STR 	"topics"
#define IP_STR 		"ip"
#define PORT_STR	"port"

void json_test(void) /* this function has ram lack in cJSON_Print */
{
	cJSON* cxt  = NULL;
	cJSON* data = NULL;
	cJSON* part = NULL;

	cxt    = cJSON_Parse(msg);
	data   = cJSON_GetArrayItem(cxt, 0);

	printf("cxt  = %s\n\n", cJSON_Print(cxt));
	printf("data = %s\n\n", cJSON_Print(data));

	//firsr test
	part   = cJSON_GetObjectItem(data, TOPIC_STR);
	printf("part = %s\n\n", cJSON_Print(part));

#if 0
	part   = cJSON_GetObjectItem(data, IP_STR);
	printf("part = %s\n\n", cJSON_Print(part));

	part   = cJSON_GetObjectItem(data, PORT_STR);
	printf("part = %s\n\n", cJSON_Print(part));
#endif
	
	//cJSON_Delete(part);

	//printf("cxt  = %s\n\n", cJSON_Print(cxt));
	//printf("data = %s\n\n", cJSON_Print(data));

	cJSON_Delete(cxt);
	//cJSON_Delete(data);
	//cJSON_Delete(part);

	if(cxt == NULL)
	{
		printf("cxt = NULL\n");
	}

	if(data == NULL)
	{
		printf("data = NULL\n");
	}

	if(part == NULL)
	{
		printf("part = NULL\n");
	}
}


const char* conststring = "0123456789111111";

unsigned short uart_receive_frame_playload_handle(unsigned char *playloadin, unsigned short size, unsigned char *playloadOut)
{

    unsigned int i=0,j=0;

	if(NULL == playloadin || NULL == playloadOut)
	{
	    printf("WARN: Error in parameter.\n");
		return 0;
	}

	for(i = 0; i < size; i++)
    {
		if((i < size - 1) && (playloadin[i] == 0xF4) && (playloadin[i+1] == 0xF4))
		{
            playloadOut[j++] = 0xF4;
			i++;
        }
        else 
		{
			playloadOut[j++] = playloadin[i];
        }
    }

	return j;
}

typedef struct
{
	int i;
	int j;
	int k;
}_E_type;

typedef struct 
{
	int a;
	int b;
	int c;
	char d[20];
	char e[40];
	//char* n;
}e_struct;

//#define ss(ssd) takehhh(ssd)
void takehhh(char* ssd);

void take2(char* ssd)
{

	takehhh(ssd);
}

void takehhh(char* ssd)
{

	printf("### msg = %s\n", ssd);
}

u_int16_t Crc16 (void* Dat, u_int8_t Start_Byte, u_int8_t Length) 
{
	u_int8_t  i = 0;
	u_int16_t Crc = 0xFFFF;
	u_int8_t* prt;
	prt = Dat+Start_Byte;
	while ( Length-- ) 
	{
			Crc ^= *prt++;
			for(i = 0; i<8; i++) 
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

int main(void)
{
	#if 0
	char* data_buffer = (char*)malloc(1024);

	memset(data_buffer, 0, 1024);

	memcpy(data_buffer, STRING, sizeof(STRING));

	printf("sizeof(data_buffer)  = %ld %s\n", sizeof(data_buffer), data_buffer);
	printf("sizeof(*data_buffer) = %ld %ld\n", sizeof(*data_buffer), strlen(data_buffer));
	printf("sizeof(*data_buffer) = %c  %c  %c\n", *data_buffer, *data_buffer+1, *(data_buffer+1));

	free(data_buffer);
	data_buffer = NULL;

	handle_address_check(&hand);
	printf("%p\n",&hand);			//check again


	handle_null_check(hand);

	//char* mes = "hello";
	//printf("ddddd = %c", mes[2]);

	printf("%ld ######## %ld\n", strlen(STRING), sizeof(STRING));

	char str_2[10];
	memset(str_2, 0x00, sizeof(str_2));
	memcpy(str_2, STRING, strlen(STRING));

	char* loc = NULL;
	loc = &str_2[2];
	printf("loc = %s\n", loc);
	#endif

	//json_test();

	//int list[2] ={1};
	//printf("%d\n", list[0]);
	//printf("%d\n", list[1]);
#if 0
	int a = 1;
	int b = 0;
	printf("(a|b = %d)\n", a|b);

	unsigned int cc = -1;
	printf("cc = %d\n", cc);

	printf("res = %d\n", cc == 255);

	if(255 == cc)
	{
		printf("yes  ### 333 \n");
	}
	else
	{

		printf("oh mu gou \n");
	}

	printf("result = %d\n", 1==1);

	char my_St[10];
	memset(my_St, 0x00, 10);
	memcpy(my_St, conststring, 10);

	printf("my_St = %s\n", my_St);
	memcpy(my_St, &my_St[3], 10);
	printf("my_St = %s\n", my_St);


	int i = 0;
	unsigned char log[20] = {0xF4,0xF4,0xF4,0xF4,0xF4,0xF4,0xF4,0xF4,0xF4,0xF4,0xF4,0xF4,0xF4, 0xF4};

	printf("### result = %d\n", uart_receive_frame_playload_handle(log, 20, log));

	for(i=0; i<20; i++)
	{

		printf("%02X ", log[i]);
	}
	printf("\n");


	_E_type tyo;
	printf("sizeof = %ld  ### %ld", sizeof(tyo), sizeof(_E_type));

	e_struct AA;


	//e_struct* pAA = &AA;

	memset(&AA, 0x00, sizeof(e_struct));
	
	AA.a = 1;
	AA.b = 2;
	AA.c = 3;
#endif
#if 0
	printf("result = %d\n", NULL == AA.n);

	if(NULL == AA.n)
	{
		printf(" == \n");
	}
	else
	{
		printf(" != \n");
	}
#endif

#if 0
	printf("sizeof(int)  = %ld\n", sizeof(int));
	printf("sizeof(int*)  = %ld\n", sizeof(int*));

	printf("AA.a address = %p\n", &AA.a);
	printf("AA.b address = %p\n", &AA.b);
	printf("AA.c address = %p\n", &AA.c);

	printf("Crc = %02X\n", Crc16(&AA.a, 0 , sizeof(e_struct)));
	printf("Crc = %02X\n", Crc16(&AA.a, 0 , (&AA.c - &AA.a)));

	printf("address cut = %ld\n", (&AA.c - &AA.a));
	printf("address cut = %ld\n", (AA.e - AA.d));
	printf("address cut = %ld\n", (&AA.e[2] - &AA.e[1]));

	printf("address cut = %ld\n", ((void*)AA.d - (void*)&AA.a));

	//take2("hello");

	u_int16_t crc_ver = 0x1234;
	u_int16_t crc_res = ((crc_ver>>8)&0xFF)+((crc_ver<<8)&0xFF00);

	printf("crc_ver = %02X\n", crc_ver);
	printf("crc_res = %02X\n", crc_res);

	printf("###HELLO = %d\n", HELLO);
#endif

	char* malc;
	
	F_MALLOC(malc, 10 ,1);


	memcpy(malc, "hello", 5);
	printf("malc = %s\n", malc);

	int i = 0;

	printf("hex = ");
	for(i=0;i<10;i++)
	{
		printf("%02X ", malc[i]);
	} 

	printf("\n");

	F_FREE(malc);

	if(NULL == malc)
	{
		printf("malc is freed\n");
	}

	return 1;
	
}
