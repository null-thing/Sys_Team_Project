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

    fp_txt = fopen("client_test.txt","w");
    fp_img = fopen("image_test.jpg","wb");

        puts("Connection accepted");

    recv_bytes = recv(client_socket, buf , 1000 , 0);
    if (recv_bytes<0){
        printf("err!");
	}
    buf[recv_bytes] = '\0';

//
    printf("test message:%s\n",buf);


    recv_bytes = recv(client_socket, buf , 1000 , 0);
    if (recv_bytes<0){
        printf("err!");
	}
    buf[recv_bytes] = '\0';


    printf("test message:%s\n",buf);


    if (recv(client_socket, buf , 1000 , 0) < 0){
        printf("err!");
	}

    printf("txt_len:%s\n",buf);

    txt_len = atoi(buf);
    printf("txt_len:%d\n",txt_len);

    if (recv(client_socket, buf , 1000 , 0) < 0){
        printf("err!");
	}
    
    printf("img_len:%s\n",buf);

    img_len = atoi(buf);
    printf("img_len:%d\n",img_len);

    for(i=0;i<=((txt_len-(txt_len%1000))/1000);i++){
        if (recv(client_socket, buf , 1000 , 0) < 0){
            printf("err!");
        }
        fwrite(buf,sizeof(char),1000,fp_txt);
	}

    for(i=0;i<=((img_len-(img_len%1000))/1000);i++){
        if (recv(client_socket, buf , 1000 , 0) < 0){
            printf("err!");
        }
        fwrite(buf,sizeof(char),1000,fp_txt);
    }


    fclose(fp_txt);
    fclose(fp_img);

    close(client_socket);
    close(server_socket);
}