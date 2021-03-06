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

static void delay_test(void)
{
	while(1)
	{
		LOGI("sleep test");
		sleep(5); // ok,是5s
	}	
}

#define SERVER (1)

int main(void)
{	
	//delay_test();
	//atoi_test();
	//htons_test();
	//inet_addr_test();
	//inet_ntoa_test();
	//inet_aton_test();
	//inet_pton_test();
	
	//gethostbyname_test("baidu.com");
	//gethostbyname_test("api-test.fotile.com");
	//gethostbyname_test("10.49.2.153");
	
#if SERVER
	tcp_server_test("127.123.0.1", "8031");
#else
	tcp_client_test("127.123.0.1", "8031");
	//tcp_client_test("api-test.fotile.com", "8080");
	//tcp_client_test("10.49.2.153", "80");
	//tcp_client_test("0.0.0.0", "5000");
#endif

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

	LOGI("server ip_addr = %s,  port = %d", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
	char buf[1024];
	while(1) {
		//char ch;
		
		//memset(buf, 0x00, sizeof(buf));
		printf("server waiting\n");
 
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);
		//read(client_sockfd, buf ,100);
		recv(client_sockfd, buf, 1024, 0);
		LOGI("buff = \n%s", buf);
		//ch++;
		//write(client_sockfd, &ch, 1);
		close(client_sockfd);
	}
#endif
}

