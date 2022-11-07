#ifndef MINI_LIB_H
#define MINI_LIB_H

typedef struct MYFILE_{
    int fd;
    void *buffer_read;
    void *buffer_write;
    int ind_read;
    int ind_write;
} MYFILE;

typedef struct FILE_LIST_ELEMENT_{
    MYFILE *my_file;
    struct FILE_LIST_ELEMENT_ *next;
} FILE_LIST_ELEMENT;

typedef FILE_LIST_ELEMENT* FILE_LIST;

extern FILE_LIST file_list;

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

int mini_fwrite(void *buffer, int size_element, int number_element, MYFILE *file);

int mini_fflush(MYFILE *file);

int mini_fclose(MYFILE *file);

int mini_fgetc(MYFILE *file);

int mini_fputc(MYFILE *file, char c);

void mini_touch(char *file);

#endif