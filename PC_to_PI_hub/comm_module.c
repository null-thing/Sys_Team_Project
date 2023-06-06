#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <pthread.h>
#include <semaphore.h>

sem_t x,y;
pthread_t tid;
pthread_t thread1[100];
pthread_t thread2[100];
int readercount = 0;


char* SERVER_IP_ADDRESS = "192.168.0.4";
int PORT_NUMBER_PI = 8010; // set to desired port number    
int PORT_NUMBER_GUI = 8011;


int main(void) {
    
    int socket_desc , client_sock, client_size;

    int server_socket_PI = socket(AF_INET, SOCK_STREAM, 0);
    int server_socket_GUI = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_PI == -1 | server_socket_GUI) {
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

    printf("Server_PI linstening on port %d", PORT_NUMBER_PI);
    printf("Server_GUI linstening on port %d", PORT_NUMBER_GUI); 
    
    int client_socket;
    socklen_t client_address_length = sizeof(client_addr);

    // Accept the connection
    client_socket = accept(server_socket_PI, (struct sockaddr *)&client_addr, &client_address_length);    
    if (client_socket == -1) {
        perror("Socket accepting failed");
        exit(EXIT_FAILURE);
    }

    // do your behavior in here


    close(client_socket);
    close(server_socket_PI);
}


void* reader(void* param){
    sem_wait(&x);
    readercount++;

    if (readercount == 1)
        sem_wait(&y);

    sem_post(&x);

    printf("\n%d reader", readercount);

    sleep(5);





}

void *client_handler_PI (){

}

void *client_hadler_GUI(){


}