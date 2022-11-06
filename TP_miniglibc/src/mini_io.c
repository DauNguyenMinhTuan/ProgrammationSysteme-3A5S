#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include "mini_lib.h"

#define IOBUFFER_SIZE 2048

MYFILE* mini_fopen(char *file, char mode){
    MYFILE *new_file;
    new_file = (MYFILE*)mini_calloc(sizeof(MYFILE), 1);
    if(new_file == NULL){
        mini_perror("Error!");
        return NULL;
    }
    new_file->buffer_read = (MYFILE*)mini_calloc(1, IOBUFFER_SIZE);
    new_file->buffer_write = (MYFILE*)mini_calloc(1, IOBUFFER_SIZE);
    if(new_file->buffer_read == NULL || new_file->buffer_write == NULL){
        perror("Initialization Error!");
        return NULL;
    }
    new_file->ind_read = new_file->ind_write = -1;
    switch(mode){
        case 'r':
            new_file->fd = open(file, O_RDONLY);
            break;
        case 'w':
            new_file->fd = open(file, O_WRONLY);
            break;
        case 'b':
            new_file->fd = open(file, O_RDWR);
            break;
        case 'a':
            new_file->fd = open(file, O_APPEND);
            break;
        default:
            perror("Invalid file mode!");
    }
    if(new_file->fd < 0){
        mini_perror("Error opening file!");
    }
    return new_file;
}

int mini_fread(void *buffer, int size_element, int number_element, MYFILE *file){
    if(file->ind_read == -1){
        file->buffer_read = mini_calloc(IOBUFFER_SIZE, 1);
        if(file->buffer_read == NULL){
            perror("Cannot Initiate Buffer!");
            return -1;
        }
        file->ind_read = 0;
    }
    int nb_read, ind;
    ind = 0;
    nb_read = size_element * number_element;
    while((file->ind_read = read(file->fd, file->buffer_read + file->ind_read,
                                ((nb_read < IOBUFFER_SIZE) ? nb_read : IOBUFFER_SIZE)))){
        if(file->ind_read < 0){
            mini_perror("Error Reading!");
            return -1;
        }
        nb_read -= ((nb_read < IOBUFFER_SIZE) ? nb_read : IOBUFFER_SIZE);
        for(int i = 0; i < file->ind_read; i++){
            *(((char*)buffer) + ind) = *((char*)(file->buffer_read) + i);
            ind++;
        }
        if(file->ind_read < IOBUFFER_SIZE){
            break;
        }
        else{
            file->ind_read = 0;
        }
    }
    return ind;
}