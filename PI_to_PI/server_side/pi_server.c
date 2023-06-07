#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int PORT_NUMBER = 8000; // set to desired port number
char* SERVER_IP_ADDRESS = "127.0.0.1"; //테스트용 노트북의 ip주소: rpi->노트북 자료 받음


/*  
    Server pi - bridge pi
*/


void recv_file_from_client(int client_socket){
    FILE* fp;
    int file_len, message_len, recv_bytes;
    char* write_num, *pi_serial_num, *file_category, *message_contents;
    char recv_buf[1050], file_name[10];

    //header information: [pi_serial_num][file_category][file_len][message_len][message_contents]

    recv_bytes =  recv(client_socket, recv_buf, 1050, 0);
    if (recv_bytes<0){
        printf("err!");
	}


    pi_serial_num = strtok(recv_buf,"+*+*");
    file_category = strtok(NULL, "+*+*");
    file_len = atoi(strtok(NULL,"+*+*"));
    message_len = atoi(strtok(NULL,"+*+*"));
    message_contents = strtok(NULL,"+*+*");
    strcpy(file_name, pi_serial_num);
    strcat(file_name, ".");
    strcat(file_name, file_category);

    if(!strcmp(file_category, "txt")){
        fp = fopen(file_name, "w");
    }else{
        fp = fopen(file_name, "wb");
    }

    printf("%s\n",message_contents);


    fwrite(message_contents, sizeof(char), message_len,fp);

    printf("%s\n",message_contents);
    
    while(message_len == 1000){

        recv_bytes =  recv(client_socket, recv_buf, 1050, 0);
        if (recv_bytes<0){
            printf("err!");
	    }

        pi_serial_num = strtok(recv_buf,"+*+*");
        file_category = strtok(NULL, "+*+*");
        file_len = atoi(strtok(NULL,"+*+*"));
        message_len = atoi(strtok(NULL,"+*+*"));
        message_contents = strtok(NULL,"+*+*");
        strcpy(file_name, pi_serial_num);
        strcat(file_name, ".");
        strcat(file_name, file_category);

        printf("%s\n",message_contents);


        fwrite(message_contents, sizeof(char), message_len,fp);

        printf("%s\n",message_contents);
    }

    printf("file recv complete!\n");
    fclose(fp);
}
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

    FILE* fp_txt;
    FILE* fp_img;
    char buf[1000];
    int write_num;
    int txt_len;
    int img_len;
    int recv_bytes;
    int i;


    recv_bytes = recv(client_socket, buf , 1000 , 0);
    if (recv_bytes<0){
        printf("err!");
	}
    buf[recv_bytes] = '\0';

//
    printf("test message:%s\n",buf);

    recv_file_from_client(client_socket);
    recv_file_from_client(client_socket);


    close(client_socket);
    close(server_socket);
}