#ifndef __LOG_H__
#define __LOG_H__

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

/******************************************************************************************************************************
\033[显示方式;前景色;背景色m
显示方式 :0（默认值）、1（高亮）、22（非粗体）、4（下划线）、24（非下划线）、5（闪烁）、25（非闪烁）、7（反显）、27（非反显）
前景色:30（黑色）、31（红色）、32（绿色）、 33（黄色）、34（蓝色）、35（洋红）、36（青色) 、37（白色）
背景色:40（黑色）、41（红色）、42（绿色）、 43（黄色）、44（蓝色）、45（洋红）、46（青色）、47（白色）
\033[0m 默认
\033[1;32;40m 绿色
033[1;31;40m 红色

printf( "\033[1;31;40m 输出红色字符 \033[0m");
******************************************************************************************************************************/

#define LOG_LEVEL_CONFIG	(2)

typedef enum {
    LOG_NONE = 0,   /*!< No log output */
    LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    LOG_INFO,       /*!< Information messages which describe normal flow of events */
    LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    LOG_VERBOSE,    /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
    
    LOG_MAX
} log_level_t;


#define ESC_START     "\033["
#define ESC_END       "\033[0m"
#define COLOR_INFO    "0;32m"
#define COLOR_WARN    "0;33m"   
#define COLOR_ERROR   "0;31m"

#define LOG_OUTPUT(level, tag, format, ...) do{\
		if(level == LOG_INFO)		{printf(ESC_START COLOR_INFO  "[I %s:%s<%d>]:" format ESC_END "\r\n", tag, __FUNCTION__, __LINE__, ##__VA_ARGS__);}\
		else if(level == LOG_WARN)	{printf(ESC_START COLOR_WARN  "[W %s:%s<%d>]:" format ESC_END "\r\n", tag, __FUNCTION__, __LINE__, ##__VA_ARGS__);}\
		else if(level == LOG_ERROR)	{printf(ESC_START COLOR_ERROR "[E %s:%s<%d>]:" format ESC_END "\r\n", tag, __FUNCTION__, __LINE__, ##__VA_ARGS__);}\
}

#define LOG_LEVEL(level, tag, format, ...) do{\
		if(level <= LOG_LEVEL_CONFIG) LOG_OUTPUT(level, tag, format, ##__VA_ARGS__)\
		}while(0)

#define LOG_I(tag, format, ...) LOG_LEVEL(LOG_INFO,  tag, format, ##__VA_ARGS__)
#define LOG_W(tag, format, ...) LOG_LEVEL(LOG_WARN,  tag, format, ##__VA_ARGS__)
#define LOG_E(tag, format, ...) LOG_LEVEL(LOG_ERROR, tag, format, ##__VA_ARGS__)


#ifdef __cplusplus
#endif

#endif




