#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int PORT_NUMBER = 8000; // set to desired port number
char* SERVER_IP_ADDRESS = "127.0.0.1"; //테스트용 노트북의 ip주소: rpi->노트북 자료 전송

/*  
    client pi - sensor pi
*/

int send_flie_to_server(int server_socket, char* file_name, char* file_category){
    FILE* fp;
    int file_len;
    if(!strcmp(file_category, "txt")){
        fp = fopen(file_name, "r");
    }else{
        fp = fopen(file_name, "rb");
    }



    fclose(fp);
    return 0;
}


void main() {
    int client_sock;
    struct sockaddr_in server_addr;
    
    
    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    if (client_sock == -1){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUMBER);


    if (inet_pton(AF_INET, SERVER_IP_ADDRESS, &server_addr.sin_addr.s_addr) <= 0) {
    perror("Invalid server address");
    exit(EXIT_FAILURE);
    }


    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    // do your behavior in here

    FILE* fp_txt;
    FILE* fp_img;
    char buf[1000];
    int txt_len;
    int img_len;
    int write_num;

    char* test_message = "Ph'nglui Mglw'nafh Cthulhu R'lyeh Wgah'nagl Fhtagn.";


    fp_txt = fopen("client_test.txt","r");
    fp_img = fopen("image_test.jpg","rb");



    if(send(client_sock, test_message, strlen(test_message),0) == -1){
            printf("err!");
    }

    if(send(client_sock, test_message, strlen(test_message),0) == -1){
            printf("err!");
    }


    fseek(fp_txt, 0, SEEK_END);
    txt_len = (int)ftell(fp_txt);
    printf("txtlen: %d\n", txt_len);

    if(send(client_sock, &txt_len, sizeof(int),0) == -1){
            printf("err!");
    }

    fseek(fp_img, 0, SEEK_END);
    img_len = (int)ftell(fp_img);
    printf("txtimg: %d\n", img_len);

    if(send(client_sock, &img_len, sizeof(int),0) == -1){
            printf("err!");
    }

    while(!feof(fp_txt)){
        fread(buf,1,sizeof(buf),fp_txt);
        write_num = send(client_sock, buf, sizeof(buf),0);
        if(write_num == -1){
            printf("err!");
            break;
        }
        
    }
    while(!feof(fp_img)){
        fread(buf,1,sizeof(buf),fp_img);
        write_num = send(client_sock, buf, sizeof(buf),0);
        if(write_num == -1){
            printf("err!");
            break;
        }
    }

    printf("completed!\n");

    fclose(fp_txt);
    fclose(fp_img);


    close(client_sock);
}
