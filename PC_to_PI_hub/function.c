#include <stdio.h>
#include "interpret.h"
#include "function.h"
#define SERVER_IP_ADDRESS "127.0.0.1"
#define PORT_NUMBER_PI 8010    
#define PORT_NUMBER_GUI 8011
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024


void *client_handler_PI (void* arg){
    printf("is_end : %d\n",is_end);
    char buffer_in[BUFFER_SIZE];
    Client_info client_socket_PI = *((Client_info*)arg);
    pthread_mutex_lock(&mu_PI);
    printf("PI client handler\n"); 
    // do handler function
    // read_data
    // interpret()
    // write data()
    pthread_mutex_unlock(&mu_PI);
    while (1){
        if (is_end == -1) {
            close(client_socket_PI.socket);
            pthread_exit(NULL); 
            num_clients_PI -= 1; //may cause bug, review needed
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
    while((bytes = read(client_socket_GUI, buffer, sizeof(buffer) - 1)) > 0){
        buffer[bytes] = '\0';
        printf("Message from client: %s\n", buffer);
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

void send_data(char* text, char** img, int* socket) {
    send(*socket, text, sizeof(text), 0);
    int imgbytes ;
    for (int i=0;i<num_clients_PI;i++) {
        imgbytes = sizeof(img);
        send(*socket, img, imgbytes, 0);
    }
}

void close_sockets() {
    for (int i = 0; i<MAX_CLIENTS; i++){
        close(client_info_PI[i]->socket);
    }
    num_clients_PI = 0;
}

void* exit_handler(void* arg){
    if (is_end==-1) close_sockets();
}
