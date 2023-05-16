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



typedef struct sockaddr info;

void *receive(void* connectionfd) {
    printf("thread creation success!\n");
    int *clientfd = (int*) connectionfd;
    char *buffer = malloc(sizeof(char) * 1024);
    bzero(buffer, 1024);
    int n = read(*clientfd, buffer, 1024);
    printf("Read status: %d\n", n);
    if(n < 0) {
        printf("Error recieving data from client\n");
        pthread_exit(NULL);
    }

    printf("hello world lol\t%p\n", buffer);
    free(buffer);
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
        printf("Connectonfd:%d\n", connectionfd);
		if(connectionfd < 0) {return 4;}
		pthread_create(&thread_id, NULL, &receive, &connectionfd);
	}
	
	close(serverfd);
	return 0;
}
