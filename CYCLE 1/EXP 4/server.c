#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int findMin(int arr[]) {
	int min = 0;
	for(int i = 0; i < 10; i++) {
		if (arr[min] > arr[i])
			min = i;
	}
	return arr[min];
}

int findMax(int arr[]) {
	int max = 0;
	for(int i = 0; i < 10; i++) {
		if (arr[max] < arr[i])
			max = i;
	}
	return arr[max];
}

int findAvg(int arr[]) {
	int avg = 0;
	for(int i = 0; i < 10; i++) {
		avg += arr[i];
	}
	avg /= 10;
	return avg;
}

void main()
{
	
	int socketfd,clientfd,read_size,c;
	struct sockaddr_in server,client;
	int i;
	int arr[10], out[3];
	
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
	
	clientfd=accept(socketfd,(struct sockaddr*)&client, (socklen_t*)&c);
	
	if (clientfd<0)
	{
	    printf("Accept failed");
	}
	
	printf("Connection accepted\n");
	
	if(recv(clientfd, arr, 10 * sizeof(int), 0) < 0) {
		printf("Receive failed!\n");
	} else {
		printf("Received data:");
		for(int i = 0; i < 10; i++) {
			printf(" %d", arr[i]);
		}
		printf("\n");
	}
	
	printf("Calculating min, max and avg.\n");
	out[0] = findMin(arr);
	out[1] = findMax(arr);
	out[2] = findAvg(arr);
	
	if(send(clientfd, out, 3 * sizeof(int), 0) < 0) {
		printf("Send failed!\n");
	} else {
		printf("Result sent to client.\n");
	}
	
}
