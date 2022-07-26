#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include "port.h"

int addrlen;
int sockfd;
struct sockaddr_in server, client;

static int k = 0;
pthread_t t1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct phillosopher
{
    pthread_t tp;
    int clientfd;
    int id;
    int leftflag, rightflag;
    int eat;
    int think;
    int remtime;
} * p;
struct chopstick
{
    int id;
    int flag;
} * c;

char msg[100];

void initialize()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed\n");
    }
    printf("Socket created successfully\n");

    addrlen = sizeof(server);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&server, (socklen_t)addrlen) < 0)
    {
        perror("Bind failed\n");
    }
    printf("Bind successful\n");

    listen(sockfd, 10);
}

void *allocate(void *arg)
{

    struct phillosopher *ph = (struct phillosopher *)arg;

    char eat[10] = "EATING";
    char think[10] = "THINKING";
    char wait[10] = "WAITING";

    while (ph->remtime > 0)
    {
        if (!c[ph->id].flag && !c[((ph->id) + 1) % 5].flag)
        {
            pthread_mutex_lock(&lock);
            c[ph->id].flag = 1;             // grab left
            c[((ph->id) + 1) % 5].flag = 1; // grab right
            pthread_mutex_unlock(&lock);

            printf("EATING: %d, REM-TIME %d\n", ph->id, ph->remtime);
            send(ph->clientfd, eat, sizeof(char) * 10, 0);

            ph->eat = 1; // eat
            ph->think = 0;

            sleep(5);

            ph->remtime -= 5;
            c[ph->id].flag = 0;             // release left
            c[((ph->id) + 1) % 5].flag = 0; // release right

            ph->eat = 0;
            ph->think = 1; // think

            pthread_mutex_unlock(&lock);
            sleep(1);
        }
        else
        {
            printf("THINK: %d\n", p->id);
            send(ph->clientfd, think, sizeof(char) * 10, 0);
            sleep(1);
        }
    }

    if (ph->remtime == 0)
    {
        printf("COMPLETED: %d\n",ph->id);
        send(ph->clientfd, "COMPLETED", sizeof(char) * strlen("COMPLETED"), 0);
        pthread_cancel(ph->tp);
    }
}

void *acceptConn()
{
    for (int i = 0; i < 5; i++)
    {
        p[i].clientfd = accept(sockfd, (struct sockaddr *)NULL, (socklen_t *)NULL);
        printf("Phillosopher %d accepted\n", p[i].id);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&p[i].tp, NULL, allocate, &p[i]);
    }
}

void main()
{

    c = (struct chopstick *)malloc(sizeof(struct chopstick) * 5);
    for (int i = 0; i < 5; i++)
    {
        c[i].id = i;
        c[i].flag = 0;
    }

    p = (struct phillosopher *)malloc(sizeof(struct phillosopher) * 5);
    for (int i = 0; i < 5; i++)
    {
        p[i].id = i;
        p[i].leftflag = 0;  // not allocated the left fork
        p[i].rightflag = 0; // not allocated the right fork
        p[i].eat = 0;
        p[i].think = 1;
        p[i].remtime = 10;
    }

    initialize();

    pthread_create(&t1, NULL, acceptConn, NULL);

    pthread_join(t1, NULL);

    for (int i = 0; i < 5; i++)
        pthread_join(p[i].tp, NULL);
    close(sockfd);
}