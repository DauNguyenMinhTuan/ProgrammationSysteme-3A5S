#include "mini_lib.h"

int main(int argc, char **argv){
    // void *ptr1 = mini_calloc(1, 200000000);
    // void *ptr2 = mini_calloc(100, 200000000);
    // mini_free(ptr1);
    // ptr2 = mini_calloc(1, 200000);
    mini_printf("Sussy Baka\n");
    char *msg;
    msg = mini_calloc(sizeof(char), 20);
    mini_scanf(msg, 20);
    mini_printf(msg);
    mini_scanf(msg, 10);
    mini_printf(msg);
    mini_exit();
}