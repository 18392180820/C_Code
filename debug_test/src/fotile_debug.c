/******************************************************************************
 * @file	fotile_debug.c
 * @author	Yugk
 * @version V1.1.0
 * @date	20-03-2020
 * @brief	to implement the dev(gw and node managemant)
 ******************************************************************************

/******************************************************
*						Include
******************************************************/

#include "fotile_debug.h"
#include "fotile_debug_order.h"

/******************************************************
*						Macros
******************************************************/
#define FOTILE_CMD_ARGS_SIZE 		(16)
#define FOTILE_CMD_ARGS_LEN 		(64)

/******************************************************
*				 Function Define
******************************************************/
void fotile_cmd_debug_op(int argc, char argv[][64])
{

//TODO: need to confirm command format

	if(0 == strcmp(argv[1], DUBUG_CMD_REBOOT_DEV)) 				//cmd: reboot gateway
	{
		cmd_restart_gateway_op();
	}
	else if(0 == strcmp(argv[1], DEBUG_CMD_UNICAST))			//cmd: gateway lora unicast, point to point
	{
		cmd_gateway_unicast_op();
	}
	else if(0 == strcmp(argv[1], DEBUG_CMD_MULTICAST))			//cmd: gateway lora multicast, point to group
	{
		cmd_gateway_multicast_op();
	}
	else if(0 == strcmp(argv[1], DEBUG_CMD_BROADCAST))			//cmd: gateway lora broadcast, point to all
	{
		cmd_gateway_broadcast_op();
	}
	else if(0 == strcmp(argv[1], DEBUG_CMD_LORA_OFF))			//cmd: turn off gateway lora service actively
	{
		cmd_turn_off_lora_service_op();
	}
	else if(0 == strcmp(argv[1], DEBUG_CMD_LORA_ON))			//cmd: turn on gateway lora service actively
	{
		cmd_turn_on_lora_service_op();
	}
	else if(0 == strcmp(argv[1], DEBUG_CMD_MQTT_OFF))			//cmd: cut off gateway mqtt connection from broker actively
	{
		cmd_cut_off_mqtt_link_op();
	}
	else if(0 == strcmp(argv[1], DEBUG_CMD_MQTT_CON))			//cmd: connect mqtt broker actively 
	{
		cmd_con_mqtt_broker_op();
	}
	else if(0 == strcmp(argv[1], DEBUG_CMD_MQTT_SEND))			//cmd: send mqtt message to broker for debug
	{
		cmd_send_mqtt_msg_op();									
	}
	else
	{
		CONSOLE_DBG_E("### UNKNOWN CMD ");
	}

	return;
}

/**
* @ Description: process info order
* @ argv[][64]: order in 2-d array format
* @ return: void
*/
void fotile_info_debug_op(char argv[][64])
{
	if((0 == strcmp(argv[1], "?")) 
	 || (0 == strcmp(argv[1], "h"))
	 || (0 == strcmp(argv[1], "help")))
	{
		CONSOLE_DBG_I("cmd/info -specific_string");
		//TODO: add more context in help needed
	}
	else if(0 == strcmp(argv[1], DUBUG_INFO_FT_SF_VER)) 		//info: check fotile software version
	{
		CONSOLE_DBG_I("recv version info ");
	}
	else if(0 == strcmp(argv[1], DUBUG_INFO_GW_RAM_STAT))		//info: check gateway ram information
	{
		info_get_sys_ram_op();
	}
	else if(0 == strcmp(argv[1], DUBUG_INFO_GW_IPC_STAT))		//info: check gateway ipc information
	{
		info_get_sys_ipc_op();
	}
	else if(0 == strcmp(argv[1], DUBUG_INFO_ND_STAT))			//info: check node status
	{
		CONSOLE_DBG_I("recv nd_stat info ");
	}
	else if(0 == strcmp(argv[1], DUBUG_INFO_ND_CON_STAT))		//info: check node connection status
	{
		CONSOLE_DBG_I("recv nd_con info ");
	}
	else if(0 == strcmp(argv[1], DUBUG_INFO_OUTDEV_WORK_STAT))	//info: check outdoor device work status
	{
		CONSOLE_DBG_I("recv outdev_work_stat info ");
	}
	else if(0 == strcmp(argv[1], DUBUG_INFO_TASK_LIST)) 		//info: check thread task running status
	{
		CONSOLE_DBG_I("recv task_list info ");
	}
	else if(0 == strcmp(argv[1], DUBUG_INFO_MQ_STAT))			//info: check message queue status		
	{
		CONSOLE_DBG_I("recv mq_stat info ");
	}
	else if(0 == strcmp(argv[1], DUBUG_INFO_WIFI_CON_STAT))		//info: check wifi connection status		
	{
		CONSOLE_DBG_I("recv wifi_con_stat info ");
	}
	else
	{
		CONSOLE_DBG_E("### UNKNOWN INFO ");
	}

	return;
}

/**
* @ Description: confirm the debug mode
* @ mode: char*
* @ return: mode choice
*/
int confirm_debug_mode(char* mode)
{
	if(0 == strcmp(mode, DEBUG_CMD_PREFIX))
	{
		return DEBUG_CMD_MODE;
	}
	else if (0 == strcmp(mode, DEBUG_INFO_PREFIX))
	{
		return DEBUG_INFO_MODE;
	}
	else
	{
		return DEBUG_ERROR_MODE;
	}
}

/**
* @ Description: confirm debug mode and process
* @ argc:width of the string input by termianl(sepearted by blank space)
* @ argv[][64]:string input in 2-d array format
* @ return: mode choice
*/
void process_debug_order(int argc, char argv[][64])
{
	if (argc == 0)
	{
		return;
	}

	if((NULL != argv[1]) && ('-' == *argv[1]))
	{
		memcpy(argv[1], argv[1]+1, 63);
	}

	switch(confirm_debug_mode(argv[0]))
	{
		case DEBUG_CMD_MODE:

			//CONSOLE_DBG_I("CMD MODE");
			fotile_cmd_debug_op(argc, argv);
		
			break;

		case DEBUG_INFO_MODE:	
			
			//CONSOLE_DBG_I("INFO MODE");
			fotile_info_debug_op(argv);
		
			break;

		case DEBUG_ERROR_MODE:
		default:
			CONSOLE_DBG_E("### UNKNOW MODE, PLEASE RETRY OR EXIT");
			break;
	}
	CONSOLE_DBG_B();
}


/**
* @ Description: string to 2-d array, sepearted by blank space
* @ arg: string input by termianl
* @ args: 2-d array to store string
* @ return: array width(seperated by blank space)
*/
int get_args_size(char* arg, char args[][FOTILE_CMD_ARGS_LEN])
{
	int args_size = 0;
	int sublen = 0;
	int len = 0;
	int local = 0;

	char* p = arg;

	if(0 == *p)
	{
		return 0;
	}

	while(10 != *p)
	{
		if(32 != *p)
		{
			sublen ++;
			len ++;
			p ++;
		}
		else
		{
			if(sublen > 0)
			{
				//read sub command
				if(sublen > FOTILE_CMD_ARGS_LEN - 1)
				{
					memcpy(args[args_size++], arg+local, FOTILE_CMD_ARGS_LEN-1);
					CONSOLE_DBG_W("cmd len overflow");
				}
				else
				{
					memcpy(args[args_size++], arg+local, sublen);
				}

				if(args_size >= FOTILE_CMD_ARGS_SIZE)
				{
					return FOTILE_CMD_ARGS_SIZE;
				}
				
				len ++;
				local = len;
				sublen = 0;
				p ++;
			}
			else
			{
				//read blank space form begin
				len ++;
				local = len;
				p ++;
				
				continue;
			}
		}
	}
	
	memcpy(args[args_size++], arg+local, sublen);

	return args_size;
}

/**
* @ Description: read string from terminal and start debug cycle
* @ arg:string input by termianl
* @ return: void
*/
void process_debug_input_op(char* arg)
{
	char argv[FOTILE_CMD_ARGS_SIZE][FOTILE_CMD_ARGS_LEN];

	memset(argv, 0x00, sizeof(argv));
	
	int argc = get_args_size(arg, argv);
	
	process_debug_order(argc, argv);
}

/**
* @ Description: read string from terminal and start debug cycle
* @ input:  void
* @ return: void
*/
void* debug_adb_cycle(void* parm)
{	
	pthread_detach(pthread_self());
	
	char buf[1024];

	CONSOLE_DBG_I("start debug_console_thread");

	while(GATEWAY_DEBUG_SWITCH)
	{
		setbuf(stdin, NULL);

		memset(buf, 0x00, sizeof(buf));

		char* s = fgets(buf, 1024, stdin);

		if (32 == buf[0]) //32 related to blank space, push button with blank space+enter to exit cycle
		{
			CONSOLE_DBG_W("exit debug cycle");
			break;
		}

		process_debug_input_op(buf);

		/* delay 50ms */
		usleep(DEBUG_INPUT_DELAY);
	}

	pthread_exit(NULL);
	
}

int create_debug_adb_handler(void)
{
	pthread_t id;
	
	if (0 != pthread_create(&id, NULL, (void*)debug_adb_cycle, NULL))
	{
		CONSOLE_DBG_E("###Error: create create console_debug_handler failed");

		return DEBUG_ERROR;
	}
	
	CONSOLE_DBG_I("--- create debug_adb_handler succ ---");
	pthread_join(id , NULL);			//in lora gateway code, needed to be shielded ???
	

	return DEBUG_OK;
		
}

/**
* @ Description: present system information per interval seconds
* @ input:  interval
* @ return: void
*/
void* debug_cycle(void* interval)
{
	CONSOLE_DBG_I("------ start debug cycle ------");
	
	int *p = (int*)interval;
	
	while(GATEWAY_DEBUG_SWITCH)
	{
		info_get_sys_ipc_op();
		info_get_sys_ram_op();

		if(p)
		{
			sleep(*p);
		}
		else
		{
			sleep(30);
		}
		
	}
}

/**
* @ Description: start debug thread, present information every interval seconds
* @ input:  interval(seconds)
* @ return: void
*/
int create_debug_handler(int interval)
{
	pthread_t id;
	
	if (0 != pthread_create(&id, NULL, (void*)debug_cycle, (void*)&interval))
	{
		CONSOLE_DBG_E("###Error: create create console_debug_handler failed");

		return DEBUG_ERROR;
	}
	
	CONSOLE_DBG_I("------ create create console_debug_handler success ------");

	pthread_join(id , NULL);
	
	return DEBUG_OK;
	
}

