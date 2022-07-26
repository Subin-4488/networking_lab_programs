#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define port 8080

int sockfd, clientfd;
struct sockaddr_in server;

void initialize()
{

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
        return;
    }
    printf("Socket created successfully\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&server, (socklen_t)sizeof(server)) < 0)
    {
        perror("Connect failed\n");
        return;
    }
    printf("Connect success\n"); 

    char msg[100];
    while (1)
    {
        printf("********Enter the request:********\n");
        scanf("%s",msg);
        send(sockfd, msg, sizeof(int) * strlen(msg), 0);
        if (recv(sockfd, msg, sizeof(char) * 100, 0) > 0)
        {
            FILE *f=fopen(msg,"r");
            char temp[50];
            while (!feof(f)){
                fscanf(f,"%s",temp);
                printf("%s",temp);
            }
        }
        printf("\n\n");
    }
}

void main()
{
    initialize();
}