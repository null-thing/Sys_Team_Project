#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int PORT_NUMBER = 8010; // set to desired port number
char* SERVER_IP_ADDRESS = "127.0.0.1"; //테스트용 노트북의 ip주소: rpi->노트북 자료 받음
int PI_SERIAL_NUM = 4;

/*  
    Server pi - bridge pi
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
    //printf("file_len = %d\n",file_len);

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
        //printf("file_1000_len = %d\n",file_len);


        fclose(fp_test_1000);
    }
    fclose(fp);
}

void recv_file_from_client(int client_socket){
    FILE* fp;
    int file_len, message_len, recv_bytes;
    char *message_contents, *header_divider;
    char recv_buf[1033], file_name[10],int_holder[10],file_category[3],pi_serial_num[10],write_num[10];
    

    //header information: [pi_serial_num][file_category][file_len][message_len][message_contents]
    memset(recv_buf, 0, sizeof(recv_buf));
    recv_bytes =  recv(client_socket, recv_buf, 1033, 0);
    if (recv_bytes<0){
        printf("err!");
	}


    header_divider = recv_buf;
    strncpy(pi_serial_num,recv_buf,10);
    strncpy(file_category,header_divider+10,3);
    strncpy(int_holder,header_divider+13,10);
    file_len = atoi(int_holder);
    strncpy(int_holder,header_divider+23,10);
    message_len = atoi(int_holder);
    message_contents = header_divider+33;
    strcpy(file_name, pi_serial_num);
    strcat(file_name, ".");
    strcat(file_name, file_category);


    if(!strcmp(file_category, "txt")){
        fp = fopen(file_name, "w");
    }else{
        fp = fopen(file_name, "wb");
    }
    //printf("filename:%s\n",file_name);
    //printf("filelen:%d\n",file_len);
    //printf("messagelen:%d\n",message_len);
    //printf("%x\n",recv_buf);
    //bin_printer(message_contents);

    fwrite(message_contents, sizeof(char), message_len,fp);

    
    while(message_len == 1000){
        memset(recv_buf, 0, sizeof(recv_buf));
        recv_bytes =  recv(client_socket, recv_buf, 1033, 0);
        if (recv_bytes<0){
            printf("err!");
	    }

        header_divider = recv_buf;
        strncpy(pi_serial_num,recv_buf,10);
        strncpy(file_category,header_divider+10,3);
        strncpy(int_holder,header_divider+13,10);
        file_len = atoi(int_holder);
        strncpy(int_holder,header_divider+23,10);
        message_len = atoi(int_holder);
        message_contents = header_divider+33;
        strcpy(file_name, pi_serial_num);
        strcat(file_name, ".");
        strcat(file_name, file_category);

        //printf("%x\n",recv_buf);
        //bin_printer(message_contents);

        fwrite(message_contents, sizeof(char), message_len,fp);
    }

    //printf("file recv complete!\n");
    fclose(fp);
}
int main(void) {
    int socket_desc , client_sock, client_size;
    FILE* fp_txt;
    FILE* fp_img;
    int write_num;
    int txt_len;
    int img_len;
    int recv_bytes;
    int recv_pid;
    int work_num = 0;
    int i;
    int* file_exist_arr[4];
    int client_socket;
    socklen_t client_address_length;
    int terminating_variable = 1;

    fp_txt = fopen("0000000001.txt","w");
    fp_img = fopen("0000000001.jpg","wb");
    fclose(fp_txt);
    fclose(fp_img);

    fp_txt = fopen("0000000002.txt","w");
    fp_img = fopen("0000000002.jpg","wb");
    fclose(fp_txt);
    fclose(fp_img);

    fp_txt = fopen("0000000003.txt","w");
    fp_img = fopen("0000000003.jpg","wb");
    fclose(fp_txt);
    fclose(fp_img);

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


    printf("###############################checkpoint 1\n");

    while(terminating_variable){
        if (listen(server_socket, 5) == -1) { // Maximum 5 pending connections
            perror("Socket listening failed");
            exit(EXIT_FAILURE);
        }

        client_address_length = sizeof(client_addr);

        // Accept the connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_address_length);
        if (client_socket == -1) {
            perror("Socket accepting failed");
            exit(EXIT_FAILURE);
        }

        // do your behavior in here
        recv_bytes = recv(client_socket, &recv_pid , sizeof(int) , 0);
        if (recv_bytes<=0){
            printf("Program terminated");
            close(client_socket);

            for(i=0;i<3;i++){
                client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_address_length);
                if (client_socket == -1) {
                    perror("Socket accepting failed");
                    exit(EXIT_FAILURE);
                }
                recv_bytes = recv(client_socket, &recv_pid , sizeof(int) , 0);
                if(send(client_socket, &terminating_variable, sizeof(int),0) == -1){
                    printf("err!");
                }
                close(client_socket);
            }
            break;
            //close conntction, get out of loop after terminating sensor connectd pi
	    }

        printf("###############################checkpoint 2\n");

        printf("pi num: %d\n",recv_pid);
        
        if((recv_pid>=1)&&(recv_pid<=3)){
            if(send(client_socket, &terminating_variable, sizeof(int),0) == -1){
                    printf("err!");
                }
            recv_file_from_client(client_socket);
            recv_file_from_client(client_socket);
            close(client_socket);
            printf("##################################checkpoint 2-1\n");

        }else{
            if(work_num == 0){
                //terminate connection
            }else{
                //send three file
                send_flie_to_server(client_socket, "0000000001.txt","txt");
                send_flie_to_server(client_socket, "0000000001.jpg","jpg");
                send_flie_to_server(client_socket, "0000000002.txt","txt");
                send_flie_to_server(client_socket, "0000000002.jpg","jpg");
                send_flie_to_server(client_socket, "0000000003.txt","txt");
                send_flie_to_server(client_socket, "0000000003.jpg","jpg");
            }
            close(client_socket);
        }

        printf("###############################checkpoint 3\n");
        printf("terminating_variable: %d\n",terminating_variable);
        scanf("input: %d",&terminating_variable); //input terminating variabe for test
        work_num = work_num + 1;
        printf("work_num:%d\n",work_num);
    }
    close(server_socket);
}