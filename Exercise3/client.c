#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "utils.h"
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char filepath[1000] , server_reply[MESSAGE_SIZE];
     
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    while(1)
    {
        printf("Enter file to be received: ");
        scanf("%s" , filepath);
         
        if(send(sock , filepath , strlen(filepath) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
 
        if(recv(sock , server_reply , sizeof(server_reply) , 0) < 0)
        {
            puts("recv failed");
            break;
        }

        if(strncmp(server_reply, FNE, strlen(FNE)) == 0) {
            printf("Server reply: ");
            printf("%s\n", server_reply);
            memset(server_reply, 0, sizeof(server_reply));          
        } else {

            FILE *f = fopen("hopekitty.jpg", "w+b");


            int messages_number = atoi(server_reply);
            memset(server_reply, 0, strlen(server_reply));

            printf("i shall wait for %d messages\n", messages_number);

            for(int i = 0; i < messages_number; ++i) {
                printf("receiving\n");
                if(recv(sock , server_reply , sizeof(server_reply) , 0) < 0)
                {
                    puts("recv failed");
                    break;
                }
                printf("writing\n");
                printf("ce am primit arre dimensiunea %d \n", strlen(server_reply));

                int bytes = fwrite(&server_reply, sizeof(char), sizeof(server_reply), f);
                printf("reseting\n");
                printf("am reusit sa scriu %d\n", bytes);

                memset(server_reply, 0, sizeof(server_reply));
                printf("%d\n", i);

            }
            
            fclose(f);

            printf("Server reply: File downloaded!\n");
        }
    }
     
    close(sock);
    return 0;
}