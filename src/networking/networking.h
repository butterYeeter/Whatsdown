#ifndef NETWORKING_H
#define NETWORKING_H

#ifdef SERVER
#include <mongoc/mongoc.h>
#endif

typedef enum {
    TP_LoginRequest = 0,
    TP_SignupRequest = 1,
} PacketType;

typedef struct {
    char *username;
    char *hash;
} LoginRequest;

typedef struct {
    char *username;
    char *hash;
} SignupRequest;

typedef struct {
    char *from;
    char *to;
    char *payload;
} Text;

int copy_int(char *buffer, int data, int writer);
int copy_string(char *buffer, char *str, int writer);
int read_int(char *buffer, int reader, int *ret);
int read_string(char *buffer, int reader, char **ret);

char *serialize_login_request(LoginRequest req);
char *serialize_signup_request(SignupRequest req);

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

char *serialize_signup_request(SignupRequest req) {
    char *buffer = malloc(1024);
    int writer = 0;
    writer = copy_int(buffer, (int)TP_SignupRequest, writer);
    writer = copy_string(buffer, req.username, writer);
    writer = copy_string(buffer, req.hash, writer);
    return buffer;
}

void deserialize_login_request(char *buffer) {
    char *username;
    char *hash;
    int reader = 1;
    reader = read_string(buffer, reader, &username);
    reader = read_string(buffer, reader, &hash);
    
    printf("username = %s\n", username);
    printf("hash = %s\n", hash);
    free(username);
    free(hash);

}

void deserialize_signup_request(char *buffer) {
    char *username;
    char *hash;
    int reader = 1;
    reader = read_string(buffer, reader, &username);
    reader = read_string(buffer, reader, &hash);
    
    #ifdef SERVER
    mongoc_init();
    mongoc_client_t *client = mongoc_client_new("mongodb+srv://user:Wc4GbzYemcGwjIOE@cluster0.bcvcu2k.mongodb.net/?retryWrites=true&w=majority");
    mongoc_collection_t *collection = mongoc_client_get_collection(client, "Users", "Users");

    bson_t *doc = bson_new();
    bson_oid_t oid;
    bson_oid_init(&oid, NULL);
    BSON_APPEND_OID(doc, "_id", &oid);
    BSON_APPEND_UTF8(doc, "name", username);
    BSON_APPEND_UTF8(doc, "phash", hash);

    mongoc_collection_insert_one(collection, doc, NULL, NULL, NULL);
    #endif
    printf("[SERVER DEBUG]: User (%s) signed up!\n", username);
    free(username);
    free(hash);
}

typedef void (*Handler)(char *);

Handler deserialize_packet(char *buffer) {
    int type;
    int reader = 0;
    reader = read_int(buffer, reader, &type);
    switch ((PacketType)type) {
        case TP_LoginRequest: 
            return deserialize_login_request;
        case TP_SignupRequest:
            return deserialize_signup_request;
    }    
}

#endif