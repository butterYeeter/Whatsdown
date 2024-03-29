#ifndef SOCKS_H
#define SOCKS_H

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct {
    int socket;
    bool connected;
    struct sockaddr_in address;
} Socket;

typedef struct {
    int socket;
} Client;

Client client_new();
Socket socket_new();

#endif
#ifdef SOCKS_IMPLEMENTATION

Socket socket_new(int domain, int protocol) {
    return (Socket){.socket=socket(domain, protocol, 0)};
}

void socket_bind(Socket *socket, char *ip, int port) {
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);;
    address.sin_port = htons(port);
    socket->address = address;
    socket->connected = false;
    bind(socket->socket, (struct sockaddr*)&address, sizeof(address));
}

void socket_connect(Socket *socket, char *ip, int port) {
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);;
    address.sin_port = htons(port);
    socket->address = address;
    int status = connect(socket->socket, (struct sockaddr*)&address, sizeof(address));
    if (status != -1) {
        socket->connected = true;
    }
}

void socket_send(Socket *socket, void *packet, size_t packet_size) {
    if (socket->connected) {
        if (send(socket->socket, packet, packet_size, 0) != packet_size) {
            printf("Sending failed...\n");
        }
    }
}

char *socket_read(Socket *socket) {
    void *buffer = malloc(1024);
    int n = read(socket->socket, buffer, 1024);
    if (n > 0) {
        return buffer;
    }
    free(buffer);
    return NULL;
}

void socket_listen(Socket *socket) {
    listen(socket->socket, 3);
}

Socket socket_accept(Socket *socket) {
    int size = sizeof(socket->address);
    int new_socket = accept(socket->socket, (struct sockaddr*)&socket->address, (socklen_t*)&size);
    Socket ret;
    ret.socket = new_socket;
    return ret;
}

#endif