#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "mini_lib.h"

typedef struct malloc_element{
    void *allocated_zone;
    long long zone_size;
    int status;
    struct malloc_element *my_malloc;
} malloc_element;

malloc_element *malloc_list = NULL;

void *mini_calloc(int size_element, int number_element){
    if(size_element < 0){
        mini_perror("Invalid size of element!");
        return NULL;
    }
    if(number_element < 0){
        mini_perror("Invalid number of element!");
        return NULL;
    }
    if(size_element == 0 || number_element == 0){
        return NULL;
    }
    long long final_size = size_element;
    final_size *= (long long)number_element;
    malloc_element *my_list = malloc_list;
    while(my_list != NULL){
        if(!my_list->status && my_list->zone_size >= final_size){
            my_list->status = 1;
            return my_list->allocated_zone;
        }
        my_list = my_list->my_malloc;
    }
    void *buffer = sbrk(final_size);
    if(buffer == ((void *) -1)){
        mini_perror("Cannot allocate memory!");
        return NULL;
    }
    malloc_element *new_element = sbrk(sizeof(malloc_element));
    if(new_element == NULL){
        mini_perror("Cannot store information of allocated zone!");
        return NULL;
    }
    bzero(buffer, final_size);
    new_element->allocated_zone = buffer;
    new_element->status = 1;
    new_element->zone_size = final_size;
    new_element->my_malloc = malloc_list;
    malloc_list = new_element;
    return buffer;
}

void mini_free(void *ptr){
    if(ptr == NULL) return;
    malloc_element *my_list = malloc_list;
    while(my_list != NULL){
        if(my_list->allocated_zone == ptr){
            if(my_list->status){
                my_list->status = 0;
                break;
            }
        }
        my_list = my_list->my_malloc;
    }
}

void mini_exit(){
    while(file_list != NULL){
        mini_fclose(file_list->my_file);
    }
    _exit(0);
}