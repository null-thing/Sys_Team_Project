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

extern char shared_data[1024];

typedef struct _Client_info{
    int socket;
    int uid;
    struct sockaddr_in address;
} Client_info;

typedef enum {
    LIGHT = 123,
    DISTANCE = 100  //value is placeholder yet
}Threshold;

typedef struct _Rcvdata{
    char* image;
    char* text;
    Client_info* cli_info;
    int socket;
}Rcvdata;

typedef struct _Interpretdata{
    char* image;
    char door_opened;
    char light_on;
    Client_info* cli_info;
    int socket;
}Interpretdata;

void read_data(Rcvdata*,Client_info*);
void interpret(Rcvdata*, char**) ;
#endif