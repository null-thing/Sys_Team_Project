#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

PORT_NUMBER = 700; // set to desired port number
SERVER_IP_ADDRESS = "192.168.0.4";

int main(void) {
    int socket_desc , client_sock, client_size;
    struct sockaddr_in server_addr ,client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    if (client_sock == -1){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUMBER);


    if (inet_pton(AF_INET, SERVER_IP_ADDRESS, &server_addr.sin_addr) <= 0) {
    perror("Invalid server address");
    exit(EXIT_FAILURE);
    }

    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    // do your behavior in here


    // 예를 들어,
    // use IO function such as read and write,
    /*
    char message[] = "hello server! from client.";
    write(client_sock, message, sizeof(message)); //5번
    */

    close(client_sock);
}
