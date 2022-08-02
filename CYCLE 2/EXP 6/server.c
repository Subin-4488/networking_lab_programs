#include "head.h"

struct sockaddr_in server;
int sockfd;
int clientfd[10];
static int k = 0;

void sendAck(int fd, int flag)
{
    if (flag)
        send(fd, OK, sizeof(char) * 20, 0);
    else
        send(fd, NOT_OK, sizeof(char) * 20, 0);
}

void initialize()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printError("Socket creation failed", 1);

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    server.sin_family = AF_INET;

    if (bind(sockfd, (struct sockaddr *)&server, (socklen_t)addrlen) < 0)
        printError("Bind failed", 1);

    listen(sockfd, 10);
}

void retrieve(int fd, char file[50], pid_t id)
{
    char msg[50];
    char command[50];
    char idx[10];
    char temp[80];

    printf("EXECUTING RETREIVE...\n");
    sleep(1);
    FILE *f;
    strcpy(command,"FILES/"); strcat(command,file);
    f = fopen(command, "r");
    if (f == NULL)
    {
        sendAck(fd, 0);
        printError("Error occured while retrieving the file", 0);
    }
    else
    {
        printf("FILE FOUND\n");
        printf("Processing...\n");
        sprintf(msg,"%ld",pthread_self());
        send(fd,msg,sizeof(char) * 20, 0); sleep(1);
        while (!feof(f))
        {
            fscanf(f, "%s", msg);
            send(fd, msg, sizeof(char) * 20, 0);
        }
        sendAck(fd, 1);
        return;
    }
}

void *manageClient(void *arg)
{

    char msg[50];

    printf("THREAD: %ld\n", pthread_self());
    int fd = *(int *)arg;

    while (1)
    {
        if (recv(fd, msg, sizeof(char) * 20, 0) < 0)
            sendAck(fd, 0);
        else
        {
            printf("FILE REQUESTED: %s\n", msg);
            retrieve(fd, msg, getpid());
        }
    }
}

void *acceptClient()
{
    while (1)
    {
        pthread_t t;
        clientfd[k++] = accept(sockfd, (struct sockaddr *)NULL, (socklen_t *)&addrlen);
        if (clientfd[k - 1] < 0)
            printError("Client accept failed", 1);

        pthread_create(&t, NULL, manageClient, &clientfd[k - 1]);
    }
}

void main()
{
    initialize();

    pthread_t t;
    pthread_create(&t, NULL, acceptClient, NULL);
    pthread_join(t, NULL);
}
