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
#include "function.h"

#define SERVER_IP_ADDRESS "127.0.0.1"
#define PORT_NUMBER_PI 8010    
#define PORT_NUMBER_GUI 8011
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

pthread_mutex_t mu_PI;
int readercount = 0;

Client_info* client_info_PI[MAX_CLIENTS];
int num_clients_PI = 0;
int is_end = 0;

int main(void) {
    int socket_desc , client_socket_PI, client_socket_GUI;
    socklen_t client_address_length_PI, client_address_length_GUI;

    int server_socket_PI = socket(AF_INET, SOCK_STREAM, 0);
    int server_socket_GUI = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_PI == -1) {
        perror("Server Socket creation failed");
        exit(EXIT_FAILURE);
    }
    if (server_socket_GUI == -1){
        perror("Server Socket creation failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr_PI, server_addr_GUI, client_addr_PI, client_addr_GUI;
    memset(&server_addr_PI, 0, sizeof(server_addr_PI));
    memset(&server_addr_GUI, 0, sizeof(server_addr_GUI));

    server_addr_PI.sin_family = AF_INET;
    server_addr_PI.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    server_addr_PI.sin_port = htons(PORT_NUMBER_PI); // Set the desired port number

    server_addr_GUI.sin_family = AF_INET;
    server_addr_GUI.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    server_addr_GUI.sin_port = htons(PORT_NUMBER_GUI); // Set the desired port number

    if (bind(server_socket_PI, (struct sockaddr *) &server_addr_PI, sizeof(server_addr_PI)) == -1) {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    if (bind(server_socket_GUI, (struct sockaddr *) &server_addr_GUI, sizeof(server_addr_GUI)) == -1) {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket_PI, 5) == -1) { // Maximum 5 pending connections
        perror("Socket listening failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket_GUI, 1) == -1) { // Maximum 5 pending connections
        perror("Socket listening failed");
        exit(EXIT_FAILURE);
    }

    // Start Client handling
    printf("Server_PI linstening on port %d\n", PORT_NUMBER_PI);
    printf("Server_GUI linstening on port %d\n", PORT_NUMBER_GUI); 
    
    // GUI side: 
    client_address_length_GUI = sizeof(client_addr_GUI);
    client_socket_GUI = accept(server_socket_GUI, (struct sockaddr *)&client_addr_GUI, &client_address_length_GUI);    
    if (client_socket_GUI == -1) {
        perror("Socket accepting failed");
        exit(EXIT_FAILURE);
    }
    printf("GUI connected\n");
    int index_PI = 0;
    // threading test
    while (1) {
        // PI SIDE :
        // accepting connection
        pthread_t thread_PI;
        client_address_length_PI = sizeof(client_addr_PI);
        client_socket_PI = accept(server_socket_PI, (struct sockaddr *)&client_addr_PI, &client_address_length_PI);    
        if (client_socket_PI == -1) {
            perror("Socket accepting failed");
            exit(EXIT_FAILURE);
        }
        if (num_clients_PI >= MAX_CLIENTS) {
            printf("Reject : Already max number of PI hub clients\n");
            close(client_socket_PI);
            continue;
        }
        
        Client_info* cli = (Client_info*)malloc(sizeof(Client_info));
        cli->address = client_addr_PI;
        cli->socket =  client_socket_PI;
        cli->uid = readercount++;

        add_client(cli);
        num_clients_PI+=1;
        pthread_create(&thread_PI, NULL, &client_handler_PI, (void*)cli);
        sleep(1);
    }

    //gui에서 exit sign 보냄
    //exit 시그널을 보내거나
    // 연결을 끊기
    close(client_socket_PI);
    close(client_socket_GUI);


    char message[30];
    int bytes_received = recv(client_socket_PI, message, sizeof(message)-1, 0); //3번
    message[bytes_received] = '\0';
    printf("from client : %s \n", message);


    if (is_end) {
        close(server_socket_PI);
        close(server_socket_GUI);        
    }
    return EXIT_SUCCESS;
}