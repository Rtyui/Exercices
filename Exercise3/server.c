#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#include "utils.h"

struct params {
    int socket;
};

void* client_handler(void *params) {
    struct params p = *((struct params *) params);
    int read_size;
    char client_message[MESSAGE_SIZE];

    while( (read_size = recv(p.socket , client_message , sizeof(client_message) , 0)) > 0 )
    {
        char filepath[30];
        strcpy(filepath, client_message);
        memset(client_message, 0, strlen(client_message));

        FILE *f;
        printf("%s\n", filepath);
        f = fopen(filepath, "r");

        if(f) {
            fseek(f, 0, SEEK_END);
            long int total_size = ftell(f);
            fseek(f, 0, SEEK_SET);
            printf("total size is %d -- file is now at %d\n", total_size, ftell(f));

            int messages_number;
            long int last_chunk;

            if(total_size % MESSAGE_SIZE == 0) {
                last_chunk = MESSAGE_SIZE;
                messages_number = total_size / MESSAGE_SIZE;
            } else {
                last_chunk = total_size % MESSAGE_SIZE;
                messages_number = total_size / MESSAGE_SIZE + 1;
            }

            sprintf(client_message, "%d", messages_number);
            write(p.socket , client_message , strlen(client_message));
            memset(client_message, 0, strlen(client_message));

            printf("i will send %d messages containing file\n", messages_number);
            for(int i = 0; i < messages_number; ++i) {
                printf("reading\n");
                int bytes = fread(&client_message, sizeof(char), MESSAGE_SIZE, f);
                printf("sending\n");
                printf("read %d bytes\n", bytes);
                printf("actual size is %d bytes\n", ftell(f));
                printf("strlen is %d bytes\n", strlen(client_message));
                send(p.socket , &client_message, bytes, 0);
                printf("reseting mem\n");
                memset(client_message, 0, MESSAGE_SIZE);
                printf("%d\n", i);
                sleep(1);
            }

            fclose(f);
        } else {
            sprintf(client_message, "%s", FNE);
            write(p.socket , client_message , strlen(client_message));
        }

        memset(client_message, 0, strlen(client_message));
        sleep(2);
        printf("done\n");
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

    pthread_t threads[20];
    int active_threads = 0;

    while(1) {
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

        pthread_create(&threads[active_threads], NULL, &client_handler, &p);
        active_threads++;
    }
     
    return 0;
}