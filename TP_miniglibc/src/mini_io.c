#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mini_lib.h"

#define IOBUFFER_SIZE 2048

FILE_LIST file_list = NULL;

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
            new_file->fd = open(file, O_WRONLY | O_TRUNC);
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
    if(new_file->fd <= 0){
        mini_perror("Error opening file!");
        return NULL;
    }
    // printf("open %d\n", new_file->fd);
    FILE_LIST_ELEMENT *new_file_list_element = (FILE_LIST)mini_calloc(sizeof(FILE_LIST_ELEMENT), 1);
    if(new_file_list_element == NULL){
        mini_perror("File List Error!");
        return NULL;
    }
    new_file_list_element->my_file = new_file;
    new_file_list_element->next = file_list;
    file_list = new_file_list_element;
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

int mini_fwrite(void *buffer, int size_element, int number_element, MYFILE *file){
    if(file->ind_write == -1){
        file->buffer_write = mini_calloc(1, IOBUFFER_SIZE);
        if(file->buffer_write == NULL){
            perror("Cannot Initiate Buffer!");
            return -1;
        }
        file->ind_write = 0;
    }
    int nb_write = mini_strlen(buffer);
    nb_write = (nb_write < size_element * number_element) ? nb_write : size_element * number_element;
    for(int i = 0; i < nb_write; i++){
        *((char*)(file->buffer_write) + file->ind_write) = *(((char*)buffer) + i);
        file->ind_write++;
        if(file->ind_write == IOBUFFER_SIZE){
            int checker = write(file->fd, file->buffer_write, IOBUFFER_SIZE);
            if(checker < 0){
                mini_perror("Error Writing!");
                return -1;
            }
            file->ind_write = 0;
        }
    }
    return nb_write;
}

int mini_fflush(MYFILE *file){
    int nb_write = write(file->fd, file->buffer_write, file->ind_write);
    if(nb_write < 0){
        mini_perror("Error Flushing!");
        return -1;
    }
    bzero(file->buffer_write, IOBUFFER_SIZE);
    file->ind_write = 0;
    return nb_write;
}

int mini_fclose(MYFILE *file){
    if(file->ind_write >= 0 && mini_fflush(file) < 0){
        mini_perror("Error Flushing!");
        return -1;
    }
    if(file_list == NULL){
        mini_perror("Cannot Find File In File List!");
        return -1;
    }
    if(file_list->my_file == file){
        file_list = file_list->next;
    }
    else{
        FILE_LIST tmp = file_list;
        while(tmp->next != NULL && tmp->next->my_file != file){
            tmp = tmp->next;
        }
        if(tmp->next->my_file == file){
            tmp->next = tmp->next->next;
            mini_free(tmp->next);
        }
    }
    if(close(file->fd) < 0){
        mini_perror("Error Closing File!");
        return -1;
    }
    return 0;
}

int mini_fgetc(MYFILE *file){
    char *res;
    res = mini_calloc(1, 1);
    if(read(file->fd, res, 1) < 0){
        mini_perror("Reading Error!");
        return -1;
    }
    return *res;
}

int mini_fputc(MYFILE *file, char c){
    if(write(file->fd, &c, 1) < 0){
        mini_perror("Printing Error!");
        return -1;
    }
    return 0;
}

void mini_touch(char *file){
    int fd = open(file, O_EXCL | O_CREAT);
    if(fd <= 0){
        mini_perror("File Already Exists!");
        return;
    }
    int ok = close(fd);
    if(ok < 0){
        mini_perror("Error Closing File!");
        return;
    }
}