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
int read_string(char *buffer, int reader, char **ret);

char *serialize_login_request(LoginRequest req);
char *deserlize_packet(char *buffer);
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

int read_string(char *buffer, int reader, char **ret) {
    int length;
    reader = read_int(buffer, reader, &length);

    char *str = malloc(length + 1);
    int i;
    for (i = 0; i < length; i++) {
        str[i] = buffer[reader];
        reader++;
    }
    str[i] = '\0';

    *ret = str; 
    return reader;
}

char *serialize_login_request(LoginRequest req) {
    char *buffer = malloc(1024);
    int writer = 0;
    writer = copy_int(buffer, (int)TP_LoginRequest, writer);
    writer = copy_string(buffer, req.username, writer);
    writer = copy_string(buffer, req.hash, writer);
    return buffer;
}

char *deserlize_packet(char *buffer) {
    int type;
    int reader = 0;
    reader = read_int(buffer, reader, &type);
    switch ((PacketType)type) {
        case TP_LoginRequest: 
            char *username;
            char *hash;
            reader = read_string(buffer, reader, &username);
            reader = read_string(buffer, reader, &hash);
            
            printf("username = %s\n", username);
            printf("hash = %s\n", hash);

    }    
}

#endif