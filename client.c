#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "file.h"

#define LENGTH 2048

volatile sig_atomic_t flag = 0;
int sockfd = 0;

void str_overwrite_stdout()
{
	printf("%s", "> ");
	fflush(stdout);
}

void str_trim_lf(char *arr, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{ // trim \n
		if (arr[i] == '\n')
		{
			arr[i] = '\0';
			break;
		}
	}
}

void catch_ctrl_c_and_exit(int sig)
{
	flag = 1;
}

/* 发送数据处理 */
void send_msg_handler()
{
	char message[LENGTH] = {};
	char buffer[LENGTH + 32] = {};

	while (1)
	{
		str_overwrite_stdout();
		fgets(message, LENGTH, stdin);
		str_trim_lf(message, LENGTH);

		if (strcmp(message, "exit") == 0)
		{
			break;
		}
		else
		{
			sprintf(buffer, "%s: %s\n", name, message);
			send(sockfd, buffer, strlen(buffer), 0);
		}

		bzero(message, LENGTH);
		bzero(buffer, LENGTH + 32);
	}
	catch_ctrl_c_and_exit(2);
}

/* 接受数据处理 */
void recv_msg_handler()
{
	char message[LENGTH] = {};
	while (1)
	{
		int receive = recv(sockfd, message, LENGTH, 0);
		if (receive > 0)
		{
			printf("%s", message);
			str_overwrite_stdout();
		}
		else if (receive == 0)
		{
			break;
		}
		memset(message, 0, sizeof(message));
	}
}

int main(int argc, char **argv)
{

	//接受一个输入进来的端口号
	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *ip = "127.0.0.1";
	int port = atoi(argv[1]);
	int n = 0;

	signal(SIGINT, catch_ctrl_c_and_exit);


	printf("登录请输入1，注册输入0 :\n");
	scanf("%d", &n);
	if (n == 0)
	{
		registered();
	} 
	else 
	{
		login();
	}
	

	struct sockaddr_in server_addr;

	//IP协议，端口，IP地址设置 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);

	// 连接到服务器
	int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (err == -1)
	{
		printf("ERROR: connect\n");
		return EXIT_FAILURE;
	}

	//发送姓名 
	send(sockfd, name, 32, 0);

	printf("----- 欢迎进入聊天室 -----n");


	//创建一个线程发送数据
	pthread_t send_msg_thread;
	if (pthread_create(&send_msg_thread, NULL, (void *)send_msg_handler, NULL) != 0)
	{
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

	//创建一个线程接收数据
	pthread_t recv_msg_thread;
	if (pthread_create(&recv_msg_thread, NULL, (void *)recv_msg_handler, NULL) != 0)
	{
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

	while (1)
	{
		if (flag)
		{
			printf("\nBye\n");
			break;
		}
	}

	close(sockfd);

	return EXIT_SUCCESS;
}
