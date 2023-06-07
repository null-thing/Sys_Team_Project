#ifndef INTERPRET_H
#define INTERPRET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <pthread.h>
#include <semaphore.h>

typedef struct _Client_info{
    int socket;
    int uid;
    struct sockaddr_in address;
} Client_info;

typedef enum {
    LIGHT = 123,
    DISTANCE = 100  //value is placeholder yet
}Threshold;

void read_data(Client_info, char*);
void interpret(char* , char*) ;
void write_data(Client_info, char*);

#endif