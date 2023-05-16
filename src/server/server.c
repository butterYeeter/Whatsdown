#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <mongoc/mongoc.h>

#define NETWORKING_IMPLEMENTATION
#include "../networking/networking.h"

typedef struct sockaddr info;

void *receive(void* connectionfd) {
    int *clientfd = (int*) connectionfd;
    char *buffer = malloc(1024);
    bzero(buffer, 1024);
    int n = read(*clientfd, buffer, 1024);
    if(n < 0) {
        printf("Error recieving data from client\n");
        pthread_exit(NULL);
    }

    Handler handler = deserialize_packet(buffer);
    handler(buffer);
    printf("%p\n", buffer);
    free(buffer);
    close(*clientfd);
    pthread_exit(NULL);
}


int main()
{
    int serverfd, connectionfd, serversockLen, n;
	struct sockaddr_in serversock;

	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverfd < 0) {return 1;}

	serversock.sin_family = AF_INET;
	serversock.sin_port = htons(5555);
	serversock.sin_addr.s_addr = INADDR_ANY;

	serversockLen = sizeof(serversock);
	n = bind(serverfd, (info*)&serversock, serversockLen);
	if(n < 0) {return 2;}

	n = listen(serverfd, 10);
	if(n < 0) {return 3;}

	while(true) {
        pthread_t thread_id;
        connectionfd = accept(serverfd, (info*)&serversock, &serversockLen);
		if(connectionfd < 0) {return 4;}
		pthread_create(&thread_id, NULL, &receive, &connectionfd);
	}
	
	close(serverfd);
	return 0;
}
