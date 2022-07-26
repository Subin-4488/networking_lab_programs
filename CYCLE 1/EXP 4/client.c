#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void main()
{
	int socketfd,clientfd,read_size,c;
	struct sockaddr_in server;
	char* msg;
	int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int out[3];
	
	clientfd=socket(AF_INET,SOCK_STREAM,0);
	if (socketfd==-1)
	{
	    printf("Socket creation failed");
	}
	printf("Socket created\n");
	
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(8080);
	
	if (connect(clientfd,(struct sockaddr*)&server,sizeof(server))<0)
	{
	    printf("connect Failed");
	}
	else
	    printf("connect Success\n");
	
	
	if(send(clientfd, arr, 10 * sizeof(int), 0) < 0) {
		printf("Send failed!\n");
		exit(1);
	} else {
		printf("Data sent:");
		for(int i = 0; i < 10; i++) {
			printf(" %d", arr[i]);
		}
		printf("\nWaiting for reply.\n");
	}
	
	if(recv(clientfd, out, 3 * sizeof(int), 0) < 0) {
		printf("Receive failed!\n");
		exit(1);
	} else {
		printf("Result received:\n");
		printf("Min: %d\n", out[0]);
		printf("Max: %d\n", out[1]);
		printf("Avg: %d\n", out[2]);
	}
	
	
	close(clientfd);
	
}
