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
    MYFILE *inp_file;
    inp_file = mini_fopen("./src/mini_string.c", 'r');
    char *str;
    str = (char*)mini_calloc(1, 5000);
    int cnt;
    cnt = mini_fread(str, 1, 3000, inp_file);
    MYFILE *out_file;
    out_file = mini_fopen("./dummy.out", 'w');
    mini_fwrite(str, 1, 3000, out_file);
    MYFILE *file1;
    MYFILE *file2;
    file1 = mini_fopen("./src/main.c", 'r');
    file2 = mini_fopen("./dummy2.out", 'w');
    int c = mini_fgetc(file1);
    if(c < 0){
        mini_perror("fgetc Error!");
        return 1;
    }
    mini_fputc(file2, c);
    mini_touch("./dummy3.out");
    mini_exit();
    // exo35 fini
}