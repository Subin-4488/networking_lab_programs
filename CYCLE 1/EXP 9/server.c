#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>

#define port 8080

struct sockaddr_in server, client;
int clientfd;
static int k = 0;
int sockfd;

int sec = 5;
int min = 31;
int hour = 7;

pthread_t t1, t2;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *setTime()
{

    while (1)
    {
        // if (hour < 9)
        // {
        //     printf("0%d:", hour);
        // }
        // else
        // {
        //     printf("%d:", hour);
        // }
        // if (min < 10)
        // {
        //     printf("0%d:", min);
        // }
        // else
        // {
        //     printf("%d:", min);
        // }
        // if (sec < 10)
        // {
        //     printf("0%d\n", sec);
        // }
        // else
        // {
        //     printf("%d\n", sec);
        // }

        sleep(1);
        pthread_mutex_lock(&lock);
        sec = (sec + 1) % 60;
        pthread_mutex_unlock(&lock);

        if (sec == 0)
        {
            pthread_mutex_lock(&lock);
            min = (min + 1) % 60;
            pthread_mutex_unlock(&lock);
            
            if (min == 0)
            {
                pthread_mutex_lock(&lock);
                hour = (hour + 1) % 13;
                pthread_mutex_unlock(&lock);
                
                if (hour == 0)
                {
                    pthread_mutex_lock(&lock);
                    hour = 1;
                    sec = 0;
                    min = 0;
                    pthread_mutex_unlock(&lock);
                }
            }
        }
    }
}

void initialize()
{
    sockfd = socket(SOCK_DGRAM, AF_INET, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&server, (socklen_t)sizeof(server)) < 0)
    {
        perror("Socket bind failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket bind successfull\n");
}

void *sendReceive()
{
    char msg[100];
    char retmsg[100];
    char th[5], tm[5], ts[5], temp[5];

    while (1)
    {
        int addrlen = sizeof(client);
        if (recvfrom(sockfd, msg, sizeof(char) * 99, 0, (struct sockaddr *)&client, (socklen_t *)&addrlen) > 0)
        {
            strcpy(retmsg, "TIME NOW: ");
            if (hour < 9)
            {
                strcpy(th, "0");
                sprintf(temp, "%d", hour);
                strcat(th, temp);
            }
            else
            {
                sprintf(th, "%d", hour);
            }
            if (min < 10)
            {
                strcpy(tm, "0");
                sprintf(temp, "%d", min);
                strcat(tm, temp);
            }
            else
            {
                sprintf(tm, "%d", min);
            }
            if (sec < 10)
            {
                strcpy(ts, "0");
                sprintf(temp, "%d", sec);
                strcat(ts, temp);
            }
            else
            {
                sprintf(ts, "%d", sec);
            }

            strcat(retmsg, th);
            strcat(retmsg, ":");
            strcat(retmsg, tm);
            strcat(retmsg, ":");
            strcat(retmsg, ts);

            printf("%s\n",retmsg);

            sendto(sockfd, retmsg, sizeof(char) * strlen(retmsg), 0, (struct sockaddr *)&client, (socklen_t)sizeof(client));
        }
    }
}

void main()
{
    initialize();

    pthread_create(&t1, NULL, setTime, NULL);
    pthread_create(&t2, NULL, sendReceive, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}