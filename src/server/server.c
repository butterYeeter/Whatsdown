// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <strings.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <pthread.h>
// #include <mongoc/mongoc.h>
// #include <fcntl.h>

// #define SOCKS_IMPLEMENTATION
// #include "../../include/socks.h"

// #define SERVER
// #define NETWORKING_IMPLEMENTATION
// #include "../networking/networking.h"



// void *receive(void* conn) {
//     Socket *client = (Socket *)conn;
// 	while (true) {
// 		char *buffer = socket_read(client);

// 		if (buffer) {
// 			Handler handler = deserialize_packet(buffer);
// 			handler(buffer);
// 		}
// 	}
    
// 	return NULL;
// }

// int main() {
// 	Socket server = socket_new(AF_INET, SOCK_STREAM);
//     socket_bind(&server, "127.0.0.1", 5555);
// 	socket_listen(&server);

// 	while (true) {
//         pthread_t thread_id;
// 		Socket client = socket_accept(&server);
// 		pthread_create(&thread_id, NULL, &receive, (void *)&client);
// 	}
	
// 	return 0;
// }




// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <strings.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <pthread.h>
// #include <mongoc/mongoc.h>
// #include <fnctl.h>
// #include <aio.h>
// 
// #define NETWORKING_IMPLEMENTATION
// #include "../networking/networking.h"
// 
// typedef struct sockaddr info;
// 
// void *receive(void* connectionfd) {
    // int *clientfd = (int*) connectionfd;
    // char *buffer = malloc(1024);
    // bzero(buffer, 1024);
    // int n = read(*clientfd, buffer, 1024);
    // if(n < 0) {
        // printf("Error recieving data from client\n");
        // pthread_exit(NULL);
    // }
// 
    // Handler handler = deserialize_packet(buffer);
    // handler(buffer);
    // printf("%p\n", buffer);
    // free(buffer);
    // close(*clientfd);
    // pthread_exit(NULL);
// }
// 
// 
// int main()
// {
    // int serverfd, connectionfd, serversockLen, n;
	// struct sockaddr_in serversock;
// 
	// serverfd = socket(AF_INET, SOCK_STREAM, 0);
	// if(serverfd < 0) {return 1;}
// 
	// serversock.sin_family = AF_INET;
	// serversock.sin_port = htons(5555);
	// serversock.sin_addr.s_addr = INADDR_ANY;
// 
	// serversockLen = sizeof(serversock);
	// n = bind(serverfd, (info*)&serversock, serversockLen);
	// if(n < 0) {return 2;}
// 
	// n = listen(serverfd, 10);
	// if(n < 0) {return 3;}
    // 
// 
    // char *buffer = (char*) malloc(1024);
// 
	// while(true) {
        // pthread_t thread_id;
        // connectionfd = accept(serverfd, (info*)&serversock, &serversockLen);
		// if(connectionfd < 0) {return 4;}
        // int flag = fcntl(connectionfd, F_GETFL, 0);
        // // if(fcntl(connectionfd, F_SETFL, flags| O_NONBLOCK) == -1) {
        // //     printf("Failed to set socket to O_NONBLOCK\n");
        // // }
		// // pthread_create(&thread_id, NULL, &receive, &connectionfd);
        // struct aiocb aiocbp;
        // aiocbp.fildes = connectionfd;
        // aiocb_var.aio_fildes = fd;
        // aiocb_var.aio_buf = buffer;
        // aiocb_var.aio_nbytes = 1024;
        // aiocb_var.aio_offset = 0;
        // aiocb_var.aio_lio_opcode = LIO_READ;
        // aiocb_var.aio_sigevent.sigev_notify = SIGEV_NONE;
        // aio_read(&aiocbp);
	// }
	// 
	// free(buffer);
    // close(serverfd);
	// return 0;
// }


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

typedef enum {
	SUPR = 1, SINR = 2, AR = 3, MS = 4, MR = 5
} Type;

typedef struct {
	char *username;
	char *phash;
} SignupRequest;

typedef struct {
	char *username;
	char *phash;
} SigninRequest;

typedef struct {
	char *token;
} AccessResponse;

typedef struct {
	char *to;			// A username
	char *contents;
	char *token;
} MessageSend;

typedef struct {
	char *contents;
	char *from;			// A username
} MessageReceive;

typedef struct {
	Type packType;
	void *packetdata;
} Packet;



int main() {
	int serverfd, connectionfd, n, length;
	struct sockaddr_in serveraddr, connection;

	serverfd = socketfd(AF_INET, SOCK_STREAM, 0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(5555);
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	length = sizeof(serveraddr);

	n = bind(serverfd, (struct sockaddr*)&serveraddr, &length);
	if(n < 0) {return 1};
	



	return 0;
}
