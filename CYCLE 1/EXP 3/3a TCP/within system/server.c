#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void main()
{
	int socketfd,clientid,read_size,c;
	struct sockaddr_in server,client;
	char* msg="Hello world";
	int i;
	
	socketfd=socket(AF_INET,SOCK_STREAM,0);
	if (socketfd==-1)
	{
	    printf("Socket creation failed");
	}
	printf("Socket created\n");
	
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(8080);
	
	if (bind(socketfd,(struct sockaddr*)&server,sizeof(server))<0)
	{
	    printf("Bind Failed");
	}
	else
	    printf("Bind Success\n");
	
	listen(socketfd,3);
	
	printf("Waiting for incoming connections...");
	
	c=sizeof(struct sockaddr_in);
	
	clientid=accept(socketfd,(struct sockaddr*)&client, (socklen_t*)&c);
	
	if (clientid<0)
	{
	    printf("Accept failed");
	}
	
	printf("Connection accepted\n");
	
	write(clientid,msg,sizeof(char)*11);
	
}
