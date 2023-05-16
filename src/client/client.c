#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define DEBUG 1

#define SOCKS_IMPLEMENTATION
#include "../../include/socks.h"

#define NETWORKING_IMPLEMENTATION
#include "../networking/networking.h"

#include "../../include/hash.c"

void *receive(void *args) {   
    #ifdef DEBUG
    printf("[DEBUG]: Starting receive thread\n");
    #endif

    Socket *socket = (Socket *)args;
    while (true) {
        char *data;
        if ((data = socket_read(socket)) != NULL) {
            printf("%p\n", data);
        }
        else {
        }   
    }
}
   
#define COLOR(r, g, b) printf("\033[38;2;"#r";"#g";"#b"m");


int main() {
    Socket client = socket_new(AF_INET, SOCK_STREAM);
    socket_connect(&client, "127.0.0.1", 12345);

    pthread_t t;

    char *username = malloc(sizeof(char) * 30);
    char *password = malloc(sizeof(char) * 30);
    
    printf("\033[2J");
    printf("\033[H");
    COLOR(255, 0, 0);
    printf("Welcome to Whatsdown\n");
    COLOR(255, 255, 255);
    printf("Username: ");
    fgets(username, 30, stdin);

    password = getpass("Password: ");
    password[strcspn(password, "\n")] = 0;
    username[strcspn(username, "\n")] = 0;

    char *hash = malloc(sizeof(char) * 257);
    sha256_easy_hash_hex(password, strlen(password), hash);
    printf("%s\n", hash);

    int ret = 0;
    LoginRequest request = (LoginRequest){.username=username, .hash=hash};
    char *buffer = serialize_login_request(request);
    printf("buffer = %p\n", buffer);
    
    // int type;
    // int reader = 0;
    // read_int(buffer, reader, &type);
    // printf("type = %d\n", type);
    socket_send(&client, buffer, 1024);    

    free(hash);
    free(password);
    free(username);

    pthread_create(&t, NULL, &receive, &client);
    pthread_exit(NULL);
    return 0;
}