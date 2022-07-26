#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void main()
{
	int socketfd,clientid,read_size,c;
	struct sockaddr_in server;
	char* msg;
	
	msg=(char *)malloc(sizeof(char)*20);
	
	int i;
	
	socketfd=socket(AF_INET,SOCK_STREAM,0);
	if (socketfd==-1)
	{
	    printf("Socket creation failed");
	}
	printf("Socket created\n");
	
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(8080);
	
	if (connect(socketfd,(struct sockaddr*)&server,sizeof(server))<0)
	{
	    printf("connect Failed");
	}
	else
	    printf("connect Success\n");
	
	if (recv(socketfd,msg,sizeof(char)*11,0)<0)
	{
	    printf("Receive failed");
	}
	printf("%s\n",msg);
	close(socketfd);
	
}
