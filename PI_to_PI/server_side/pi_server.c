#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

int PI_PORT_NUMBER_0 = 8010; //pc
int PI_PORT_NUMBER_1 = 8000; //pi1
int PI_PORT_NUMBER_2 = 8001; //pi2
int PI_PORT_NUMBER_3 = 8002; //pi3

int is_exit = 1;

char* SERVER_IP_ADDRESS = "127.0.0.1"; //테스트용 노트북의 ip주소: rpi->노트북 자료 받음


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
    printf("filename:%s\n",file_name);
    printf("filelen:%d\n",file_len);
    printf("messagelen:%d\n",message_len);
    //printf("%x\n",recv_buf);
    //bin_printer(message_contents);

    fwrite(message_contents, sizeof(char), message_len,fp);

    
    while(message_len == 1000){
        memset(recv_buf, 0, sizeof(recv_buf));
        recv_bytes =  recv(client_socket, recv_buf, 1033, 0);
        if (recv_bytes<0){
            printf("err!");
            break;
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
    printf("filename:%s\n",file_name);
    printf("filelen:%d\n",file_len);
    printf("messagelen:%d\n",message_len);
    //printf("%x\n",recv_buf);
    //bin_printer(message_contents);

        fwrite(message_contents, sizeof(char), message_len,fp);
    }

    //printf("file recv complete!\n");
    fclose(fp);
}


void send_file_to_server(int client_socket, char* file_name, char* file_category, int serial_num){
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
        snprintf(header_message_contents,1033,"%010d%s%010d%010d",serial_num,file_category,file_len,message_len);
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




void *pi_to_pi(void *socket_desc){
    int client_desc = *(int*)socket_desc;

    printf("thread started\n");
    while(is_exit){
           recv_file_from_client(client_desc);
        //recv_file_from_client(client_desc);
        printf("doing\n");
    }

    free(socket_desc);
    printf("thread end\n");

}

void *pi_to_pc(void *sock){
    char *message = "connection?";
    char filename[20];
    int socket_desc = *(int*)sock;
    int i;
    printf("thread started\n");
    while(is_exit){
        
        //if( send(socket_desc , message , strlen(message) , 0) < 0){
		//    puts("Send failed");
        //    is_exit = 0;
		//    break;
	    //}
        
        //scanf("is_exit: %d",&is_exit); //code for test
        /*
        for(i = 1 ; i<=3 ; i++){
            snprintf(filename,20,"%010d.txt",i);
            send_file_to_server(socket_desc,filename,"txt",i);

            snprintf(filename,20,"%010d.jpg",i);
            send_file_to_server(socket_desc,filename,"jpg",i);
        }
        */
        
        //printf("pipc\n");
    }
    free(sock);
    printf("thread end\n");

}





int main(void) {

    pthread_t thread[10];
    FILE* fp_txt;
    FILE* fp_img;
    char buf[1000];
    int write_num;
    int txt_len;
    int img_len;
    int recv_bytes;
    int i;
    int thread_hold;


    int socket_desc;

    int server_socket_PC = socket(AF_INET, SOCK_STREAM, 0);
    int server_socket_PI_1 = socket(AF_INET, SOCK_STREAM, 0);
    int server_socket_PI_2 = socket(AF_INET, SOCK_STREAM, 0);
    int server_socket_PI_3 = socket(AF_INET, SOCK_STREAM, 0);

    int client_socket_PC, client_socket_PI_1 , client_socket_PI_2 , client_socket_PI_3;

    struct sockaddr_in server_addr_PC, server_addr_PI_1, server_addr_PI_2, server_addr_PI_3;
    struct sockaddr_in client_addr_PC, client_addr_PI_1, client_addr_PI_2, client_addr_PI_3;
    socklen_t client_address_length_PC = sizeof(client_addr_PC);
    socklen_t client_address_length_PI_1 = sizeof(client_addr_PI_1);
    socklen_t client_address_length_PI_2 = sizeof(client_addr_PI_2);
    socklen_t client_address_length_PI_3 = sizeof(client_addr_PI_3);

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

    if (server_socket_PC == -1) {
        perror("Socket creation failed");
        //exit(EXIT_FAILURE);
    }

    if (server_socket_PI_1 == -1) {
        perror("Socket creation failed");
        //exit(EXIT_FAILURE);
    }

    if (server_socket_PI_2 == -1) {
        perror("Socket creation failed");
        //exit(EXIT_FAILURE);
    }


    if (server_socket_PI_3 == -1) {
        perror("Socket creation failed");
        //exit(EXIT_FAILURE);
    }

    printf("checkpoint 0\n");

    memset(&server_addr_PI_1, 0, sizeof(server_addr_PC));
    memset(&server_addr_PI_1, 0, sizeof(server_addr_PI_1));
    memset(&server_addr_PI_1, 0, sizeof(server_addr_PI_2));
    memset(&server_addr_PI_1, 0, sizeof(server_addr_PI_3));

printf("checkpoint 1\n");

    server_addr_PC.sin_family = AF_INET;
    server_addr_PC.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    server_addr_PC.sin_port = htons(PI_PORT_NUMBER_0); // Set the desired port number

    server_addr_PI_1.sin_family = AF_INET;
    server_addr_PI_1.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    server_addr_PI_1.sin_port = htons(PI_PORT_NUMBER_1); // Set the desired port number

    server_addr_PI_2.sin_family = AF_INET;
    server_addr_PI_2.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    server_addr_PI_2.sin_port = htons(PI_PORT_NUMBER_2); // Set the desired port number

    server_addr_PI_3.sin_family = AF_INET;
    server_addr_PI_3.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    server_addr_PI_3.sin_port = htons(PI_PORT_NUMBER_3); // Set the desired port number

printf("checkpoint 2\n");


    if (bind(server_socket_PC, (struct sockaddr *) &server_addr_PC, sizeof(server_addr_PC)) == -1) {
        perror("Socket binding failed");
        //exit(EXIT_FAILURE);
    }

    if (bind(server_socket_PI_1, (struct sockaddr *) &server_addr_PI_1, sizeof(server_addr_PI_1)) == -1) {
        perror("Socket binding failed");
        //exit(EXIT_FAILURE);
    }

    if (bind(server_socket_PI_2, (struct sockaddr *) &server_addr_PI_2, sizeof(server_addr_PI_2)) == -1) {
        perror("Socket binding failed");
        //exit(EXIT_FAILURE);
    }

    if (bind(server_socket_PI_3, (struct sockaddr *) &server_addr_PI_3, sizeof(server_addr_PI_3)) == -1) {
        perror("Socket binding failed");
        //exit(EXIT_FAILURE);
    }

    if (listen(server_socket_PC, 5) == -1) { // Maximum 5 pending connections
        perror("Socket listening failed");
        //exit(EXIT_FAILURE);
    }

    if (listen(server_socket_PI_1, 5) == -1) { // Maximum 5 pending connections
        perror("Socket listening failed");
        //exit(EXIT_FAILURE);
    }

    if (listen(server_socket_PI_2, 5) == -1) { // Maximum 5 pending connections
        perror("Socket listening failed");
        //exit(EXIT_FAILURE);
    }

    if (listen(server_socket_PI_3, 5) == -1) { // Maximum 5 pending connections
        perror("Socket listening failed");
        //exit(EXIT_FAILURE);
    }


printf("checkpoint 3\n");

    // Accept the connection

    /*
    client_socket_PC = accept(server_socket_PC, (struct sockaddr *)&client_addr_PC, &client_address_length_PC);
    if (client_socket_PC == -1) {
        printf("ttt");
        perror("Socket accepting failed");
        //exit(EXIT_FAILURE);
    }
    */

    client_socket_PI_1 = accept(server_socket_PI_1, (struct sockaddr *)&client_addr_PI_1, &client_address_length_PI_1);
    if (client_socket_PI_1 == -1) {
        perror("Socket accepting failed");
        //exit(EXIT_FAILURE);
    }

    printf("checkpoint 3.3\n");

/*
    client_socket_PI_2 = accept(server_socket_PI_2, (struct sockaddr *)&client_addr_PI_2, &client_address_length_PI_2);
    if (client_socket_PI_2 == -1) {
        perror("Socket accepting failed");
        //exit(EXIT_FAILURE);
    }

    client_socket_PI_3 = accept(server_socket_PI_3, (struct sockaddr *)&client_addr_PI_3, &client_address_length_PI_3);
    if (client_socket_PI_3 == -1) {
        perror("Socket accepting failed");
        //exit(EXIT_FAILURE);
    }
*/
    // do your behavior in here


    printf("checkpoint 3.6\n");
    /*
    recv_bytes = recv(client_socket_PI_1, buf , 1000 , 0);
    if (recv_bytes<0){
        printf("err!");
	}
    buf[recv_bytes] = '\0';
*/

    printf("checkpoint4\n");
    thread_hold = pthread_create(&thread[0],NULL,pi_to_pc,(void*)&client_socket_PC);
    thread_hold = pthread_create(&thread[1],NULL,pi_to_pi,(void*)&client_socket_PI_1);
 //   thread_hold = pthread_create(&thread[2],NULL,pi_to_pi,(void*)&client_socket_PI_2);
 //   thread_hold = pthread_create(&thread[3],NULL,pi_to_pi,(void*)&client_socket_PI_3);



    thread_hold = pthread_join(thread[0],NULL);
    for(int i=1; i<2; i++){
        thread_hold = pthread_join(thread[i],NULL);
        printf("joined\n");
    }

    close(client_socket_PC);
    close(client_socket_PI_1);
    close(client_socket_PI_2);
    close(client_socket_PI_3);
    close(server_socket_PC);
    close(server_socket_PI_1);
    close(server_socket_PI_2);
    close(server_socket_PI_3);
}