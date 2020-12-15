#ifndef __MY_MATH_H__
#define __MY_MATH_H__

/* config file for debug, like dev_config.h in lora gateway code */

#include "log.h"

#ifdef  __cplusplus
extern "C" {
#endif

int16_t find_mode(int16_t* data, uint16_t len);
void find_mode_test(void);

#ifdef __cplusplus
#endif

#endif





