#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void socketconnect(){
	int socket_desc;
	struct sockaddr_in server;
	char *message , server_reply[2000];
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) printf("Could not create socket");
		
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8080 );

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
		puts("connect error");
		return 1;
	}
	
	puts("Connected\n");
	
	if (recv(socket_desc, server_reply , 2000 , 0) < 0){
		puts("recv failed");
	}
	// puts("Reply received\n");
	puts(server_reply);
    size_t reply_len = strlen(server_reply);
    int doornum, dooropen, lighton = process_byte_string((void*)server_reply, reply_len);
     FILE* file = fopen("door.txt", "w");
     if (file == NULL){
        printf("can't read file \n");
        return 1;
     }
     fprintf(file, "%d\n%d\n%d\n", doornum, dooropen, lighton);
     fclose(file);
}

int process_byte_string(const char* byte_string, size_t byte_string_length) {
    // Extracting values from the byte string using casting
    char sdoor[2];
    strncpy(sdoor, byte_string, 1);
    int door_number = atoi(sdoor);
    char sopen[2];
    strncpy(sopen, byte_string+1, 1);
    int door_open = atoi(sopen);
    char slight[2];
    strncpy(slight, byte_string+2, 1);
    int light_on = atoi(slight);

    char img[byte_string_length];
    strncpy(img, byte_string+4, byte_string_length-4);
    const char* image_data = img;

    // Saving the image as a jpg file
    FILE* image_file = fopen("image.jpg", "wb");
    if (image_file == NULL) {
        printf("Failed to open the image file\n");
        return;
    }

    fwrite(image_data, sizeof(char), byte_string_length - 3 * sizeof(int), image_file);
    fclose(image_file);

    // Printing the extracted values
    printf("Door Number: %d\n", door_number);
    printf("Door Open: %d\n", door_open);
    printf("Light On: %d\n", light_on);

    return door_number, door_open, light_on;
}

int start_door_num(){
    int num;
    printf("house door detector\n\n");
    printf("please insert your total door number\n");
    printf(">> ");
    scanf("%d", &num);
    if(num < 0 || num > 10){
        fprintf(stderr, "Error: Incorrect number entered. Program terminated.\n");
        exit(1);
    }
    return num;
}

// start from this line!!!!
void current_door_cli(const char* datapath, int dnum) { // if accept by one door one text
// main line, show {n}numbers of doors data, and current time. And you need to make request line about door history; you need to save each door history
    FILE* file = fopen(datapath, "r");
    if (file == NULL) {// if {n} door is not found
        printw(">> Door %i is not found\n", dnum);
        return;
    }
    int lightSensor, doorSensor, doornum;
    int isDoorOpen = 0;
    char doorstr[10];
    
    while (fscanf(file, "%d\n%d\n%d", &doornum, &lightSensor, &doorSensor) == 3) {
        if (lightSensor || doorSensor) {// 여기 door만 확실히 할건지 고민
            isDoorOpen = 1;
        }
        else{
            isDoorOpen = 0;
        }
        if (isDoorOpen == 1) {
            strcpy(doorstr, "open");
        }
        else {
            strcpy(doorstr, "closed");
        }
        time_t currentTime = time(NULL);
        char* timeString = ctime(&currentTime);
        struct tm* timeInfo = localtime(&currentTime);
        int seconds = timeInfo->tm_sec;

        if (seconds == 0 || seconds == 5){
            char str[1000];
            sprintf(str, "historyfile/history%d.txt", doornum);
            FILE* historyfile = fopen(str, "a");
            if (historyfile == NULL){
                printw("Can't write history\n");
            }
            char temp[1000];
            sprintf(temp, "%s                   %d           %d          %s\n", timeString, lightSensor, doorSensor, doorstr);
            char* final = strchr(temp, '\n');
            if (final != NULL){
                *final = '\0';
            }
            fprintf(historyfile, "%s          %d              %d         1   %s\n", temp, lightSensor, doorSensor, doorstr);
            fclose(historyfile);
        }

        printw(">> Door%d:       %d              %d           %s\n",doornum, lightSensor, doorSensor, doorstr);
    }
    fclose(file);
}

int history_reader(const char* datapath, int dnum){ // when you request {n}door history, open history data and show data
    FILE* file = fopen(datapath, "r");
    char quitcommand[100];
    clear();
    if (file == NULL) {// if {n} door history is not found
        printw("+---------------------------------------------------+\n");
        printw("|                                                   |");
        printw("\n|          Door %i's history is not found            |\n", dnum);
        printw("|                                                   |\n");
        printw("+---------------------------------------------------+");
        refresh();
        sleep(2);
        return 1;
    }
    
    printw("Door %d history \n", dnum);
    printw("time                         light sensor    door sensor   door open\n");
    char buffer[1000] = {0, };
    fread(buffer, 1, 1000, file);

    printw("%s", buffer);
    fclose(file);

    return 0;
}




int main() {
    socketconnect();


    const char* datapath = "testfile/door";
    printf("\n");
    // -----------------------door input session---------------------------
    clear();
    int totaldoornum = start_door_num();
    printf("Total door number: %d\n", totaldoornum);

    for (int i = 0; i < totaldoornum; i++){
        char str[1000];
        sprintf(str, "historyfile/history%d.txt", i+1);
        FILE* historyfile = fopen(str, "w");
        fclose(historyfile);
    }

    sleep(1);


    // -------------------current door session-------------------
    initscr(); // initialize curses library
    nodelay(stdscr, TRUE);
    int ch;
    refresh();

    while(1) {
        clear();
        move(0,0);
        printw("House Door Detector \n\n");
        printw("           light sensor    door sensor   door open\n");
        // ---------------each door print line-----------------------
        for (int i = 0; i < totaldoornum; i++){
            char temp[100];
            sprintf(temp, "%d.txt", i+1);
            char prepath[500];
            strcpy(prepath, datapath);
            strcat(prepath, temp);
            const char* totalpath = prepath;

            current_door_cli(prepath, i+1);
        }
        time_t currentTime = time(NULL);
        char* timeString = ctime(&currentTime);
        printw("\ncurrent time: %s \n\n", timeString);
        printw("-------------------------------------------------------\n");
        printw("insert {n} to see {n}door history, 'q' to quit\n");
        refresh();
        // you should make history write code

    // -------------------------- history print line ----------------------
        // you should update hitory.txt while history section is until open
        ch = getch();
        if (ch != ERR) {
            int number = 0;
            if (ch == 'q'){
                break;
                endwin();
            }
            if (ch >= '0' && ch <= '9'){
                number = ch - '0';
                while(1){
                    char* datapath = "historyfile/history";
                    char temp[100];
                    sprintf(temp, "%d.txt", number);
                    char prepath[500];
                    strcpy(prepath, datapath);
                    strcat(prepath, temp);

                    int quitsin = history_reader(prepath, number); // path rebuild needed
                    if (quitsin){
                        break;
                    }
                    time_t newtime = time(NULL);
                    char* newtimestring = ctime(&newtime);
                    printw("\n\ncurrent time: %s \n\n", newtimestring);
                    printw("\n----------------------------------------------------\n");
                    printw("insert 'q' to quit\n");
                    refresh();
                    ch = getch();
                    if(ch == 'q'){
                        break;
                    }
                    sleep(1);
                }
            }
            else{
                clear();
                printw("+---------------------------------------------------+\n");
                printw("|                                                   |");
                printw("\n|        please insert number, not alpahbet         |\n");
                printw("|                                                   |\n");
                printw("+---------------------------------------------------+");
                refresh();
                sleep(2);
            }
        }
        sleep(1); // reset cycle, not yet sellected
    }
        endwin();


    return 0;
}
