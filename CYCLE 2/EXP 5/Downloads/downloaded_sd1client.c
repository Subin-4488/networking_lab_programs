#include "head.h"

int sockfd;
struct sockaddr_in server;
struct clientData c;

void initialize()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printError("Socket creation failed", 1);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    server.sin_family = AF_INET;

    if (connect(sockfd, (struct sockaddr *)&server, (socklen_t)addrlen) < 0)
        printError("Connect failed", 1);
}

void upload()
{
    char msg[50];
    char file[50];
    system("ls");
    printf("Enter filename:\n");
    scanf(" %s", msg);

    FILE *f = fopen(msg, "r");
    if (f == NULL)
    {
        printError("FILE NOT FOUND!!", 0);
        return;
    }
    else
    {
        strcpy(file, msg);
        if (send(sockfd, STOR, sizeof(char) * 4, 0) < 0)
        {
            printError("Connection lost", 0);
            return;
        }
        else
        {
            if (recv(sockfd, msg, sizeof(char) * 10, 0) < 0)
            {
                printError("Connection lost", 0);
            }
            else
            {
                if (!strcmp(msg, OK))
                {
                    printf("FILE: %s\n", file);
                    if (send(sockfd, file, sizeof(char) * 50, 0) < 0)
                    {
                        printError("Connection failed", 0);
                        return;
                    }
                    else
                    {
                        printf("File uploaded...\n");
                        recv(sockfd, msg, sizeof(char) * 10, 0);
                        printf("ACKNOWLEGEMENT: %s\n", msg);
                    }
                }
                else
                {
                    printf("ACKNOWLEGEMENT: %s", msg);
                    return;
                }
            }
        }
    }
}

void retrieve()
{
    char msg[50];
    char file[50];
    strcpy(msg, "ls FILES/");
    strcat(msg, c.username);
    system(msg);

    printf("Enter filename:\n");
    msg[0] = '\0';
    scanf("%s", msg);

    strcpy(file, msg);
    if (send(sockfd, RETR, sizeof(char) * 4, 0) < 0)
    {
        printError("Connection lost", 0);
        return;
    }
    else
    {
        if (recv(sockfd, msg, sizeof(char) * 10, 0) < 0)
        {
            printError("Connection lost", 0);
        }
        else
        {
            if (!strcmp(msg, OK))
            {
                send(sockfd, file, sizeof(char) * 50, 0);
                printf("Downloading...\n");
                if (recv(sockfd, msg, sizeof(char) * 10, 0) < 0)
                {
                    printError("Connection lost", 0);
                }
                else
                {
                    printf("File retrieved from FTP server...\n");
                    printf("ACKNOWLEGEMENT: %s\n", msg);
                }
            }
            else
            {
                printf("ACKNOWLEGEMENT: %s", msg);
                return;
            }
        }
    }
}

void main()
{
    printf("Connecting to FTP server...\n");
    sleep(1);
    initialize();
    printf("Connected to FTP server\n");

    char msg[50];
    printf("Enter username: ");
    scanf("%s", msg);
    strcpy(c.username, msg);

    printf("Enter password: ");
    scanf("%s", msg);
    strcpy(c.password, msg);

    c.clientfd = -1;
    if (send(sockfd, &c, sizeof(struct clientData), 0) < 0)
        printError("Connection failed", 1);

    printf("Waiting for authentication...\n");
    recv(sockfd, msg, sizeof(char) * 10, 0);

    if (!strcmp(msg, OK))
        printf("Authenticated: %s\n", msg);
    else
        printf("Failed %s\n", msg);

    while (1)
    {
        int option;
        printf("Enter the OPTION: \n1: UPLOAD FILE\n2: DOWNLOAD FILE\n");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            upload();
            break;
        case 2:
            retrieve();
            break;
        default:
            printf("Enter valid option\n");
            break;
        }
    }
}