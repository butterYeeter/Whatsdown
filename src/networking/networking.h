#ifndef NETWORKING_H
#define NETWORKING_H

typedef enum {
    TP_LoginRequest = 0,
} PacketType;

typedef struct {
    char *username;
    char *hash;
} LoginRequest;

int copy_int(char *buffer, int data, int writer);
int copy_string(char *buffer, char *str, int writer);
int read_int(char *buffer, int reader, int *ret);
char *read_string(char *buffer, int reader, char **ret);

char *serialize_login_request(LoginRequest req);
#endif

#ifdef NETWORKING_IMPLEMENTATION

int copy_int(char *buffer, int data, int writer) {
    buffer[writer] = (char)data;
    writer++;
    return writer;
}

int copy_string(char *buffer, char *str, int writer) {
    writer = copy_int(buffer, strlen(str), writer);
    for (int i = 0; i < strlen(str); i++) {
        buffer[writer] = str[i];
        writer++;
    }
    return writer;
}

int read_int(char *buffer, int reader, int *ret) {
    *ret = (int)buffer[reader];
    reader++;
    return reader;
} 

char *read_string(char *buffer, int reader, char **ret) {
    int length;
    reader = read_int(buffer, reader, &length);
    printf("string length = %d\n", length);
    return NULL;
}

char *serialize_login_request(LoginRequest req) {
    char *buffer = malloc(1024);
    int writer = 0;
    writer = copy_int(buffer, (int)TP_LoginRequest, writer);
    writer = copy_string(buffer, req.username, writer);
    writer = copy_string(buffer, req.hash, writer);
    return buffer;
}

#endif