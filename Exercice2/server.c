#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define SERVER_MQ "/mqservername"
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int main()
{
	mqd_t server_mq;

	int seats[200];
	int available[200];

	for(int i = 0; i < 200; ++i) {
		seats[i] = i + 1;
		available[i] = 1;
	}

	struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
	
	server_mq = mq_open(SERVER_MQ, O_RDONLY | O_CREAT, 0660, &attr);

	char buffer[MSG_BUFFER_SIZE];

	while(1)
	{
		
		mq_receive(server_mq, buffer, MSG_BUFFER_SIZE, NULL);

		int seat = -1;
		for(int i = 0; i < 200; ++i) {
			if(available[i] == 1) {
				seat = seats[i];
				available[i] = 0;
				break;
			}
		}
		printf("Recveived message from client with mq %s, i sent him %d !\n", buffer, seat);

		mqd_t client_mq;
        client_mq = mq_open(buffer, O_WRONLY);
		sprintf(buffer, "%d", seat);
		mq_send(client_mq, buffer, strlen(buffer), 0);
		
		mq_close(client_mq);
	}

	mq_close(server_mq);
	mq_unlink(SERVER_MQ);
	
}