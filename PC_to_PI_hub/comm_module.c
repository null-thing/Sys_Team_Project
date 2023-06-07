#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <pthread.h>
#include <semaphore.h>

#define SERVER_IP_ADDRESS "127.0.0.1"
#define PORT_NUMBER_PI 8010    
#define PORT_NUMBER_GUI 8011
#define MAX_CLIENTS 10

pthread_mutex_t mu_PI, mu_GUI;
pthread_t tid;
pthread_t thread[2];
pthread_t thread_PI[100];
pthread_t thread_GUI[100];
int readercount = 0;


int main(void) {
    
    int socket_desc , client_socket_PI, client_socket_GUI, client_size;
    socklen_t client_address_length;

    int server_socket_PI = socket(AF_INET, SOCK_STREAM, 0);
    int server_socket_GUI = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_PI == -1 | server_socket_GUI == -1) {
        perror("Server Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr_PI, server_addr_GUI, client_addr;
    memset(&server_addr_PI, 0, sizeof(server_addr_PI));
    memset(&server_addr_GUI, 0, sizeof(server_addr_PI));

    server_addr_PI.sin_family = AF_INET;
    server_addr_PI.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    server_addr_PI.sin_port = htons(PORT_NUMBER_PI); // Set the desired port number

    server_addr_GUI.sin_family = AF_INET;
    server_addr_GUI.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    server_addr_GUI.sin_port = htons(PORT_NUMBER_GUI); // Set the desired port number

    int num_PI_clients;
    // number of GUI client is bound to 1
    if (bind(server_socket_PI, (struct sockaddr *) &server_addr_PI, sizeof(server_addr_PI)) == -1 |
        bind(server_socket_GUI, (struct sockaddr *) &server_addr_GUI, sizeof(server_addr_GUI)) == -1) {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket_PI, 5) == -1) { // Maximum 5 pending connections
        perror("Socket listening failed");
        exit(EXIT_FAILURE);
    }

    //debug?

    printf("Server_PI linstening on port %d\n", PORT_NUMBER_PI);
    printf("Server_GUI linstening on port %d\n", PORT_NUMBER_GUI); 
    

    /*
    client_address_length = sizeof(client_addr);
    
    // Accept the connection
    client_socket_PI = accept(server_socket_PI, (struct sockaddr *)&client_addr, &client_address_length);    
    if (client_socket_PI == -1) {
        perror("Socket accepting failed");
        exit(EXIT_FAILURE);
    }
    */
   int i =0;

    // threading test
    while (1) {
        // pthread_create(&(thread_PI[]), NULL, &client_handler_PI, NULL);
        // pthread_create(&(thread_GUI[i]), NULL, &client_handler_GUI, NULL);

        // Accept the connection
        int i = 0;
        client_address_length = sizeof(client_addr);
        client_socket_PI = accept(server_socket_PI, (struct sockaddr *)&client_addr, &client_address_length);    
        if (client_socket_PI == -1) {
            perror("Socket accepting failed");
            exit(EXIT_FAILURE);

        int client_name;
        recv(client_socket_PI, &client_name, sizeof(client_name),0);
        

        // recv(client_socket_GUI, &)
        // pthread_create(&thread_PI[i]);
        



    }    
        
    }
    char message[30];
    int bytes_received = recv(client_socket_PI, message, sizeof(message)-1, 0); //3번
    message[bytes_received] = '\0';
    printf("from client : %s \n", message);

    close(client_socket_PI);
    close(client_socket_GUI);
    close(server_socket_PI);
    close(server_socket_GUI);
}


void *client_handler_PI (void* arg){
    pthread_mutex_lock(&mu_PI);
    printf("PI client handler\n");
    //arg will be..


    


    pthread_mutex_unlock(&mu_PI);
    return NULL;

}

void *client_hadler_GUI(void* arg){
    pthread_mutex_lock(&mu_GUI);



    printf("GUI client handler\n");


    pthread_mutex_unlock(&mu_GUI);

    return NULL;
}