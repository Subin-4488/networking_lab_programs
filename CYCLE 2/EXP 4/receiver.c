#include "head.h"

struct sockaddr_in server;
int sockfd;

void initialize()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = port;
    if (connect(sockfd, (struct sockaddr *)&server, addrlen) < 0)
    {
        perror("Connect failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server\n");
}

int cmprDomain(char m1[100], char m2[100])
{
    int i, j;
    for (i = strlen(m1) - 1, j = strlen(m2) - 1; j >= 0; i--, j--)
    {
        if (m1[i] != m2[j])
            return 0;
    }
    return 1;
}

int receiveAck()
{
    char temp[10];
    recv(sockfd, temp, sizeof(char) * 9, 0);
    if (!strcmp(temp, OK))
        return 1;
    return 0;
}

void *receiveMessage()
{
    char msg[200];
    int t;
    char temp[200];

    while (1)
    {
        msg[0]='\0';
        printf("Enter 1 to check inbox:\n");
        scanf("%d", &t);
        if (t == 1)
        {
            send(sockfd, HELLO, sizeof(HELLO), 0);
            if (receiveAck())
            {
                printf("Acknowledgement received\n");
                printf("Enter your email address to check the inbox:\n");
                scanf(" %s", msg);
                while (!cmprDomain(msg, DOMAIN))
                {
                    printf("Enter proper domain\n");
                    printf("Enter your email address:\n");
                    scanf(" %s", msg);
                }
                strcpy(temp,RECIPIENT); strcat(temp,msg); strcpy(msg,temp);
                send(sockfd, msg, sizeof(msg), 0); 
                if (receiveAck())
                {
                    printf("Acknowledgement received\n");
                    struct mail m[10];
                    int count;
                    recv(sockfd,(struct mail*)&m,sizeof(struct mail)*10,0);
                    sleep(1);
                    recv(sockfd,&count,sizeof(int),0);
                    printf("\n***INBOX***\n");
                    for (int i=0;i<count;i++){
                        printf("%d)\n%s\n%s\n%s\n\n",i+1,m[i].src,m[i].recepient,m[i].msg);
                    }
                    close(sockfd);
                    exit(EXIT_SUCCESS);
                }
                else
                    printf("Failed\n");
            }
            else
                printf("Failed\n");
        }
    }
}

void main()
{
    initialize();
    pthread_t t;
    pthread_create(&t, NULL, receiveMessage, NULL);
    pthread_join(t, NULL);
}