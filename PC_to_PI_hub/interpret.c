#include "interpret.h"
#include "function.h"


void read_data(Rcvdata* recieved_data, Client_info* client) {
    recieved_data->cli_info = client;
    recieved_data->image;
    recieved_data->text;
}

void interpret(Rcvdata* recieved_data, char**data) {
     //header information: [pi_serial_num][file_category][file_len][message_len][message_contents]
    int index = 0;
    char file_name[10];
    char category[3];
    char pi_name[10];
    char data_len[10];
    char array[1000];
    for (int i=0; i<sizeof(recieved_data->text); i++){
        if (i<10){
           file_name[i] = recieved_data->text[i];
        } else if (i<13) {
            category[i-10]= recieved_data->text[i];
        } else if (i<23) {
            pi_name[i-13] = recieved_data->text[i];
        } else if (i<33) {
            data_len[i-23] = recieved_data -> text[i];
        } else{
            array[i-33] = recieved_data->text[i]; 
        }
    }

    //category?
    

    strncpy(shared_data, recieved_data->cli_info->uid, sizeof(shared_data) -1);
    strncpy(shared_data, recieved_data->text, sizeof(shared_data) -1);
    strncpy(shared_data, recieved_data->image, sizeof(shared_data) -1);

}