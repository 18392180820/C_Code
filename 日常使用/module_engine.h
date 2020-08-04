//****************************************************************************/
// @file 		module_engine.h
// @author:		cairj
// @version:	V1.0.0
// @date 		2019/04/18
//****************************************************************************/
/*
* Copyright (c) 2018, FOTILE GROUP.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*  - Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*  - Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of the FOTILE GROUP nor the
*    names of its contributors may be used to endorse or promote products
*    derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL FOTILE GROUP BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef MODULE_ENGINE_H
#define MODULE_ENGINE_H

#define SUCCESS 0
#define FAILURE -1

#define COMMON_API
#define OUT		//表示出参

typedef enum ACCESS_MODE
{
	READ_WRITE,
	READ_ONLY,
	WRITE_ONLY,
	ACCESS_MODE_NUM
}E_ACCESS_MODE;

/**
* 目前的引擎代码中由于cJSON库的限制只能支持最多4字节的属性值，当前情况已经够用，
* 如果需要支持大于4字节的，则需要更新引擎代码以及协议上对属性的赋值定义
*/

/**
* @ Description: 
* @ param file: 	config file path 
* @ return: 		load result
*/
COMMON_API int load_config_file(const char* file);

/**
* @ Description: 
* @ return: 
*/
COMMON_API int destroy();

/**
* @ Description:	convert json data  to uart hex 
* @ param json_data: 		
* @ param uart_hex: 
* @ return: 		convert result
*/
COMMON_API int json_to_hex(const char* json_data, OUT char* uart_hex);

/**
* @ Description: 	convert uart hex to json data
* @ param: 
* @ param: 
* @ return: 		convert result
*/
COMMON_API int hex_to_json(const char* uart_hex, OUT char* json_data);

#endif /* MODULE_ENGINE_H */

