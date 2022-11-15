#include "mini_lib.h"
#include <stdio.h>

// void checklist(){
//     printf("Check file list: ");
//     FILE_LIST tmp = file_list;
//     while(tmp != NULL){
//         printf("%d ", tmp->my_file->fd);
//         tmp = tmp->next;
//     }
//     printf("\n");
// }

void show_instructions(){
    mini_printf("-------------------------------------------------------------------------------------------------\n");
    mini_printf("|                          Liste de commandes disponibles                                       |\n");
    mini_printf("| mini_touch file : cree un fichier vide file si il n’existe pas                                |\n");
    mini_printf("| mini_cp src dest : realise une copie du fichier src dans dest                                 |\n");
    mini_printf("| mini_echo chaine : affiche la chaine passee a l'ecran                                         |\n");
    mini_printf("| mini_cat fichier : affiche le contenu du fichier                                              |\n");
    mini_printf("| mini_head -n N fichier : affiche les N premieres lignes du fichier                            |\n");
    mini_printf("| mini_tail -n N fichier : affiche les N dernieres lignes du fichier                            |\n");
    mini_printf("| mini_clean file : cree un fichier vide file si il n’existe pas ou le remet a zero s'il existe |\n");
    mini_printf("| mini_grep mot fichier : affiche toutes les lignes contenant mot du fichier                    |\n");
    mini_printf("| mini_wc fichier : affiche le nombre de mots du fichier                                        |\n");
    mini_printf("| exit : quitte le shell                                                                        |\n");
    mini_printf("-------------------------------------------------------------------------------------------------\n");
}

int main(){
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
    
    // MYFILE *inp_file;
    // inp_file = mini_fopen("./src/mini_string.c", 'r');
    // char *str;
    // str = (char*)mini_calloc(1, 5000);
    // int cnt;
    // cnt = mini_fread(str, 1, 3000, inp_file);
    // MYFILE *out_file;
    // out_file = mini_fopen("./dummy.out", 'w');
    // mini_fwrite(str, 1, 3000, out_file);
    // mini_cp("./src/mini_string.c", "./dummy.out");

    // MYFILE *file1;
    // MYFILE *file2;
    // file1 = mini_fopen("./src/main.c", 'r');
    // file2 = mini_fopen("./dummy2.out", 'w');
    // int c = mini_fgetc(file1);
    // if(c < 0){
    //     mini_perror("fgetc Error!");
    //     return 1;
    // }
    // mini_fputc(file2, c);
    // mini_touch("./dummy3.out");
    // mini_echo("Here is a Makefile\n");
    // mini_cat("./Makefile");
    // mini_printf("\n");
    // mini_head("./src/main.c", 10);
    // mini_tail("./src/main.c", 10);
    // mini_clean("./dummy2.out");
    // mini_grep("include", "./dummy.out");
    // mini_wc("./dummy.out");
    show_instructions();
    char *command = (char*)mini_calloc(sizeof(char), BUFSIZ);
    char **args = (char**)mini_calloc(sizeof(char*), 10);
    for(int i = 0; i < 10; i++){
        args[i] = (char*)mini_calloc(sizeof(char), BUFSIZ);
    }
    while(1){
        int checker;
        if((checker = mini_scanf(command, BUFSIZ)) < 0){
            mini_perror("Error Reading Command!");
            return 1;
        }
        int start_pos = 0;
        while(start_pos < mini_strlen(command) && command[start_pos] == ' '){
            start_pos++;
        }
        int end_pos;
        for(end_pos = start_pos; end_pos < mini_strlen(command); end_pos++){
            if(command[end_pos] == ' '){
                break;
            }
        }
        for(int j = start_pos; j < end_pos; j++){
            args[j - start_pos] = command[j];
        }
        args[end_pos - start_pos] = '\0';
        
    }
    mini_exit();
}