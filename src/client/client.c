#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define DEBUG 1

#define SOCKS_IMPLEMENTATION
#include "../../include/socks.h"

#include "../../include/hash.c"

void* receive(void *args)
{   
    #ifdef DEBUG
    printf("[DEBUG]: Starting receive thread\n");
    #endif
    Socket *socket = (Socket *)args;
    while (true) {
        char *data;
        if ((data = socket_read(socket)) != NULL) {
            printf("%p\n", data);
        }
    }
    return NULL;
}
   
#define COLOR(r, g, b) printf("\033[38;2;"#r";"#g";"#b"m");

typedef enum {
    TP_LoginRequest,
} PacketType;

typedef struct {
    PacketType type;
    const char *username;
    const char *hash;
} LoginRequest;

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
    pthread_create(&t, NULL, &receive, &client);

    LoginRequest request = (LoginRequest){.type=TP_LoginRequest, .username=username, .hash=hash};
    socket_send(&client, &request, sizeof(request));    
    
    free(hash);
    free(password);
    free(username);

    pthread_exit(&ret);
    return 0;
}