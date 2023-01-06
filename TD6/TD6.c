#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define STR_PERE "bar"
#define STR_FILS "foo"

int main(){
    pid_t status;
    printf("[%d] Je suis processus père\n", getpid());
    status = fork();
    char buffer[10];
    int fd;
    fd = open("dummy_file.txt", O_RDWR);
    switch(status){
        case -1:
            perror("Forking Error\n");
            exit(EXIT_FAILURE);
        case 0:
            printf("[%d] Je suis processus fils\n", getpid());
            lseek(fd, 3, SEEK_SET);
            write(fd, STR_FILS, strlen(STR_FILS));
            sleep(2);
            lseek(fd, 3, SEEK_SET);
            read(fd, buffer, 3);
            buffer[3] = '\0';
            printf("%s\n", buffer);
            break;
        default:
            int tmp = status;
            wait(&status);
            lseek(fd, 3, SEEK_SET);
            write(fd, STR_PERE, strlen(STR_PERE));
            sleep(1);
            lseek(fd, 3, SEEK_SET);
            read(fd, buffer, 3);
            buffer[3] = '\0';
            printf("%s\n", buffer);
            printf("[%d] J'ai engendre\n", getpid());
            printf("[%d] Mon fils est %d\n", getpid(), tmp);
    }
    printf("[%d] Je termine\n", getpid());
    exit(EXIT_SUCCESS);
}