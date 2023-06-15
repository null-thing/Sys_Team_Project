#include "interpret.h"
#include "function.h"


void read_data(Rcvdata* recieved_data, Client_info* client) {
    recieved_data->cli_info = client;
    recieved_data->image;
    recieved_data->text;
}

void interpret(Rcvdata* recieved_data) {
    int open = 0;
    int on = 0;
    // if ( ) {
    // }

    for (int i=0; i<MAX_CLIENTS;i++){
        if (!clients_interpreted[i].valid){
            clients_interpreted[i].valid = 1;
            clients_interpreted[i].cli_info = recieved_data->cli_info;
            clients_interpreted[i].door_opened = open;
            clients_interpreted[i].light_on =  on;
            clients_interpreted[i].image = recieved_data->image;
        }
    }
}