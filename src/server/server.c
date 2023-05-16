#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mongoc/mongoc.h>

#define SOCKS_IMPLEMENTATION
#include "../../include/socks.h"

int main() {
    printf("Server!\n");
    return 0;
}