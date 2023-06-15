#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int PI_SERIAL_NUM = 1;
int PORT_NUMBER = 8000; // set to desired port number
char* SERVER_IP_ADDRESS = "127.0.0.1"; //테스트용 노트북의 ip주소: rpi->노트북 자료 전송

/*  
    client pi - sensor pi
*/

void bin_printer(char* input_val){
    int i;
    for(i=0;i<1000;i++){
        printf("%x",input_val[i]);
    }
    printf("\n");
}

void send_flie_to_server(int client_socket, char* file_name, char* file_category){
    FILE* fp;
    FILE* fp_test_1000;
    int file_len, message_len, write_num;
        int mismatch = 0;
    char message_contents[1000],header_message_contents[1033];

    //header information: [pi_serial_num][file_category][file_len][message_len][message_contents]
    if(!strcmp(file_category, "txt")){
        fp = fopen(file_name, "r");
    }else{
        fp = fopen(file_name, "rb");
    }
    
    if(!strcmp(file_category,"jpg")){
        fp_test_1000 = fopen("image_1000.jpg","wb");
    }


    fseek(fp, 0, SEEK_END);
    file_len = (int)ftell(fp);
    fseek(fp, 0, SEEK_SET);
    printf("file_len = %d\n",file_len);

    while (!feof(fp))
    {
        memset(header_message_contents, 0, sizeof(header_message_contents));
        memset(message_contents, 0, sizeof(message_contents));
        message_len = fread(message_contents,sizeof(char),sizeof(message_contents),fp);
        snprintf(header_message_contents,1033,"%010d%s%010d%010d",PI_SERIAL_NUM,file_category,file_len,message_len);
        memcpy(header_message_contents+33, message_contents, message_len);
        write_num = send(client_socket, header_message_contents, sizeof(header_message_contents),0);
        if(write_num == -1){
            printf("err!");
            break;
        }

        printf("%s",message_contents);

        if(!strcmp(file_category,"jpg")){
            fwrite(header_message_contents+33, sizeof(char), message_len,fp_test_1000);
        }
    }


    printf("\n");

    if(!strcmp(file_category,"jpg")){
        fseek(fp_test_1000, 0, SEEK_END);
        file_len = (int)ftell(fp_test_1000);
        fseek(fp_test_1000, 0, SEEK_SET);
        printf("file_1000_len = %d\n",file_len);


        fclose(fp_test_1000);
    }
    fclose(fp);
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
    char* txt_name = "client_test.txt";
    char* img_name = "image_test.jpg";
/*    char* test_message = "Ph'nglui Mglw'nafh Cthulhu R'lyeh Wgah'nagl Fhtagn.";


    if(send(client_sock, test_message, strlen(test_message),0) == -1){
            printf("err!");
    }

*/  int i = 0;
    int is_exit = 1;
    char *message = "connection?";
while(is_exit){
    i = i+ 1;
    if(i%2 == 1){
    send_flie_to_server(client_sock,txt_name,"txt");
    }else{    
    //send_flie_to_server(client_sock,img_name,"jpg");
    i = 0;
    }
    //if( send(client_sock , message , strlen(message) , 0) < 0){
	//	puts("Send failed");
    //    is_exit = 0;
	//	break;	}

    printf("completed!\n");
}

    close(client_sock);
}