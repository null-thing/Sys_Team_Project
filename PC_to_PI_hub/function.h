#ifndef FUNCTION_H
#define FUNCTION_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <pthread.h>
#include <semaphore.h>

#include "interpret.h"
#define MAX_CLIENTS 5


extern int is_end;
extern pthread_mutex_t mu_PI;
extern pthread_cond_t cond;
extern int readercount;
extern int num_clients_PI;
extern Client_info* client_info_PI[MAX_CLIENTS];
extern char shared_data[1024];

void* client_handler_PI(void*);
void* client_handler_GUI(void*);
void add_client(Client_info*);
void remove_client(int);
void* send_data(void*);
void close_sockets();

#endif