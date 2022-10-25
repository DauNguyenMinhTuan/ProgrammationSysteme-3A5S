#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
    struct stat stats;
    char path[50];
    sprintf(path, "./%s", argv[1]);
    if(stat(path, &stats) >= 0){
        int stat_mode = stats.st_mode;
        char type = '?';
        if(S_ISREG(stat_mode)){
            type = '-';
        }
        else if(S_ISLNK(stat_mode)){
            type = 'l';
        }
        int nb_link = (int)stats.st_nlink;
        struct passwd *pw;
        struct group *gr;
        pw = getpwuid(stats.st_uid);
        char pws[20], grs[20];
        if(pw != NULL){
            strcpy(pws, pw->pw_name);
        }
        else{
            sprintf(pws, "%8d", (int)stats.st_uid);
        }
        gr = getgrgid(stats.st_gid);
        if(gr != NULL){
            strcpy(grs, gr->gr_name);
        }
        else{
            sprintf(grs, "%8d", (int)stats.st_gid);
        }
        int size = (int)stats.st_size;
        char time[50];
        strftime(time, 50, "%a %e %h %Y %H:%M:%S", localtime(&(stats.st_mtim)));
        printf("%c%c%c%c%c%c%c%c%c%c %d %s %s %d %s %s\n", type,
                                        stats.st_mode & S_IRUSR ? 'r' : '-',
                                        stats.st_mode & S_IWUSR ? 'w' : '-',
                                        stats.st_mode & S_IXUSR ? 'x' : '-',
                                        stats.st_mode & S_IRGRP ? 'r' : '-',
                                        stats.st_mode & S_IWGRP ? 'w' : '-',
                                        stats.st_mode & S_IXGRP ? 'x' : '-',
                                        stats.st_mode & S_IROTH ? 'r' : '-',
                                        stats.st_mode & S_IWOTH ? 'w' : '-',
                                        stats.st_mode & S_IXOTH ? 'x' : '-',
                                        nb_link, pws, grs, size, time, argv[1]);
    }
    else{
        perror("Cannot open file!");
        return 1;
    }
}