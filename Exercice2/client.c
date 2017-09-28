#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <mqueue.h>

#define SERVER_MQ "/mqservername"
#define CLIENT_MQ "/mqclientname"
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int main()
{
	mqd_t server_mq;
	mqd_t client_mq;
	char buffer[MSG_BUFFER_SIZE];

	struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

	client_mq = mq_open(SERVER_MQ, O_CREAT | O_RDONLY, 0660, &attr);
	server_mq = mq_open(SERVER_MQ, O_WRONLY);

	sprintf(buffer, CLIENT_MQ);
	printf("Starting send!!\n");
	mq_send(server_mq, buffer, strlen(buffer), 0);
	printf("Client: Sent Request!\n");
	mq_close(server_mq);

	mq_receive(client_mq, buffer, MSG_BUFFER_SIZE, NULL);

	printf("I have received %s !!\n", buffer);
	
	mq_close(client_mq);
	mq_unlink(CLIENT_MQ);
}