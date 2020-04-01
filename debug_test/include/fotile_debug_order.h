#ifndef __FOTILE_DEBUG_ORDER_H__
#define __FOTILE_DEBUG_ORDER_H__


#ifdef  __cplusplus
extern "C" {
#endif


#define 	MALLOC_SIZE			(2048)

/* debug - cmd function */
extern void cmd_restart_gateway_op(void);
extern void cmd_gateway_unicast_op(void);
extern void cmd_gateway_multicast_op(void);
extern void cmd_gateway_broadcast_op(void);	
extern void cmd_turn_off_lora_service_op(void);
extern void cmd_turn_on_lora_service_op(void);
extern void cmd_cut_off_mqtt_link_op(void);
extern void cmd_con_mqtt_broker_op(void);
extern void cmd_send_mqtt_msg_op(void);		


/* debug - info function */
extern void info_get_sys_ram_op(void);
extern void info_get_sys_ipc_op(void);

#endif