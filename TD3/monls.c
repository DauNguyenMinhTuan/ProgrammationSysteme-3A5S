#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#define LTEMPS 32

extern int errno;

void print_stat(const char *ref, struct stat *status){
    struct passwd *pw;
    struct group *grp;
    char type;
    char pws[20], grs[20], temps[LTEMPS];

    type = '?';
    if(S_ISREG(status->st_mode)){
        type = '-';
    }
    else if(S_ISDIR(status->st_mode)){
        type = 'd';
    }
    else if(S_ISCHR(status->st_mode)){
        type = 'c';
    }
    else if(S_ISBLK(status->st_mode)){
        type = 'b';
    }
    else if(S_ISFIFO(status->st_mode)){
        type = 'p';
    }

    strftime(temps, LTEMPS, "%a %e %h %Y %H:%M:%S", localtime(&(status->st_mtime)));

    pw = getpwuid(status->st_uid);
    if(pw != NULL){
        strcpy(pws, pw->pw_name);
    }
    else{
        sprintf(pws, "%8d", (int)status->st_uid);
    }

    grp = getgrgid(status->st_gid);
    if(grp != NULL){
        strcpy(grs, grp->gr_name);
    }
    else{
        sprintf(grs, "%8d", (int)status->st_gid);
    }

    printf("%c%c%c%c%c%c%c%c%c%c %2d %8s %8s %9d %s %s\n", type,
            status->st_mode&S_IRUSR ? 'r' : '-',
            status->st_mode&S_IWUSR ? 'w' : '-',
            status->st_mode&S_IXUSR ? 'x' : '-',
            status->st_mode&S_IRGRP ? 'r' : '-',
            status->st_mode&S_IWGRP ? 'w' : '-',
            status->st_mode&S_IXGRP ? 'x' : '-',
            status->st_mode&S_IROTH ? 'r' : '-',
            status->st_mode&S_IWOTH ? 'w' : '-',
            status->st_mode&S_IXOTH ? 'x' : '-',
            (int)status->st_nlink, pws, grs, (int)status->st_size, temps, ref);
}

void lsRp(char *file_name){
    struct dirent *dp;
    DIR *dirp;
    struct stat status;
    int count = 0;
    char **child_dir;
    child_dir = (char**)malloc(sizeof(char*) * 100);
    if((dirp = opendir(file_name)) == NULL){
        fprintf(stderr, "Could not open %s\n", file_name);
        return;
    }
    printf("%s:\n", file_name);
    while((dp = readdir(dirp))){
        printf("%s", dp->d_name);
        stat(dp->d_name, &status);
        
        if(S_ISDIR(status.st_mode) && strcmp(dp->d_name, "..") && strcmp(dp->d_name, ".")){
            printf("/");
            child_dir[count] = (char*)malloc(sizeof(char) * strlen(dp->d_name));
            strcpy(child_dir[count], dp->d_name);
            count++;
        }
        else{
            printf(" ");
        }
    }
    printf("\n");
    for(int i = 0; i < count; i++){
        printf("\n");
        lsRp(child_dir[i]);
    }
}

int main(){
    // ls -l part
    printf("ls -l\n");
    DIR *dirp;
    struct dirent *dp;
    struct stat status;
    if((dirp = opendir(".")) == NULL){
        perror("Could not open '.'\n");
        return 1;
    }
    while((dp = readdir(dirp))){
        if(stat(dp->d_name, &status) == -1){
            fprintf(stderr, "Cannot obtain status of %s\n", dp->d_name);
            continue;
        }
        print_stat(dp->d_name, &status);
    }
    if(errno != 0){
        perror("Error reading directory\n");
        return 1;
    }
    else{
        closedir(dirp);
    }

    // ls -Rp part
    printf("\n\nls -Rp\n");
    lsRp(".");

    return 0;
}