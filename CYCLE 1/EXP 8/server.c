#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define port 8080

int sockfd, clientfd;
struct sockaddr_in server, client;

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
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&server, (socklen_t)sizeof(server)) < 0)
    {
        printf("Bind failed\n");
        return;
    }

    printf("Bind success\n");

    listen(sockfd, 5);
    printf("listening...\n");
    int len = sizeof(client);
    clientfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&len);
    if (clientfd < 0)
    {
        printf("Accept failed\n");
        return;
    }

    printf("Accept success\n");

    while (1)
    {
        char msg[100];
        if (recv(clientfd, msg, sizeof(char) * 100, 0) > 0)
        {
            printf("%s\n", msg);
        }
        char comm[50];
        strcpy(comm, "ping -c 5 ");
        strcat(comm, msg);
        strcat(comm, " > command.dat");
        system(comm);

        send(clientfd, "command.dat", sizeof("command.dat"), 0);
        printf("Request processed successfully\n");
    }
}

void main()
{
    initialize();
}