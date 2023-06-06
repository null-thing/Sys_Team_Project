#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <conio.h>

int start_door_num(){
    int num;
    printf("house door detector\n\n");
    printf("please insert your total door number\n");
    printf("<< ");
    
    scanf("%d", &num);
    return num;
}

// start from this line!!!!
void current_door_cli(const char* datapath, int dnum) { // if accept by one door one text
// main line, show {n}numbers of doors data, and current time. And you need to make request line about door history; you need to save each door history
    FILE* file = fopen(datapath, "r");
    if (file == NULL) {// if {n} door is not found
        printf(">> Door %i is not found\n", dnum);
        return;
    }
    int lightSensor, doorSensor, doornum;
    int isDoorOpen = 0;
    
    while (fscanf(file, "%d\n%d\n%d", &doornum, &lightSensor, &doorSensor) == 3) {
        if (lightSensor || doorSensor) {// 여기 door만 확실히 할건지 고민
            isDoorOpen = 1;
        }
        else{
            isDoorOpen = 0;
        }
        printf(">> Door%d:       %d              %d             %d\n",doornum, lightSensor, doorSensor, isDoorOpen);
    }
    fclose(file);
}

int history_reader(const char* datapath, int dnum){ // when you request {n}door history, open history data and show data
    FILE* file = fopen(datapath, "r");
    char quitcommand[100];
    system("cls");
    if (file == NULL) {// if {n} door history is not found
        printf(">> Door %i's history is not found\n", dnum);
        return 0;
    }
    printf("Door %d history \n", dnum);
    printf("time                         light sensor    door sensor   door open\n");
    char buffer[1000] = {0, };
    fread(buffer, 1, 1000, file);

    printf("%s", buffer);
    fclose(file);
    printf("\n\n\n----------------------------------------------------\n");
    printf("if you want to quit, please insert Quit\n$command line >> ");
    scanf("%s", quitcommand);
    if (strcmp(quitcommand, "Quit") == 0){
        return 1;
    }
    return 0;
}


int main() {
    const char* datapath = "C:\\Users\\ssbnn\\Desktop\\C++\\SP_2023\\Sys_Team_Project\\ssb_cli\\testfolder\\door";
    printf("\n");
    // -----------------------door input session---------------------------
    system("cls");
    int totaldoornum = start_door_num();
    printf("Total door number: %d\n", totaldoornum);
    sleep(1);

    // -------------------current door session-------------------
    for(; ;){
        system("cls");
        printf("House Door Detector \n");
        printf("           light sensor    door sensor   door open\n");
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
        printf("\ncurrent time: %s \n\n", timeString);
        printf("-------------------------------------------------------\n");
        printf("$command line >> ");
            // you should make history write code

    // -------------------------- history print line ----------------------
        // you should update hitory.txt while history section is until open
        if (kbhit()){
            char input[100];
            fgets(input, sizeof(input),stdin);
            strtok(input, "-");
            char doorcheck[100];
            int history_door_num;
            
            sscanf(input, "%s %d", doorcheck, &history_door_num);
            printf("input pharase: %s \ndoor number: %d\n", doorcheck, history_door_num);

            if (strcmp(doorcheck, "door_history") == 0){
                for(; ;){
                    int quitsin = history_reader("C:\\Users\\ssbnn\\Desktop\\C++\\SP_2023\\Sys_Team_Project\\ssb_cli\\testfolder\\history1.txt", history_door_num); // path rebuild needed
                    if (quitsin){
                        break;
                    }
                }

                sleep(5);
            }
            else{
                system("cls");
                printf("\n command error: please insert 'door_history {n}'\n");
            }
        }

        sleep(2); // reset cycle, not yet sellected

    }


    return 0;
}

