/******************************************************************************
 * @file	fotile_debug_order.c
 * @author	DMH
 * @version V1.1.0
 * @date	31-03-2020
 * @brief	contain functions for specific cmd/info debug
 ******************************************************************************/

#include "fotile_debug.h"
#include "fotile_debug_order.h"


/* debug command part */
void cmd_restart_gateway_op(void)
{

}

void cmd_gateway_unicast_op(void)
{

}

void cmd_gateway_multicast_op(void)
{

}

void cmd_gateway_broadcast_op(void)
{

}

void cmd_turn_off_lora_service_op(void)
{

}

void cmd_turn_on_lora_service_op(void)
{

}

void cmd_cut_off_mqtt_link_op(void)
{

}

void cmd_con_mqtt_broker_op(void)
{

}

void cmd_send_mqtt_msg_op(void)							
{

}



/**
* @ Description: get system ram information
* @ return: void
*/
void info_get_sys_ram_op(void)
{
	int ret = -1;
	pid_t pid = getpid() ; 
	
	if (pid>0)
	{
		char filepath[32];
		memset(filepath, 0x00, sizeof(filepath));

		sprintf(filepath,"/proc/%d/status", pid );
		FILE *fp=fopen(filepath ,"r");

		if(fp)
		{
			CONSOLE_DBG_I("############ GW RAM INFO ############");
			
			char read_buf[MALLOC_SIZE];
			memset(read_buf, 0x00, sizeof(char)*MALLOC_SIZE);
		
			ret = fread(read_buf, sizeof(char)*MALLOC_SIZE, 1, fp);

			/*
			if(1 == ret)
			{
				CONSOLE_DBG_W("file not all read");
			}
			else if(0 == ret)
			{
				CONSOLE_DBG_I("file all read");
			}
			*/
			
			CONSOLE_DBG_I("\n%s", read_buf);
			
			fclose(fp);

			CONSOLE_DBG_I("########## GW RAM INFO END ##########");
		}
		else
		{
			CONSOLE_DBG_E("### OPEN RAM FILE ERROR");
		}
	}
	else
	{
		CONSOLE_DBG_E("### GET PID ERROR");
	}
}

/**
* @ Description: get system ipc information
* @ return: void
*/
void info_get_sys_ipc_op(void)
{
	//fputs("lscpu\r", stdout);
	CONSOLE_DBG_I("############ GW IPC INFO ############");
	int ret = system("ipcs -q | grep \"0x\"");

	if(0 > ret)
	{
		CONSOLE_DBG_E("### invalid order->ipcs -q | grep \"0x\" ");

	}
	CONSOLE_DBG_I("########## GW IPC INFO END ##########");
}