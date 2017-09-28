#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <graphics.h>

#include "utils.h"

void* message_handler(void *params) {
    struct params p = *((struct params *) params);
    int read_size;
    char client_message[MESSAGE_SIZE];

    while( (read_size = recv(p.socket , client_message , sizeof(client_message) , 0)) > 0 )
    {
        printf("%sServeruSmecher: %s", KRED, client_message);
        memset(client_message, 0, sizeof(client_message));
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
}
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char buff[1000] , server_reply[MESSAGE_SIZE];
    pthread_t thread;

    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("10.2.36.13");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");

    struct params p;
    p.socket = sock;
    
    pthread_create(&thread, NULL, &message_handler, &p);
    clear();
    printf("- Chat started -\n");
    while(1)
    {

        fgets(buff, sizeof(buff), stdin);

        if(strncmp(buff, "clear", 5) == 0) {
            clear();
            continue;
        }
         
        if(send(sock , buff , strlen(buff) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        move_console_cursor_up(1);
        printf("\r%sYou: %s", KBLU, buff);
        memset(buff, 0, sizeof(buff));
    }
     
    close(sock);
    return 0;
}