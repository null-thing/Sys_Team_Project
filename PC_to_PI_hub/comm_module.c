#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

PORT_NUMBER = 700; // set to desired port number
SERVER_IP_ADDRESS = "192.168.0.4"; //set to desired ip address


int main(void) {
    int socket_desc , client_sock, client_size;

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr ,client_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    server_addr.sin_port = htons(PORT_NUMBER); // Set the desired port number

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) { // Maximum 5 pending connections
        perror("Socket listening failed");
        exit(EXIT_FAILURE);
    }

    int client_socket;
    socklen_t client_address_length = sizeof(client_addr);

    // Accept the connection
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_address_length);
    if (client_socket == -1) {
        perror("Socket accepting failed");
        exit(EXIT_FAILURE);
    }

    // do your behavior in here


    close(client_socket);
    close(server_socket);
}