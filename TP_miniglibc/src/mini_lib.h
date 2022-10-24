#ifndef MINI_LIB_H
#define MINI_LIB_H

void *mini_calloc(int size_element, int number_element);

void mini_free(void *ptr);

void mini_exit();

void mini_printf(char *msg);

int mini_strlen(char *str);

int mini_scanf(char *buf, int size_buffer);

#endif