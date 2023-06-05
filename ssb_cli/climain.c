#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

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

int history_reader(int n){ // when you request {n}door history, open history data and show data
    return 0;
}


int main() {
    const char* datapath = "C:\\Users\\ssbnn\\Desktop\\C++\\SP_2023\\Sys_Team_Project\\ssb_cli\\testfolder\\door";
    printf("\n");
    // -----------------------door input session---------------------------
    system("cls");
    int totaldoornum = start_door_num();
    printf("Total door number: %d\n", totaldoornum);
    sleep(2);

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

        // ------------------time print line-----------------------
        time_t currentTime = time(NULL);
        char* timeString = ctime(&currentTime);
        printf("current time: %s \n\n", timeString);
        printf("-------------------------------------------------------");
        sleep(5); // reset cycle, not yet sellected
        // ------------------command input line

    }


    return 0;
}

