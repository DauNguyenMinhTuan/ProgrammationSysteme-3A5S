#include "mini_lib.h"
#include <stdio.h>

int main(int argc, char **argv){
    // void *ptr1 = mini_calloc(1, 200000000);
    // void *ptr2 = mini_calloc(100, 200000000);
    // mini_free(ptr1);
    // ptr2 = mini_calloc(1, 200000);
    // mini_printf("Sussy Baka\n");
    // char *msg;
    // msg = mini_calloc(sizeof(char), 20);
    // mini_scanf(msg, 20);
    // mini_printf(msg);
    // mini_scanf(msg, 10);
    // mini_printf(msg);
    MYFILE *my_file;
    my_file = mini_fopen("./src/mini_memory.c", 'r');
    char *str;
    str = (char*)mini_calloc(1, 5000);
    int cnt;
    cnt = mini_fread(str, 1, 3000, my_file);
    printf("%s\n%d\n", str, cnt);
    mini_exit();
    // exo30 fini
}