
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "log.h"

#include "my_socket.h"
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

gethostbyname	通过域名获取IP地址
				如果入参为域名，则会将域名解析后的IP存储在（struct hostent*）结构体中
				如果入参为ip，则会将原ip存储在（struct hostent*）结构体中

select	int select(int maxfdp, fd_set *readset, fd_set *writeset, fd_set *exceptset,struct timeval *timeout);
		入参说明：
		maxfdp：被监听的文件描述符的总数，它比所有文件描述符集合中的文件描述符的最大值大1，因为文件描述符是从0开始计数的；
		readfds、writefds、exceptset：分别指向可读、可写和异常等事件对应的描述符集合。
		timeout:用于设置select函数的超时时间，即告诉内核select等待多长时间之后就放弃等待。timeout == NULL 表示等待无限长的时间
		返回说明：
		超时返回0;失败返回-1；成功返回大于0的整数，这个整数表示就绪描述符的数目。

		文件描述符 
		int FD_ZERO(int fd,  fd_set *fdset);	//将fdset清零使集合中不含任何fd（一个 fd_set类型变量的所有位都设为 0）
		int FD_CLR(int fd,   fd_set *fdset);	//将fd从fdset集合中清除（清除某个位时可以使用）
		int FD_SET(int fd,   fd_set *fdset);	//将fd加入fdset集合（设置变量的某个位置位）
		int FD_ISSET(int fd, fd_set *fdset);	//检测fd是否在fdset集合中，不在则返回0（测试某个位是否被置位）

setsockopt
getsockopt
***************************************************************/
#define BUFFER_SIZE (1024)

const char* http_time = 
"POST /v5/time_sync/ HTTP/1.1\r\n"
"Host: api-test.fotile.com\r\n"
"Content-Type: application/json\r\n"
"X-DeviceType: 262\r\n"
"Access-Token: b0614d894192bca8d2fd28e2e17a890d\r\n"
"Connection: close\r\n"
"Content-Length: 2\r\n\r\n"
"{}";

const char* send_msg = "this is a test"; 

void atoi_test(void)
{	
	LOGI("atoi result = %d", atoi("ster")); // 不能转换，返回0
	LOGI("atoi result = %d", atoi(""));	 // 为空，返回0
	LOGI("atoi result = %d", atoi("12345678955321"));	// 越界，返回异常
	LOGI("atoi result = %d", atoi("		123.123")); // 123
}

void htons_test(void)
{	
	LOGI("htons result = %02x", htons(0x12)); 			// 返回0x1200
	LOGI("htons result = %02x", htons(0x1214)); 		// 返回0x1412
	//LOGI("htons result = %02x", htons(0x121415));	// 异常，返回0x1514
}

void inet_addr_test(void)
{
	LOGI("inet_addr result = %d", inet_addr("192.168.1.1"));		// 0x0101A8C0
	LOGI("inet_addr result = %d", inet_addr("255.255.255.255"));	// 返回-1，认为是无效地址
	LOGI("inet_addr result = %d", inet_addr("192"));				// 返回其他异常值
	LOGI("inet_addr result = %d", inet_addr("192.168"));			// 返回其他异常值
	LOGI("inet_addr result = %d", inet_addr("192.1.1."));			// 返回-1
	LOGI("inet_addr result = %d", inet_addr("192.168.1.1.1"));		// 返回-1
	LOGI("inet_addr result = %d", inet_addr("192.1。1.1"));			// 返回-1
}

void inet_ntoa_test(void)
{
	struct in_addr addr;
	addr.s_addr = 0xFFFFFFFF;
	LOGI("inet_ntoa result = %s", inet_ntoa(addr));	// 255.255.255.255
	addr.s_addr = 0x01020304;
	LOGI("inet_ntoa result = %s", inet_ntoa(addr));	// 4.3.2.1，大尾顺序
}

void inet_aton_test(void)
{
	struct in_addr addr;
	
	inet_aton("255.255.255.255", &addr);
	LOGI("inet_aton result = %02x", addr.s_addr);	// 0xFFFFFFFF

	inet_aton("4.3.2.1", &addr);
	LOGI("inet_aton result = %02x", addr.s_addr);  // 0x01020304
}

//IPv4协议具有32位（4字节）地址长度；IPv6协议具有128位（16字节）地址长度
//IPv6 eg -> ABCD:EF01:2345:6789:ABCD:EF01:2345:6789
void inet_pton_test(void)
{
	struct in_addr addr_v4;
	inet_pton(AF_INET, "255.255.255.255", &addr_v4);
	LOGI("inet_aton_IPv4 result = %02x", addr_v4.s_addr);	// 0xFFFFFFFF
	inet_pton(AF_INET, "4.3.2.1", &addr_v4);
	LOGI("inet_aton_IPv4 result = %02x", addr_v4.s_addr);	// 0x01020304

	struct in6_addr addr_v6;
	inet_pton(AF_INET6, "ABCD:EF01:2345:6789:ABCD:EF01:2345:6789", &addr_v6);
	LOGI("inet_aton_IPv6 result = ");	// 0x01020304
	for(int i=0;i<16;i++)
	{
		printf("%02x ", addr_v6.s6_addr[i]);
	}

	printf("\n");
}

void gethostbyname_test(const char* host)
{
	int i = 0;

	struct hostent* hostent_content = NULL;
	hostent_content = gethostbyname(host);
	LOGI("host name = %s", hostent_content->h_name);

	for(i=0; hostent_content->h_aliases[i]; i++)
	{
        LOGI("Aliases %d: %s", i+1, hostent_content->h_aliases[i]);
    }
	
	LOGI("Address type = %s", (hostent_content->h_addrtype==AF_INET)?"AF_INET":"AF_INET6");	// AF_INET AF_INET6
	LOGI("Address len  = %d", hostent_content->h_length);
	
	for(int i=0; hostent_content->h_addr_list[i]; i++)
	{
        LOGI("IP addr %d: %s", i+1, inet_ntoa(*(struct in_addr*)hostent_content->h_addr_list[i]));
    }


	char ipstr[16] = {0};
	struct in_addr in_addr;
	char** pptr = pptr = hostent_content->h_addr_list;
    in_addr.s_addr = *(uint32_t*)(*pptr);
    strcpy(ipstr, inet_ntoa(in_addr));
	LOGI("ipstr = %s", ipstr);
}

/*****
server: socket -> bind -> listen -> accept -> read write ... -> close

socket	函数对应于普通文件的打开操作，返回（文件描述字）套接字
		调用socket创建一个socket时，返回的socket描述字它存在于协议族（address family，AF_XXX）空间中，但没有一个具体的地址。如果想要给它赋值一个地址，就必须调用bind()函数，否则就当调用connect()、listen()时系统会自动随机分配一个端口

bind	赋予socket具体地址，对应AF_INET、AF_INET6就是把一个ipv4或ipv6地址和端口号组合赋给socket

listen	监听套接字

*****/
err_t tcp_server_test(const char* server_ip, const char* server_port)
{
	ERROR_CHECK((NULL == server_ip)||(NULL == server_port));

	int server_fd;
	int client_fd = -1;
	int opt = 1;
	uint8_t count = 5;	// 可以接收的数量限制
	uint8_t read_len;	// 单次recv的数据长度
	uint8_t data_len;	// recv的一次完整长度
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(struct sockaddr);
	
	char buffer[BUFFER_SIZE];
	int send_len = strlen(send_msg);

	// socket -> 创建socket套接字
	server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ERROR_CHECK(server_fd < 0);

	// 设置该socket关闭后、可重要，未加error_check
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{	
		LOGW("setsockopt failure");
		goto exit;
	}

	// bind -> 赋予socket具体地址
	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(atoi(server_port));
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{	
		LOGW("bind failure");
		goto exit;
	}
	LOGI("server ip_addr = %s,  port = %d", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
	
	if(listen(server_fd, count) < 0)
	{	
		LOGW("listen failure");
		goto exit;
	}

	fd_set readfds;
	fd_set writefds; // writefds是否有点多余
	fd_set exceptfds;
	struct timeval t;
	int select_result;
	
	while(1)
	{	
		// 重置缓存信息
		FD_ZERO(&readfds);
		FD_ZERO(&exceptfds);
		t.tv_sec  = 3;
		t.tv_usec = 0;
		read_len = 0;
		data_len = 0;
		memset(buffer, 0x00, sizeof(buffer));	
		
		FD_SET(server_fd, &readfds);
		FD_SET(server_fd, &writefds);
		FD_SET(server_fd, &exceptfds);

		select_result = select(server_fd+1, &readfds, &writefds, &exceptfds, &t); // 设置等待时间
		
		if(select_result <= 0) // 如果select是无限长等待，这里=0的判断有点多余
		{
			LOGW("select failure or timeout");
			continue;
		}

		// 注意accept的最后参数为传入传出参数，accept函数返回的时候、会被重新赋值
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
	
		if(client_fd < 0)
		{
			LOGW("failed to accept");
			continue;
		}
		else
		{ 
			LOGI("client ip_addr = %s,  port = %d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		}		
		
		if(FD_ISSET(server_fd, &readfds))
		{	
			//TODO recv如果为阻塞
			//TODO 如果client端没有关闭socket，则会陷入死循环,
			//do{
				read_len = recv(client_fd, buffer+data_len, BUFFER_SIZE-data_len, 0);
				data_len += read_len;
				LOGI("read_len = %d， data_len =%d", read_len, data_len);
				LOGI("buffer last = %c", buffer[data_len-1]);
			//}while(read_len > 0);
		}

		if(FD_ISSET(server_fd, &writefds))
		{
			// ...
		}

		if(FD_ISSET(server_fd, &exceptfds))
		{	
			//TODO 如果client端没有关闭socket，用while则会陷入死循环
			//do{
				read_len = recv(client_fd, buffer+data_len, BUFFER_SIZE-data_len, 0);
				data_len += read_len;
				LOGI("read_len = %d", read_len);
			//}while(read_len > 0);
		}

		if(data_len > 0)
		{
			LOGI("### recv_len = %d, buffer = \n%s", data_len, buffer);
			if(send_len == send(client_fd, send_msg, send_len, 0))
			{
				LOGW("message all sent, message = %s", send_msg);
			}
		}
		
		close(client_fd);
	}
	
exit:
	close(server_fd);
	return RES_FAIL;
}

err_t tcp_client_test(const char* host, const char* port)
{
	ERROR_CHECK((NULL == host)||(NULL == port));

	int client_fd;		// client socket的套接字
	int opt = 1;		// setsockopt的选项涉资
	struct sockaddr_in server_addr;		// server的地址信息
	//struct sockaddr_in client_addr;
	
	char buffer[BUFFER_SIZE];	// 存放http回复消息的缓存
	int read_len = 0;			// 从socket中读取的单次http数据长度
	int data_len = 0;			// 从socket中读取的http数据总长度

	struct hostent* hostent_content = NULL;
	hostent_content = gethostbyname(host);	// 域名转ip（host为ip也可运行）
	ERROR_CHECK(NULL == hostent_content);

	const char* server_ip = inet_ntoa(*(struct in_addr*)hostent_content->h_addr_list[0]);
	ERROR_CHECK(NULL == server_ip);
	LOGI("server_ip = %s", server_ip);
	
	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(atoi(port));
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	
	// socket -> 创建client socket套接字
	client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ERROR_CHECK(client_fd < 0);

	// 设置该socket关闭后、可重用
	if(setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{	
		LOGW("setsockopt failure");
		goto exit;
	}
	
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
		LOGW("connect failure");
		goto exit;
    }
	LOGI("connect sueecss");
	
	LOGI("http message = \n%s", http_time);
	int http_len = strlen(http_time);
	if(send(client_fd, http_time, http_len, 0) < 0)
	{
		LOGW("send failure");
		goto exit;
	}
	LOGI("send message sueecss");
	
#if 1
	// 文件描述符，设置等待事件，用select选择等待事件的来临
	fd_set readfds;
	struct timeval t;
	t.tv_sec  = 3;
	t.tv_usec = 0;
	
	FD_ZERO(&readfds);
	FD_SET(client_fd, &readfds);

	memset(buffer, 0x00, sizeof(buffer));
	// 这里只用于单一socket数据接收，收完则close
	// 返回值：超时返回0;失败返回-1；成功返回大于0的整数，这个整数表示就绪描述符的数目。
	int select_result = select(client_fd+1, &readfds, NULL, NULL, &t);

	if(select_result <= 0)
	{
		LOGW("select failure or timeout");
		goto exit;	
	}
	
	if(FD_ISSET(client_fd, &readfds))
	{	
		//LOGI("client_fd = %d, select result = %d", client_fd, select_result);
		//TODO 如果server端没有关闭socket，则会陷入死循环
		do{
			read_len = recv(client_fd, buffer+data_len, BUFFER_SIZE-data_len, 0);
			data_len += read_len;
		}while(read_len > 0);

		if(data_len > 0)
		{
			LOGW("date_len = %d, buffer = %s", data_len, buffer);			
		}
	}

#else // 带外事件

	memset(buffer, 0x00, sizeof(buffer));
	// 这里只用于单一socket数据接收，收完则close
	// 返回值：超时返回0;失败返回-1；成功返回大于0的整数，这个整数表示就绪描述符的数目。
	fd_set readfds;
	fd_set exceptfds;
	FD_ZERO(&readfds);
	FD_ZERO(&exceptfds);
	int select_result;
	
	struct timeval t;
	
	while(1) // 循环接收, 不close则一直处于接收状态
	{	
		t.tv_sec  = 3;
		t.tv_usec = 0;
		data_len = 0;	// 重置缓存信息
		read_len = 0;
		memset(buffer, 0x00, sizeof(buffer));		
	
		FD_SET(client_fd, &readfds);	// 置位
		FD_SET(client_fd, &exceptfds);
		
		//每次调用select之前都要重新在read_fds和exception_fds中设置文件描述符connfd，因为事件发生以后，文件描述符集合将被内核修改
		select_result = select(client_fd+1, &readfds, NULL, &exceptfds, &t);
		
		if(select_result <= 0)
		{
			LOGW("select failure or timeout");
			goto exit;	
		}

		if(FD_ISSET(client_fd, &readfds))
		{
			//LOGI("client_fd = %d, select result = %d", client_fd, select_result);
			//TODO 如果server端没有关闭socket，则会陷入死循环
			do{
				read_len = recv(client_fd, buffer+data_len, BUFFER_SIZE-data_len, 0);
				data_len += read_len;
			}while(read_len > 0);

			if(data_len > 0)
			{
				LOGW("date_len = %d, buffer = \n%s", data_len, buffer);			
			}
		}
		else if(FD_ISSET(client_fd,&exceptfds)) // 带外事件
		{
			//LOGI("client_fd = %d, select result = %d", client_fd, select_result);
			do{
				read_len = recv(client_fd, buffer+data_len, BUFFER_SIZE-data_len, MSG_OOB);
				data_len += read_len;
			}while(read_len > 0);
			
			if(data_len > 0)
			{
				LOGW("date_len = %d, buffer = \n%s", data_len, buffer);			
			}
		}
	}
	
#endif
exit:
	close(client_fd);
	return RES_FAIL;
}

