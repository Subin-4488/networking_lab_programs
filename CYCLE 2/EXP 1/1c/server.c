#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "head.h"

int frames[10];
static int q = 0;
static int l = 0;

int sockfd, clientfd;
struct sockaddr_in server, client;
int addrlen;

pthread_t t;

void initialize()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    addrlen = sizeof(server);

    if (bind(sockfd, (struct sockaddr *)&server, (socklen_t)addrlen) < 0)
    {
        perror("Binded Unsuccessful\n");
        exit(EXIT_FAILURE);
    }
    printf("Bind success\n");

    listen(sockfd, 5);
    clientfd = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&addrlen);
    if (clientfd < 0)
    {
        perror("Client Accept unsuccessfull\n");
        exit(EXIT_FAILURE);
    }
    printf("Client accepted successfully\n");
}

int receiveEntireWindow(int frameSize)
{
    char *msg = (char *)malloc(sizeof(char) * 10);
    for (int p = 0; p < frameSize; p++)
    {
        msg[0] = '\0';
        recv(clientfd, msg, sizeof(char) * 10, 0);
        if (msg[0] != '\0')
        {
            printf("Received frame: %s\n", msg);
            frames[q++] = atoi(msg);
        }
        else
        {
            exit(EXIT_SUCCESS);
        }
    }
    return 1;
}

int input()
{
    int n;
    printf("Enter 1 for +ve ACK or 0 for -veNAK for the FRAME %d\n", frames[l]);
    scanf("%d", &n);

    return n;
}

void *receiveAndAcknowledge()
{

    char *msg = (char *)malloc(sizeof(char) * 100);
    char *ack = (char *)malloc(sizeof(char) * 100);
    char *nak = (char *)malloc(sizeof(char) * 100);
    char *temp = (char *)malloc(sizeof(char) * 100);

    int windowSize;
    recv(clientfd, msg, sizeof(char) * 50, 0);
    windowSize = atoi(msg);

    strcpy(ack, "ACK");
    strcpy(nak, "NAK");

    int flag = 0;
    int nakFlag = 0;
    while (1)
    {
        msg[0] = '\0';

        if (!flag)
        {
            receiveEntireWindow(windowSize);
            flag = 1;
        }
        else
        {
            receiveEntireWindow(1);
        }

        if (input())
        { // ACK
            strcpy(ack, "ACK-");
            sprintf(temp, "%d", frames[l++]);
            strcat(ack, temp);
            send(clientfd, ack, sizeof(char) * strlen(ack), 0);
            nakFlag = 0;
        }
        else
        { // NAK
            strcpy(nak, "NAK-");
            sprintf(temp, "%d", frames[l]);
            strcat(nak, temp);
            send(clientfd, nak, sizeof(char) * strlen(nak), 0);
            nakFlag = 1;

            while (1)
            {
                recv(clientfd, msg, sizeof(char) * 10, 0);
                if (!input())
                {
                    strcpy(nak, "NAK-");
                    sprintf(temp, "%d", frames[l]); // msg=frames[l]
                    strcat(nak, temp);
                    send(clientfd, nak, sizeof(char) * strlen(nak), 0);
                }
                else
                {
                    strcpy(ack, "ACK-");
                    sprintf(temp, "%d", frames[l++]);
                    strcat(ack, temp);
                    send(clientfd, ack, sizeof(char) * strlen(ack), 0);
                    break;
                }
            }
        }

        sleep(1);
    }
}

void main()
{
    initialize();

    pthread_create(&t, NULL, receiveAndAcknowledge, NULL);
    pthread_join(t, NULL);
}