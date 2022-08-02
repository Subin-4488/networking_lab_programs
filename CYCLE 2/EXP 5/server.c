#include "head.h"

struct sockaddr_in server;
int sockfd;
int clientfd[10];
static int k = 0;

void sendAck(int fd, int flag)
{
    if (flag)
        send(fd, OK, sizeof(char) * 6, 0);
    else
        send(fd, NOT_OK, sizeof(char) * 6, 0);
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

void store(int fd, char user[50], int k)
{
    char msg[50];
    char command[50];
    char idx[30];
    char dir[50];
    if (recv(fd, msg, sizeof(char) * 50, 0) < 0)
        sendAck(fd, 0);
    else
    {
        strcpy(dir, "mkdir -p FILES/");
        strcat(dir, user);
        printf("EXECUTING STORE...\n");
        system(dir);
        sleep(1);
        sprintf(idx, "%d", k);
        strcpy(command, "cp ");
        strcat(command, msg);
        strcat(command, " FILES/");
        strcat(command, user);
        strcat(command, "/");
        strcat(command, user);
        strcat(command, idx);
        strcat(command, msg);
        system(command);
        sendAck(fd, 1);
        printf("UPLOAD Success\n");
    }
}

void retrieve(int fd, char user[50], int n)
{
    char msg[50];
    char command[50];
    char idx[10];
    char temp[80];
    if (recv(fd, msg, sizeof(char) * 50, 0) < 0) // receive filename
    {
        sleep(1);
        sendAck(fd, 0);
        return;
    }
    else
    {
        printf("EXECUTING RETREIVE...\n");
        sleep(1);
        strcpy(command,"FILES/");  strcat(command,user); strcat(command,"/"); strcat(command,msg);
        FILE *f;
        f = fopen(command, "r");
        if (f == NULL){
            sendAck(fd,0);
            printError("Error occured while retrieving the file", 0);
        }
        else
        {
            system("mkdir -p Downloads");
            strcpy(temp, "cp FILES/");
            strcat(temp,user); strcat(temp,"/");
            strcat(temp, msg);
            strcat(temp, " Downloads/");
            strcat(temp,"downloaded_");
            strcat(temp, msg);
            system(temp);
            sendAck(fd,1);
            return;
        }
    }
}

void *manageClient(void *arg)
{
    char msg[50];
    struct clientData c;
    int fd = *(int *)arg;
    if (recv(fd, (struct clientData *)&c, sizeof(struct clientData), 0) < 0) // accept user struct data
    {
        sleep(1);
        sendAck(fd, 0);
    }
    else
    {
        sleep(1);
        sendAck(fd, 1);
    }

    int storeIDX = 1;

    while (1)
    {
        if (recv(fd, msg, sizeof(char) * 50, 0) < 0)
            sendAck(fd, 0);
        else
        {
            if (!strcmp(msg, STOR))
            {
                sendAck(fd, 1);
                store(fd, c.username, storeIDX++);
            }
            else if (!strcmp(msg, RETR))
            {
                sendAck(fd, 1);
                retrieve(fd, c.username, storeIDX);
            }
            else
            {
                sendAck(fd, 0);
            }
        }
    }
}

void *acceptClient()
{
    pthread_t t;
    while (1)
    {
        clientfd[k++] = accept(sockfd, (struct sockaddr *)NULL, (socklen_t *)&addrlen);
        if (clientfd[k - 1] < 0)
            printError("Client accept failed", 1);

        pthread_create(&t, NULL, manageClient, &clientfd[k - 1]);
    }
}

void main()
{
    printf("***FTP SERVER***\n");
    initialize();

    pthread_t t;
    pthread_create(&t, NULL, acceptClient, NULL);
    pthread_join(t, NULL);
}
