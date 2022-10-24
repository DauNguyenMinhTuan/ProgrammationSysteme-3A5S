#include <unistd.h>
#include "mini_lib.h"

#define BUF_SIZE 1024

static char *buffer;
static int ind = -1;

void mini_printf(char *msg){
    if(ind == -1){
        buffer = (char*)mini_calloc(BUF_SIZE, 1);
        ind = 0;
    }
    for(int i = 0; i <= mini_strlen(msg); i++){
        if(ind == BUF_SIZE || msg[i] == '\n' || msg[i] == '\0'){
            if(write(1, buffer, ind) < 0){
                perror("Printing Error!");
            }
            ind = 0;
            if(msg[i] == '\n'){
                if(write(1, "\n", 1) < 0){
                    perror("Printing Error!");
                }
            }
        }
        if(msg[i] != '\n'){
            buffer[ind++] = msg[i];
        }
    }
}

int mini_strlen(char *str){
    int res = 0;
    while(str[res] != '\0'){
        res++;
    }
    return res;
}

int mini_scanf(char *buf, int size_buffer){
    int res = read(0, buf, size_buffer);
    if(res < 0){
        perror("Reading Error!");
    }
    return res;
}