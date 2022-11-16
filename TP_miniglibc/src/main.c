#include "mini_lib.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
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

const char *command_name[] = {
    "mini_touch\0",
    "mini_cp",
    "mini_echo",
    "mini_cat",
    "mini_head",
    "mini_tail",
    "mini_clean",
    "mini_grep",
    "mini_wc",
    "exit",
    "\0"
};

void pre_mini_touch(char **argv, int argc){
    if(argc != 2){
        mini_printf("Not Right Syntax For ");
        mini_printf(argv[0]);
        mini_printf("\n");
        return;
    }
    int ok = 0;
    for(int i = 0; i < mini_strlen(argv[1]); i++){
        if(argv[1][i] == '/'){
            ok = 1;
            break;
        }
    }
    if(!ok){
        char *file_path = (char*)mini_calloc(sizeof(char), mini_strlen(argv[1]) + 3);
        file_path[0] = '.';
        file_path[1] = '/';
        for(int i = 0; i < mini_strlen(argv[1]); i++){
            file_path[i + 2] = argv[1][i];
        }
        file_path[mini_strlen(argv[1]) + 2] = '\0';
        mini_touch(file_path);
    }
    else{
        mini_touch(argv[1]);
    }
}

void pre_mini_cp(char **argv, int argc){
    if(argc != 3){
        mini_printf("Not Right Syntax For ");
        mini_printf(argv[0]);
        mini_printf("\n");
        return;
    }
    int ok;
    ok = 0;
    for(int i = 0; i < mini_strlen(argv[1]); i++){
        if(argv[1][i] == '/'){
            ok = 1;
            break;
        }
    }
    char *src_path = (char*)mini_calloc(sizeof(char), mini_strlen(argv[1]) + 3);
    if(!ok){
        src_path[0] = '.';
        src_path[1] = '/';
        for(int i = 0; i < mini_strlen(argv[1]); i++){
            src_path[i + 2] = argv[1][i];
        }
        src_path[mini_strlen(argv[1]) + 2] = '\0';
    }
    else{
        src_path = argv[1];
    }
    ok = 0;
    for(int i = 0; i < mini_strlen(argv[2]); i++){
        if(argv[2][i] == '/'){
            ok = 1;
            break;
        }
    }
    char *dest_path = (char*)mini_calloc(sizeof(char), mini_strlen(argv[2]) + 3);
    if(!ok){
        dest_path[0] = '.';
        dest_path[1] = '/';
        for(int i = 0; i < mini_strlen(argv[2]); i++){
            dest_path[i + 2] = argv[2][i];
        }
        dest_path[mini_strlen(argv[2]) + 2] = '\0';
    }
    else{
        dest_path = argv[2];
    }
    mini_cp(src_path, dest_path);
}

void pre_mini_echo(char **argv, int argc){
    for(int i = 1; i < argc; i++){
        mini_echo(argv[i]);
        mini_echo(" ");
    }
    mini_echo("\n");
}

void pre_mini_cat(char **argv, int argc){
    if(argc != 2){
        mini_printf("Not Right Syntax For ");
        mini_printf(argv[0]);
        mini_printf("\n");
        return;
    }
    int ok;
    ok = 0;
    for(int i = 0; i < mini_strlen(argv[1]); i++){
        if(argv[1][i] == '/'){
            ok = 1;
            break;
        }
    }
    char *src_path = (char*)mini_calloc(sizeof(char), mini_strlen(argv[1]) + 3);
    if(!ok){
        src_path[0] = '.';
        src_path[1] = '/';
        for(int i = 0; i < mini_strlen(argv[1]); i++){
            src_path[i + 2] = argv[1][i];
        }
        src_path[mini_strlen(argv[1]) + 2] = '\0';
    }
    else{
        src_path = argv[1];
    }
    mini_cat(src_path);
}

void pre_mini_head(char **argv, int argc){
    if(argc != 4){
        mini_printf("Not Right Syntax For ");
        mini_printf(argv[0]);
        mini_printf("\n");
        return;
    }
    int ok;
    ok = 0;
    for(int i = 0; i < mini_strlen(argv[3]); i++){
        if(argv[3][i] == '/'){
            ok = 1;
            break;
        }
    }
    char *src_path = (char*)mini_calloc(sizeof(char), mini_strlen(argv[3]) + 3);
    if(!ok){
        src_path[0] = '.';
        src_path[1] = '/';
        for(int i = 0; i < mini_strlen(argv[3]); i++){
            src_path[i + 2] = argv[3][i];
        }
        src_path[mini_strlen(argv[3]) + 2] = '\0';
    }
    else{
        src_path = argv[3];
    }
    int number_line = 0;
    for(int i = 0; i < mini_strlen(argv[2]); i++){
        number_line *= 10;
        number_line += argv[2][i] - '0';
    }
    mini_head(src_path, number_line);
}

void pre_mini_tail(char **argv, int argc){
    if(argc != 4){
        mini_printf("Not Right Syntax For ");
        mini_printf(argv[0]);
        mini_printf("\n");
        return;
    }
    int ok;
    ok = 0;
    for(int i = 0; i < mini_strlen(argv[3]); i++){
        if(argv[3][i] == '/'){
            ok = 1;
            break;
        }
    }
    char *src_path = (char*)mini_calloc(sizeof(char), mini_strlen(argv[3]) + 3);
    if(!ok){
        src_path[0] = '.';
        src_path[1] = '/';
        for(int i = 0; i < mini_strlen(argv[3]); i++){
            src_path[i + 2] = argv[3][i];
        }
        src_path[mini_strlen(argv[3]) + 2] = '\0';
    }
    else{
        src_path = argv[3];
    }
    int number_line = 0;
    for(int i = 0; i < mini_strlen(argv[2]); i++){
        number_line *= 10;
        number_line += argv[2][i] - '0';
    }
    mini_tail(src_path, number_line);
}

void pre_mini_clean(char **argv, int argc){
    if(argc != 2){
        mini_printf("Not Right Syntax For ");
        mini_printf(argv[0]);
        mini_printf("\n");
        return;
    }
    int ok;
    ok = 0;
    for(int i = 0; i < mini_strlen(argv[1]); i++){
        if(argv[1][i] == '/'){
            ok = 1;
            break;
        }
    }
    char *src_path = (char*)mini_calloc(sizeof(char), mini_strlen(argv[1]) + 3);
    if(!ok){
        src_path[0] = '.';
        src_path[1] = '/';
        for(int i = 0; i < mini_strlen(argv[1]); i++){
            src_path[i + 2] = argv[1][i];
        }
        src_path[mini_strlen(argv[1]) + 2] = '\0';
    }
    else{
        src_path = argv[1];
    }
    mini_clean(src_path);
}

void pre_mini_grep(char **argv, int argc){
    if(argc != 3){
        mini_printf("Not Right Syntax For ");
        mini_printf(argv[0]);
        mini_printf("\n");
        return;
    }
    int ok;
    ok = 0;
    for(int i = 0; i < mini_strlen(argv[2]); i++){
        if(argv[2][i] == '/'){
            ok = 1;
            break;
        }
    }
    char *src_path = (char*)mini_calloc(sizeof(char), mini_strlen(argv[2]) + 3);
    if(!ok){
        src_path[0] = '.';
        src_path[1] = '/';
        for(int i = 0; i < mini_strlen(argv[2]); i++){
            src_path[i + 2] = argv[2][i];
        }
        src_path[mini_strlen(argv[2]) + 2] = '\0';
    }
    else{
        src_path = argv[2];
    }
    mini_grep(argv[1], src_path);
}

void pre_mini_wc(char **argv, int argc){
    if(argc != 2){
        mini_printf("Not Right Syntax For ");
        mini_printf(argv[0]);
        mini_printf("\n");
        return;
    }
    int ok;
    ok = 0;
    for(int i = 0; i < mini_strlen(argv[1]); i++){
        if(argv[1][i] == '/'){
            ok = 1;
            break;
        }
    }
    char *src_path = (char*)mini_calloc(sizeof(char), mini_strlen(argv[1]) + 3);
    if(!ok){
        src_path[0] = '.';
        src_path[1] = '/';
        for(int i = 0; i < mini_strlen(argv[1]); i++){
            src_path[i + 2] = argv[1][i];
        }
        src_path[mini_strlen(argv[1]) + 2] = '\0';
    }
    else{
        src_path = argv[1];
    }
    mini_wc(src_path);
}

void (*pre_command[]) (char**, int) = {
    &pre_mini_touch,
    &pre_mini_cp,
    &pre_mini_echo,
    &pre_mini_cat,
    &pre_mini_head,
    &pre_mini_tail,
    &pre_mini_clean,
    &pre_mini_grep,
    &pre_mini_wc,
};

void run_command(char **argv, int argc){
    int cmd_id;
    int checker = 0;
    for(cmd_id = 0; mini_strcmp(command_name[cmd_id], "\0"); cmd_id++){
        if(!mini_strcmp(command_name[cmd_id], argv[0])){
            checker = 1;
            break;
        }
    }
    if(!checker){
        if(argc){
            mini_printf("Command Not Found!\n");
        }
        return;
    }
    if(!mini_strcmp(command_name[cmd_id], "exit")){
        mini_exit();
        return;
    }
    int status;
    pid_t pid, wait_pid;
    pid = fork();
    if(pid == 0){
        pre_command[cmd_id](argv, argc);
        mini_exit();
    }
    else{
        do{
            wait_pid = waitpid(pid, &status, WUNTRACED);
        }
        while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

int main(){
    show_instructions();
    char *command = (char*)mini_calloc(sizeof(char), BUFSIZ);
    while(1){
        char **argv = (char**)mini_calloc(sizeof(char*), 10);
        for(int i = 0; i < 10; i++){
            argv[i] = (char*)mini_calloc(sizeof(char), BUFSIZ);
        }
        int checker;
        if((checker = mini_scanf(command, BUFSIZ)) < 0){
            mini_perror("Error Reading Command!");
            return 1;
        }
        int start_pos, end_pos, argc = 0;
        start_pos = 0;
        end_pos = 0;
        int command_len = mini_strlen(command);
        while(start_pos < command_len){
            while(start_pos < command_len && command[start_pos] == ' '){
                start_pos++;
            }
            if(start_pos >= command_len){
                break;
            }
            end_pos = start_pos + 1;
            while(end_pos < command_len && (command[end_pos] != ' ' && command[end_pos] != '\n')){
                end_pos++;
            }
            for(int i = start_pos; i < end_pos; i++){
                argv[argc][i - start_pos] = command[i];
            }
            argv[argc][end_pos - start_pos] = '\0';
            // mini_printf(argv[argc]);
            argc++;
            start_pos = end_pos;
        }
        run_command(argv, argc);
    }
}