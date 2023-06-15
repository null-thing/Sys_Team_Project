#include <stdio.h>
#include "interpret.h"
#include "function.h"
#define SERVER_IP_ADDRESS "127.0.0.1"
#define PORT_NUMBER_PI 8010    
#define PORT_NUMBER_GUI 8011
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1033
#define DATA_BUFFER_SIZE 1000
#define FILENAME_SIZE 10

void *client_handler_PI (void* arg){

    Rcvdata recieved;
    Interpretdata interpreted;

    Client_info client_socket_info = *((Client_info*)arg);
    int client_socket = client_socket_info.socket;
    int my_id = client_socket_info.uid;

    printf("PI client handler\n"); 

    // do handler function
    char* data;
    char * header_buf;

    int file_len, file_data_len; // sender가 이렇게 보냄. long int로 보냈으면 좋았을 텐데.
    char pi_ID[10], file_category[3], file_len_str[10], file_data_len_str[10], file_data[1000];

    while(1){
        header_buf = (char*)malloc(BUFFER_SIZE);
        recv(client_socket, header_buf, BUFFER_SIZE, 10);
  
        strncpy(pi_ID, header_buf, 10);
        strncpy(file_category, header_buf+10, 3);
        strncpy(file_len_str,header_buf+13, 10);
        strncpy(file_data_len_str,header_buf+23,10);
        free(header_buf);

        file_len = atoi(file_len_str);
        file_data_len = atoi(file_data_len_str);

        for (int i =0 ; i<MAX_CLIENTS; i++){
            if (client_info_PI[i]->valid & client_info_PI[i]->uid==my_id){
                
            }
        }
        pthread_mutex_lock(&mu_PI);

        // interpret

        pthread_mutex_unlock(&mu_PI);

        if (is_end == -1) break;
    }
    interpret(&recieved, &data);

    while (1){
        if (is_end == -1) {
            close((client_socket_info.socket));
            pthread_mutex_lock(&mu_PI);
            num_clients_PI -= 1; //may cause bug, review needed
            pthread_mutex_unlock(&mu_PI);
            pthread_exit(NULL); 
            break;
        }
    } 
}

void *client_handler_GUI(void* arg){   
    char buffer_out[BUFFER_SIZE];
    
    printf("GUI connected\n");
    int client_socket_GUI = *((int*)arg);
    char buffer[256];
    ssize_t bytes;
    while(1){
        // under is testcode with client
        bytes = read(client_socket_GUI, buffer, sizeof(buffer) - 1); // deprecated ; when gui disconnects, then every socket terminates.
        if (bytes <= 0) break; //확인 필요
    }
    printf("Client disconnected. Closing all connections and exiting.\n"); 
    
    pthread_mutex_lock(&mu_PI);

    is_end = -1;
    int dummy_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT_NUMBER_PI);
    connect(dummy_socket, (struct sockaddr *)&server_address, sizeof(server_address));    
    
    pthread_mutex_unlock(&mu_PI);
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

void* send_data(void* arg) {
    while (1){
        pthread_mutex_lock(&mu_PI);
        interpreted_to_shared();
        while (strlen(shared_data) == 0) {
            pthread_cond_wait(&cond, &mu_PI);
        }
        send(*socket, shared_data, sizeof(*shared_data), 0);
        free(shared_data);
        pthread_mutex_unlock(&mu_PI);
        wait(1);
    }
}

void close_sockets() { //close all socket!!
    for (int i = 0; i<MAX_CLIENTS; i++) close(client_info_PI[i]->socket);
    num_clients_PI = 0;
}

void interpreted_to_shared(){
    int buf_size = sizeof(int) * MAX_CLIENTS;
    for (int i=0; i<MAX_CLIENTS; i++) if (clients_interpreted[i].valid) buf_size += strlen(clients_interpreted[i].image);  
    shared_data = (char*)malloc(buf_size); // **must** free after sending them.
    for (int i=0; i<MAX_CLIENTS; i++){
        if (clients_interpreted[i].valid) {
            strcat(shared_data, (char*)clients_interpreted[i].cli_info->uid);
            strcat(shared_data, (char*)clients_interpreted[i].light_on);
            strcat(shared_data, (char*)clients_interpreted[i].door_opened);
            strcat(shared_data, (char*)clients_interpreted[i].image);    
        }
    }
}