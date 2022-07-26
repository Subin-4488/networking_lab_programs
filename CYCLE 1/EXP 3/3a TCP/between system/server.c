#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define port 8080

void main()
{
	int serverfd,clientfd;
	struct sockaddr_in server,client;
	int addrlen=sizeof(server);
	
	serverfd=socket(AF_INET,SOCK_STREAM,0);
	if (serverfd<0)
		printf("Socket creation failed\n");
	else
		printf("Socket created\n");
	
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;	
	server.sin_port=htons(port);
	
	if (bind(serverfd,(struct sockaddr*)&server,addrlen)<0)
	{
		printf("Bind unsuccessfull\n");
	}
	else
	{
		printf("Bind successfull\n");
	}
	
	printf("Listening...");
	listen(serverfd,3);
	
	clientfd=accept(serverfd,(struct sockaddr*)&client,(socklen_t*)&addrlen);
	
	if (clientfd<0)
		printf("Client connection unsuccessful\n");
	else
		printf("Client connected successfully\n");
		
	char msg[20]="HELLO WORLD";
	
	send(clientfd,msg,sizeof(msg),0);
	char message[1000];

	while (recv(clientfd,&message,sizeof(char)*100,0))
	{
		printf("%s\n",message);
	}
	
	printf("MSG SENT!!\n");
		
	
}
