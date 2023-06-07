#include <stdio.h>
#include "interpret.h"
#include "function.h"
#define SERVER_IP_ADDRESS "127.0.0.1"
#define PORT_NUMBER_PI 8010    
#define PORT_NUMBER_GUI 8011
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024


void *client_handler_PI (void* arg){
    char buffer_in[BUFFER_SIZE];
    
    Client_info client_socket_PI = *((Client_info*)arg);
    pthread_mutex_lock(&mu_PI);
    printf("PI client handler\n");  
    pthread_mutex_unlock(&mu_PI);
    if (is_end == -1) {
        close(client_socket_PI.socket);
        pthread_exit(NULL); 
        num_clients_PI -= 1; //may cause bug, review needed
    }  
}

void *client_handler_GUI(void* arg){   
    char buffer_out[BUFFER_SIZE];

    int client_socket_GUI = *((int*)arg);
    // pthread_mutex_lock(&mu_GUI);


    // recv() //exit signal
    if (0) {is_end = -1;}
    printf("GUI client handler\n");

    // pthread_mutex_unlock(&mu_GUI);
    return NULL;
}



void add_client(Client_info* client){
    pthread_mutex_lock(&mu_PI);
    for (int i=0; i<MAX_CLIENTS; i++){
        if (!client_info_PI[i]){
            client_info_PI[i] = client;
            break;
        }
    }
    pthread_mutex_unlock(&mu_PI);
}

void remove_client(int uid){
    pthread_mutex_lock(&mu_PI);
    for(int i=0; i<MAX_CLIENTS; ++i){
        if(client_info_PI[i]){
            if(client_info_PI[i]->uid == uid){
                client_info_PI[i] = NULL;
                break;
            }
        }
    }
    pthread_mutex_unlock(&mu_PI);
}

void send_data() {}