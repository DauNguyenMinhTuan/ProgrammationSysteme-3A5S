#include <unistd.h>
#include "mini_lib.h"
#include <stdio.h>
#include <errno.h>

#define BUF_SIZE 1024

static char *buffer;
static int ind = -1;

extern int errno;

void mini_printf(char *msg){
    if(ind == -1){
        buffer = (char*)mini_calloc(sizeof(char), BUF_SIZE);
        ind = 0;
    }
    // printf("%d\n", mini_strlen(msg));
    for(int i = 0; i < mini_strlen(msg); i++){
        if(ind == BUF_SIZE || msg[i] == '\n'){
            if(write(1, buffer, ind) < 0){
                mini_perror("Printing Error!");
            }
            ind = 0;
            if(msg[i] == '\n'){
                if(write(1, "\n", 1) < 0){
                    mini_perror("Printing Error!");
                }
            }
        }
        if(msg[i] != '\n'){
            buffer[ind++] = msg[i];
        }
    }
    if(ind){
        if(write(1, buffer, ind) < 0){
            mini_perror("Printing Error!");
        }
        ind = 0;
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
    int res;
    for(res = 0; res < size_buffer; res++){
        if(read(0, buf + res, 1) < 0){
            mini_perror("Error!");
        }
        else if(*(buf + res) == '\n'){
            break;
        }
    }
    if(res < size_buffer){
        buf[res] = '\0';
    }
    else{
        buf[size_buffer - 1] = '\0';
        res = size_buffer - 1;
        void *ptr;
        int checker;
        while((checker = read(0, ptr, 1))){
            if(checker < 0){
                mini_perror("Error!");
                return -1;
            }
            if(*((char*)ptr) == '\n'){
                break;
            }
        }
    }
    return res;
}

int mini_strcpy(char *s, char *d){
    for(int i = 0; i <= mini_strlen(s); i++){
        d[i] = s[i];
    }
    return mini_strlen(s) + 1;
}

int mini_strcmp(const char *s1, char *s2){
    int i = 0;
    while(s1[i] != '\0' && s2[i] != '\0'){
        if(s1[i] != s2[i]){
            return s1[i] - s2[i];
        }
        i++;
    }
    return s1[i] - s2[i];
}

void mini_perror(char *message){
    write(2, message, mini_strlen(message));
    write(2, strerror(errno), mini_strlen(strerror(errno)));
}