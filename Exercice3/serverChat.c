#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#include "utils.h"

void* message_handler(void *params) {
    struct params p = *((struct params *) params);
    int read_size;
    char client_message[MESSAGE_SIZE];

    while( (read_size = recv(p.socket , client_message , sizeof(client_message) , 0)) > 0 )
    {
        printf("%sClientu': %s", KRED, client_message);
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
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
    char buff[200];
    
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    listen(socket_desc , 3);

    pthread_t thread;

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    struct params p;
    p.socket = client_sock;

    pthread_create(&thread, NULL, &message_handler, &p);
    clear();
    printf("- Chat started -\n");
    while(1) {

        fgets(buff, sizeof(buff), stdin);

        if(strncmp(buff, "clear", 5) == 0) {
            clear();
            continue;
        }
         
        if(send(client_sock , buff , strlen(buff) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        move_console_cursor_up(1);
        printf("\r%sYou: %s", KBLU, buff);
        memset(buff, 0, sizeof(buff));
    }
     
    return 0;
}