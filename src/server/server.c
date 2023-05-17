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

#define SOCKS_IMPLEMENTATION
#include "../../include/socks.h"

#define SERVER
#define NETWORKING_IMPLEMENTATION
#include "../networking/networking.h"

typedef struct sockaddr info;

void *receive(void* conn) {
    Socket *client = (Socket *)conn;
	while (true) {
		char *buffer = socket_read(client);

		if (buffer) {
			Handler handler = deserialize_packet(buffer);
			handler(buffer);
		}
	}
    
	return NULL;
}

int main() {
	Socket server = socket_new(AF_INET, SOCK_STREAM);
    socket_bind(&server, "127.0.0.1", 5555);
	socket_listen(&server);

	while (true) {
        pthread_t thread_id;
		Socket client = socket_accept(&server);
		pthread_create(&thread_id, NULL, &receive, (void *)&client);
	}
	
	return 0;
}
