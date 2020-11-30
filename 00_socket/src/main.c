
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "log.h"

/***************************************************************
socket常用api

atoi	ascii->integer 字符串转整形，跳过空白字符（例如空格、tab缩进）等
		OUTPUT：不能转换成int或者为空字符串，那么将返回0；
				转换的数值超越int，返回-1或其他异常值。
				
htons	网络字节序：是TCP/IP中规定好的一种数据表示格式，它与具体的CPU类型、操作系统等无关，从而可以保证数据在不同主机之间传输时能够被正确解释，网络字节顺序采用big-endian排序方式。
		big-endian		|大尾顺序		|地址的低位存储值的高位
		little-endian	|小尾顺序		|地址的低位存储值的低位	
		htons：将一个无符号短整型(uint16_t)的主机数值转换为网络字节顺序，即大尾顺序(big-endian)
		包含有头文件<arpa/inet.h>，直译"Host to Network Short"
		多用于网络port设定，类似的还有：
		htonl()--"Host to Network Long"
		ntohl()--"Network to Host Long"
		htons()--"Host to Network Short"
		ntohs()--"Network to Host Short" 
		理解大尾顺序与小尾顺序：
		Intel x86结构下, short型数0x1234表示为34 12, int型数0x12345678表示为78 56 34 12  
		IBM power PC结构下, short型数0x1234表示为12 34, int型数0x12345678表示为12 34 56 78
		
inet_addr	点隔地址字符串->uint32的internet地址
			将一个点间隔地址转换成一个in_addr（uint32_t型数据）。
			如"192.168.1.1"转化后的十六进制为0x0101A8C0，为大尾顺序
			OUTPUT：入参不规范时，返回-1或其他异常值
			当IP地址为255.255.255.255是被认为无效IP地址
			
inet_ntoa	uint32的internet地址->点隔地址字符串，net->ascii，uint32_t转字符串
			当IP地址为255.255.255.255是认为有效IP地址，与inet_addr()不同

inet_aton	inet_ntoa反操作，入参为点隔字符串、internet地址
			ip4addr_aton(const char *cp, ip4_addr_t *addr)

inet_pton	int inet_pton(int af, const char *src, void *dst)
			这个函数转换字符串到网络地址，第一个参数af是地址族，转换后存在dst中
			inet_pton 是inet_addr的扩展，支持的多地址族有下列：   
			af = AF_INET	src为指向字符型的地址，即ASCII的地址的首地址（ddd.ddd.ddd.ddd格式的），函数将该地址  转换为in_addr的结构体，并复制在*dst中 
			af = AF_INET6	src为指向IPV6的地址，函数将该地址转换为in6_addr的结构体，并复制在*dst中   
			如果函数出错将返回一个负值，并将errno设置为EAFNOSUPPORT
			如果参数af指定的地址族和src格式不对，函数将返回0。   	
			（IPv4协议具有32位（4字节）地址长度；IPv6协议具有128位（16字节）地址长度）
***************************************************************/
void atoi_test(void)
{	
	LOG_I("atoi result = %d", atoi("ster")); // 不能转换，返回0
	LOG_I("atoi result = %d", atoi(""));	 // 为空，返回0
	LOG_I("atoi result = %d", atoi("12345678955321"));	// 越界，返回异常
	LOG_I("atoi result = %d", atoi("		123.123")); // 123
}

void htons_test(void)
{	
	LOG_I("htons result = %02x", htons(0x12)); 			// 返回0x1200
	LOG_I("htons result = %02x", htons(0x1214)); 		// 返回0x1412
	//LOG_I("htons result = %02x", htons(0x121415));	// 异常，返回0x1514
}


void inet_addr_test(void)
{
	LOG_I("inet_addr result = %d", inet_addr("192.168.1.1"));		// 0x0101A8C0
	LOG_I("inet_addr result = %d", inet_addr("255.255.255.255"));	// 返回-1，认为是无效地址
	LOG_I("inet_addr result = %d", inet_addr("192"));				// 返回其他异常值
	LOG_I("inet_addr result = %d", inet_addr("192.168"));			// 返回其他异常值
	LOG_I("inet_addr result = %d", inet_addr("192.1.1."));			// 返回-1
	LOG_I("inet_addr result = %d", inet_addr("192.168.1.1.1"));		// 返回-1
	LOG_I("inet_addr result = %d", inet_addr("192.1。1.1"));			// 返回-1
}

void inet_ntoa_test(void)
{
	struct in_addr addr;
	addr.s_addr = 0xFFFFFFFF;
	LOG_I("inet_ntoa result = %s", inet_ntoa(addr));	// 255.255.255.255
	addr.s_addr = 0x01020304;
	LOG_I("inet_ntoa result = %s", inet_ntoa(addr));	// 4.3.2.1，大尾顺序
}

void inet_aton_test(void)
{
	struct in_addr addr;
	
	inet_aton("255.255.255.255", &addr);
	LOG_I("inet_aton result = %02x", addr.s_addr);	// 0xFFFFFFFF

	inet_aton("4.3.2.1", &addr);
	LOG_I("inet_aton result = %02x", addr.s_addr);  // 0x01020304
}

//IPv4协议具有32位（4字节）地址长度；IPv6协议具有128位（16字节）地址长度
//IPv6 eg -> ABCD:EF01:2345:6789:ABCD:EF01:2345:6789
void inet_pton_test(void)
{
	struct in_addr addr_v4;
	inet_pton(AF_INET, "255.255.255.255", &addr_v4);
	LOG_I("inet_aton_IPv4 result = %02x", addr_v4.s_addr);	// 0xFFFFFFFF
	inet_pton(AF_INET, "4.3.2.1", &addr_v4);
	LOG_I("inet_aton_IPv4 result = %02x", addr_v4.s_addr);	// 0x01020304

	struct in6_addr addr_v6;
	inet_pton(AF_INET6, "ABCD:EF01:2345:6789:ABCD:EF01:2345:6789", &addr_v6);
	LOG_I("inet_aton_IPv6 result = ");	// 0x01020304
	for(int i=0;i<16;i++)
	{
		printf("%02x ", addr_v6.s6_addr[i]);
	}

	printf("\n");
}

int main(void)
{
	//atoi_test();
	//htons_test();
	//inet_addr_test();
	//inet_ntoa_test();
	//inet_aton_test();
	inet_pton_test();
	
	
#if 0
	int server_sockfd,client_sockfd;
	int server_len,client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
 
	unlink("server_socket");
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(5000);
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);
	while(1) {
		char ch;
		printf("server waiting\n");
 
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,
			(struct sockaddr *)&client_address, &client_len);
		read(client_sockfd, &ch ,1);
		ch++;
		write(client_sockfd, &ch, 1);
		close(client_sockfd);
	}
#endif
}

