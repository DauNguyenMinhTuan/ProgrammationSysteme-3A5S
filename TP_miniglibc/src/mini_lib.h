#ifndef MINI_LIB_H
#define MINI_LIB_H

typedef struct MYFILE{
    int fd;
    void *buffer_read;
    void *buffer_write;
    int ind_read;
    int ind_write;
} MYFILE;

void *mini_calloc(int size_element, int number_element);

void mini_free(void *ptr);

void mini_exit();

void mini_printf(char *msg);

int mini_strlen(char *str);

int mini_scanf(char *buf, int size_buffer);

int mini_strcpy(char *src, char *dest);

int mini_strcmp(char *s1, char *s2);

void mini_perror(char *message);

MYFILE* mini_fopen(char *file, char mode);

int mini_fread(void *buffer, int size_element, int number_element, MYFILE *file);

#endif