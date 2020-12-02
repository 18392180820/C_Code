#ifndef __MY_SOCKET_H__
#define __MY_SOCKET_H__

#include "log.h" 

#ifdef  __cplusplus
extern "C" {
#endif

void atoi_test(void);
void htons_test(void);
void inet_addr_test(void);
void inet_ntoa_test(void);
void inet_aton_test(void);
void inet_pton_test(void);
void gethostbyname_test(const char* host);


err_t tcp_server_test(const char* server_ip, const char* server_port);
err_t tcp_client_test(const char* client_ip, const char* client_port);


#ifdef __cplusplus
#endif

#endif





