#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
int PORT_NUMBER = 8010; // set to desired port number
char* SERVER_IP_ADDRESS = "127.0.0.1";


int main(void) {    
    int client_socket, client_size;
    struct sockaddr_in server_addr, client_addr;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (client_socket == -1){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUMBER);

    if (inet_pton(AF_INET, SERVER_IP_ADDRESS, &server_addr.sin_addr) <= 0) {
        perror("Invalid server address");
        exit(EXIT_FAILURE);
    }
    
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    // do your behavior in here

    // 예를 들어,
    // use IO function such as read and write,
    char message[] = "hello server! from client.";
    send(client_socket, message, sizeof(message),0);

    //close(client_socket);
    return 0;
}
