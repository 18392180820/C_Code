#ifndef _DEBUGLOG_H
#define _DEBUGLOG_H
 
#define ESC_START     "\033["
#define ESC_END       "\033[0m"
#define COLOR_FATAL   "31;40;5m"
#define COLOR_ALERT   "31;40;1m"
#define COLOR_CRIT    "31;40;1m"
#define COLOR_ERROR   "31;40;1m"
#define COLOR_WARN    "33;40;1m"
#define COLOR_NOTICE  "34;40;1m"
#define COLOR_INFO    "32;40;1m"
#define COLOR_DEBUG   "36;40;1m"
#define COLOR_TRACE   "37;40;1m"
 
#define Msg_Info(format, args...) (printf( ESC_START COLOR_INFO "[INFO]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##args))
 
#define Msg_Debug(format, args...) (printf( ESC_START COLOR_DEBUG "[DEBUG]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##args))
 
#define Msg_Warn(format, args...) (printf( ESC_START COLOR_WARN "[WARN]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##args))
 
#define Msg_Error(format, args...) (printf( ESC_START COLOR_ERROR "[ERROR]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##args))


#define		LOG_TAG	"fotile_console.c"
#define		CONSOLE_DBG_I(format, x...) printf("[**** %s: line: %d]:" format "\r\n", LOG_TAG, __LINE__, ##x);
#define		CONSOLE_DBG_E(format, x...) printf("[$$$$$$$$ %s: %s() line: %d]:" format "\r\n", LOG_TAG, __FUNCTION__, __LINE__, ##x);
#define		CONSOLE_DBG_W(format, x...) printf("[######## %s: %s() line: %d]:" format "\r\n", LOG_TAG, __FUNCTION__, __LINE__, ##x);
#define		CONSOLE_DBG_B(format, x...) printf("\n");

#endif

