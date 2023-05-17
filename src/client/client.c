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

static char *username;
static char *hash;

void get_user_details() {
    username = malloc(sizeof(char) * 30);
    char *password = malloc(sizeof(char) * 30);
    
    printf("\033[2J");
    printf("\033[H");
    COLOR(255, 0, 0);
    printf("Welcome to Whatsdown\n");
    COLOR(255, 255, 255);
    printf("Username: ");
    fgets(username, 30, stdin);

    printf("Password: ");
    printf("\033[8m");
    fgets(password, 30, stdin);
    printf("\033[28m");

    password[strcspn(password, "\n")] = 0;
    username[strcspn(username, "\n")] = 0;

    hash = malloc(sizeof(char) * 257);
    sha256_easy_hash_hex(password, strlen(password), hash);
    free(password);
}

int main() {
    pthread_t t;

    get_user_details();

    Socket client = socket_new(AF_INET, SOCK_STREAM);
    socket_connect(&client, "127.0.0.1", 5555);
    
    SignupRequest request = (SignupRequest){.username=username, .hash=hash};
    char *packet = serialize_signup_request(request);

    socket_send(&client, packet, 1024);    

    free(hash); 
    free(username);
    free(packet);

    close(client.socket);

    // pthread_create(&t, NULL, &receive, &client);
    // pthread_exit(NULL);
    return 0;
}