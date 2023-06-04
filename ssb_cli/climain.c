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
int current_door_cli(const char* datapath) { 
// main line, show {n}numbers of doors data, and current time. And you need to make request line about door history; you need to save each door history
    FILE* file = fopen(datapath, "r");

    if (file == NULL) {
        printf("Failed to open data.txt\n");
        return 0;
    }
    
    int lightSensor, doorSensor;
    int isDoorOpen = 0;
    
    while (fscanf(file, "%d %d", &lightSensor, &doorSensor) == 2) {
        if (lightSensor || doorSensor) {
            isDoorOpen = 1;
            break;
        }
    }
    fclose(file);

    return isDoorOpen;
}

int history_reader(int n){ // when you request {n}door history, open history data and show data

}


int main() {
    const char* datapath = "C:\\Users\\ssbnn\\Desktop\\C++\\SP_2023\\Sys_Team_Project\\ssb_cli\\data.txt";
    printf("\n");
    system("cls");
    int testnum = start_door_num();
    system("cls");
    printf("Total door number: %d\n", testnum);
    return 0;
}

