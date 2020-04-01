/******************************************************************************
 * @file    fotile_console.h
 * @author  Yugk
 * @version V1.0.0
 * @date    20-03-2020
 * @brief   common api of esp_wifi module
******************************************************************************/

#ifndef __FOTILE_DEBUG_H__
#define __FOTILE_DEBUG_H__

/* config file for debug, like dev_config.h in lora gateway code */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>

#ifdef  __cplusplus
extern "C" {
#endif

/******************************************************
*                      DEBUG PRINT FORMAT
******************************************************/

#define ESC_START     "\033["
#define ESC_END       "\033[0m"
#define COLOR_INFO    "32;40;1m"
#define COLOR_ERROR   "31;40;1m"
#define COLOR_WARN    "33;40;1m"   

#define	DEBUG_INPUT_DELAY		(50000)
#define DEBUG_OK 				(0)		//(SINT32)0
#define DEBUG_ERROR 			(-1)	//(SINT32)-1


#define	CONSOLE_DEBUG_TAG	"fotile_debug.c"
#define	CONSOLE_DBG_I(format, x...) printf(ESC_START COLOR_INFO  "[INFO  %s: line: %d]:" format ESC_END "\r\n", CONSOLE_DEBUG_TAG, __LINE__, ##x);
#define	CONSOLE_DBG_E(format, x...) printf(ESC_START COLOR_ERROR "[ERROR %s: %s() line: %d]:" format ESC_END "\r\n", CONSOLE_DEBUG_TAG, __FUNCTION__, __LINE__, ##x);
#define	CONSOLE_DBG_W(format, x...) printf(ESC_START COLOR_WARN  "[WARN  %s: %s() line: %d]:" format ESC_END "\r\n", CONSOLE_DEBUG_TAG, __FUNCTION__, __LINE__, ##x);
#define	CONSOLE_DBG_B(format, x...) printf("\n");

/******************************************************
*                      Macros
******************************************************/
/*****************************************************************************
 ************   choose whether gateway in debug mode or not   ****************
 *****************************************************************************/
#define GATEWAY_DEBUG_SWITCH			(1)
#define DUBUG_EXIT						"exit"
/***********************   define debug cmd/info prefix   *************************/
#define DEBUG_CMD_PREFIX				"cmd"
#define DEBUG_INFO_PREFIX				"info"    
#define DEBUG_CMD_MODE					(1)
#define DEBUG_INFO_MODE					(2)
#define DEBUG_ERROR_MODE				(-1)

/*******************************************************************************
**************   commands below for debug command input to gateway  ************
?* 
?* 
?* 
********************************************************************************/

#define DUBUG_CMD_REBOOT_DEV			"reboot"					//reboot gateway device
#define DEBUG_CMD_UNICAST				"unicast"					//unicast:   point to point
#define DEBUG_CMD_MULTICAST				"multicast"					//multicast: one to group
#define DEBUG_CMD_BROADCAST				"broadcast"					//broadcast: one to all

#define DEBUG_CMD_LORA_OFF				"lora_off"					//turn off gateway lora service
#define DEBUG_CMD_LORA_ON				"lora_on"					//turn on  gateway lora service

#define DEBUG_CMD_MQTT_OFF				"mqtt_off"					//cut off from mqtt broker actively
#define DEBUG_CMD_MQTT_CON				"mqtt_con"					//connect mqtt broker actively
#define DEBUG_CMD_MQTT_SEND				"mqtt_send"					//send mqtt message actively for debug

//TODO: need to confirm command format



/*******************************************************************************
****************   commands below for information output   *********************
?*   input message imes -ft_sf_ver\gw_sys_stat\gw_sys_stat\nd_con_stat\
?*   outdev_work_stat\task_list\mq_stat to the uart, and gateway will print 
?*   the information as below separately
********************************************************************************/

#define DUBUG_INFO_FT_SF_VER			"sw_version"					//check software version

/* ram/ipc lists system values of gateway */
#define	DUBUG_INFO_GW_RAM_STAT			"ram"
#define DUBUG_INFO_GW_IPC_STAT			"ipc"
//#define DUBUG_INFO_SYS_VERSION		"sys_version"				//check system version
//#define DUBUG_INFO_SYS_UPTIME			"sys_uptime"				//check system running time
//#define DUBUG_INFO_SYS_CPU_USED		"sys_cpu_used"				//check system rate of cpu utilization
//TODO:add more sys_condition needed



/* nd_stat contains node num statistics, and lists the time when node is online\offline */
#define DUBUG_INFO_ND_STAT				"nd_stat"
#define DUBUG_INFO_ND_JIONED_NUM		"nd_joined_num"				//check total joined node num
#define DUBUG_INFO_ND_REGISTERED_NUM	"nd_reged_num"				//check registered node num
#define DUBUG_INFO_ND_ONLINE_NUM		"nd_online_num"				//check online node num
#define DUBUG_INFO_ND_ONLINE_TIME		"nd_online_time"			//ckeck the time when node is online
#define DUBUG_INFO_ND_OFFLINE_TIME		"nd_offline_time"			//check the time when node id offline

/* nd_con_stat lists RSSI and SNR information of all online nodes */
#define DUBUG_INFO_ND_CON_STAT			"nd_con_stat"						
#define DUBUG_INFO_ND_CON_RSSI			"nd_con_rssi"				//check node connection rssi
#define DUBUG_INFO_ND_CON_SNR			"nd_con_snr"				//check node message snr

/* outdev_work_stat contains outdoor device working status, such as fan speed\fan gear\boot rate*/
#define DUBUG_INFO_OUTDEV_WORK_STAT		"outdev_work_stat"
#define DUBUG_INFO_OUTDEV_RUNNING		"outdev_running"			//check whether outdoor device is running
#define DUBUG_INFO_OUTDEV_FAN_SPEED		"outdev_fan_speed"			//check outdoor device fan speed
#define DUBUG_INFO_OUTDEV_FAN_SPEED		"outdev_fan_speed"			//check outdoor device fan speed
#define DUBUG_INFO_OUTDEV_FAN_GEAR		"outdev_fan_gear"			//check outdoor device fan gear
#define DUBUG_INFO_OUTDEV_BOOT_RATE		"outdev_boot_rate"			//check outdoor device boot rate

/* task_list lsits the status of thread tasks, like thread name\created number\priority\stack size\running state */
#define DUBUG_INFO_TASK_LIST			"task_list"
#define DUBUG_INFO_TASK_NAME			"task_name"					//check thread task name
#define DUBUG_INFO_TASK_NUM				"task_num"					//check thread task created num
#define DUBUG_INFO_TASK_PRI				"task_pri"					//check thread task priority
#define DUBUG_INFO_TASK_STACK           "task_stack"				//check thread task stack size
#define DUBUG_INFO_TASK_STAT			"task_stat"					//check thread task running state(B\R\D\S)

/* mq_stat lists information of message queue created in uart\lora\mqtt */
#define	DUBUG_INFO_MQ_STAT				"mq_stat" 
#define DUBUG_INFO_UART_MQ				"mq_uart"					//UART MQ
#define	DUBUG_INFO_LORA_MQ				"mq_lora"					//LORA MQ	
#define	DUBUG_INFO_MQTT_MQ				"mq_mqtt"					//MQTT MQ
#define DUBUG_INFO_MQ_SIZE				"mq_size"					//total size of single MQ	
#define DUBUG_INFO_MQ_USED				"mq_used"					//used size of single MQ
#define DUBUG_INFO_MQ_LEFT				"mq_left"					//left size of single MQ

/* wifi_stat lists wifi connection state of gateway module */
#define	DUBUG_INFO_WIFI_CON_STAT		"wifi_con_stat" 
#define	DUBUG_INFO_WIFI_CON_RSSI		"wifi_con_rssi" 
//TODO:add more wifi_con_stat needed

/******************************************************
*               Function Declarations
******************************************************/

/**
* @ Description: start debug cycle
*/
extern int create_debug_handler(int interval);
extern int create_debug_adb_handler(void);

/************************ END OF USER CODE ************************/
#ifdef __cplusplus
#endif

#endif



