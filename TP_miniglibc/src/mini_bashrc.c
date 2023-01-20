#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "mini_lib.h"

#define USERNAME_LEN 32
#define LTEMPS 32

void print_file_stat(const char *ref, struct stat *status) {
    if (ref[0] == '.') {
        return;
    }
    struct passwd *pw;
    struct group *grp;
    char type;
    char pws[USERNAME_LEN], grs[USERNAME_LEN], temps[LTEMPS];
    char *ref_color = "\033[0m";

    if (status->st_mode & S_IXUSR || status->st_mode & S_IXGRP || status->st_mode & S_IXOTH) {
        ref_color = "\033[1;32m";
    }

    type = '?';
    if (S_ISREG(status->st_mode)) {
        type = '-';
    } else if (S_ISDIR(status->st_mode)) {
        type = 'd';
        ref_color = "\033[1;34m";
    } else if (S_ISCHR(status->st_mode)) {
        type = 'c';
    } else if (S_ISBLK(status->st_mode)) {
        type = 'b';
    } else if (S_ISFIFO(status->st_mode)) {
        type = 'p';
    }

    strftime(temps, LTEMPS, "%a %e %h %Y %H:%M:%S", localtime(&(status->st_mtime)));

    pw = getpwuid(status->st_uid);
    if (pw != NULL) {
        mini_strcpy(pw->pw_name, pws);
    } else {
        sprintf(pws, "%8d", (int)status->st_uid);
    }

    grp = getgrgid(status->st_gid);
    if (grp != NULL) {
        mini_strcpy(grp->gr_name, grs);
    } else {
        sprintf(grs, "%8d", (int)status->st_gid);
    }

    printf("%c%c%c%c%c%c%c%c%c%c %2d %8s %8s %9d %s %s %s\033[0m\n", type,
           status->st_mode & S_IRUSR ? 'r' : '-',
           status->st_mode & S_IWUSR ? 'w' : '-',
           status->st_mode & S_IXUSR ? 'x' : '-',
           status->st_mode & S_IRGRP ? 'r' : '-',
           status->st_mode & S_IWGRP ? 'w' : '-',
           status->st_mode & S_IXGRP ? 'x' : '-',
           status->st_mode & S_IROTH ? 'r' : '-',
           status->st_mode & S_IWOTH ? 'w' : '-',
           status->st_mode & S_IXOTH ? 'x' : '-',
           (int)status->st_nlink, pws, grs, (int)status->st_size, temps, ref_color, ref);
}

void mini_ls() {
    DIR *dirp;
    struct dirent *dp;
    struct stat status;
    if ((dirp = opendir(".")) == NULL) {
        mini_perror("Could not open current folder\n");
        mini_exit();
    }
    while ((dp = readdir(dirp))) {
        if (stat(dp->d_name, &status) == -1) {
            mini_perror("Cannot obtain status of file\n");
            mini_exit();
        }
        print_file_stat(dp->d_name, &status);
    }
    closedir(dirp);
}

void mini_chmod(char *file_path, int mode) {
    if (chmod(file_path, mode) < 0) {
        mini_perror("Cannot change rights\n");
        mini_exit();
    }
}

void mini_ln(char *target, char *link_path) {
    if (symlink(target, link_path) < 0) {
        mini_perror("Error creating link\n");
        mini_exit();
    }
}

void mini_quickdiff(char *src, char *dest) {
    MYFILE *src_file = mini_fopen(src, 'r');
    if (src_file == NULL) {
        mini_perror("Cannot Open File!");
        return;
    }
    struct stat st_src;
    stat(src, &st_src);
    int src_file_size = st_src.st_size;
    char *src_data = (char *)mini_calloc(1, src_file_size + 1);
    if (src_data == NULL) {
        mini_perror("Cannot Initiate Intermediate Buffer!");
        return;
    }
    if (mini_fread(src_data, 1, src_file_size, src_file) < 0) {
        mini_perror("Error Reading Data From Source File!");
        return;
    }
    src_data[src_file_size] = '\0';
    mini_fclose(src_file);

    MYFILE *dest_file = mini_fopen(dest, 'r');
    if (dest_file == NULL) {
        mini_perror("Cannot Open File!");
        return;
    }
    struct stat st_dest;
    stat(dest, &st_dest);
    int dest_file_size = st_dest.st_size;
    char *dest_data = (char *)mini_calloc(1, dest_file_size + 1);
    if (dest_data == NULL) {
        mini_perror("Cannot Initiate Intermediate Buffer!");
        return;
    }
    if (mini_fread(dest_data, 1, dest_file_size, dest_file) < 0) {
        mini_perror("Error Reading Data From Source File!");
        return;
    }
    dest_data[dest_file_size] = '\0';
    mini_fclose(dest_file);

    char *src_diff = (char *)mini_calloc(1, (src_file_size + 1) * 2);
    char *dest_diff = (char *)mini_calloc(1, (dest_file_size + 1) * 2);
    int index_src, index_dest;
    index_src = index_dest = 0;
    int src_diff_charcnt, dest_diff_charcnt;
    src_diff_charcnt = dest_diff_charcnt = 0;
    while (index_src < src_file_size && index_dest < dest_file_size) {
        if (src_data[index_src] != dest_data[index_dest]) {
            // printf("%d %d\n", index_src, index_dest);
            // save line to temporary buffer
            int startpos, endpos;
            startpos = endpos = index_src;
            // find the line in src
            // first find start of line
            while (startpos > 0 && src_data[startpos - 1] != '\n') {
                startpos--;
            }
            // find end of line
            while (endpos < src_file_size && src_data[endpos] != '\n') {
                endpos++;
            }
            // copy line to buffer (diff)
            src_diff[src_diff_charcnt++] = '>';
            for (int ind = startpos; ind < endpos; ind++) {
                src_diff[src_diff_charcnt++] = src_data[ind];
            }
            src_diff[src_diff_charcnt++] = '\n';
            // and set index to new position
            index_src = endpos;

            startpos = endpos = index_dest;
            // find the line in dest
            // first find start of line
            while (startpos > 0 && dest_data[startpos - 1] != '\n') {
                startpos--;
            }
            // find end of line
            while (endpos < dest_file_size && dest_data[endpos] != '\n') {
                endpos++;
            }
            // copy line to buffer (diff)
            dest_diff[dest_diff_charcnt++] = '>';
            for (int ind = startpos; ind < endpos; ind++) {
                dest_diff[dest_diff_charcnt++] = dest_data[ind];
            }
            dest_diff[dest_diff_charcnt++] = '\n';
            // and set index to new position
            index_dest = endpos;

            // printf("%s%s\n", src_diff, dest_diff);
        }
        index_src++;
        index_dest++;
    }
    // printf("src left: %d\n%s\ndest left: %d\n%s\n", index_src, src_data+index_src, index_dest, dest_data+index_dest);
    if (index_src >= src_file_size && index_dest >= dest_file_size) {
    } else if (index_src < src_file_size) {
        int startpos, endpos;
        if (src_data[index_src] == '\n') {
            startpos = (++index_src);
        } else {
            startpos = index_src;
            while (startpos > 0 && src_data[startpos - 1] != '\n') {
                startpos--;
            }
        }
        endpos = src_file_size;
        for (int ind = startpos; ind < endpos; ind++) {
            if (ind > 0 && src_data[ind - 1] == '\n') {
                src_diff[src_diff_charcnt++] = '>';
            }
            src_diff[src_diff_charcnt++] = src_data[ind];
        }
        src_diff[src_diff_charcnt++] = '\n';
        src_diff[src_diff_charcnt++] = '\0';
    } else if (index_dest < dest_file_size) {
        int startpos, endpos;
        if (dest_data[index_dest] == '\n') {
            startpos = (++index_dest);
        } else {
            startpos = index_dest;
            while (startpos > 0 && dest_data[startpos - 1] != '\n') {
                startpos--;
            }
        }
        endpos = dest_file_size;
        for (int ind = startpos; ind < endpos; ind++) {
            if (ind > 0 && dest_data[ind - 1] == '\n') {
                dest_diff[dest_diff_charcnt++] = '>';
            }
            dest_diff[dest_diff_charcnt++] = dest_data[ind];
        }
        dest_diff[dest_diff_charcnt++] = '\n';
        dest_diff[dest_diff_charcnt++] = '\0';
    }
    mini_printf(src);
    mini_printf(":\n");
    mini_printf(src_diff);
    mini_printf(dest);
    mini_printf(":\n");
    mini_printf(dest_diff);
}

void mini_mkdir(char *folder_path, int mode) {
    if (mkdir(folder_path, mode) < 0) {
        mini_perror("Cannot create directory\n");
        mini_exit();
    }
}

void mini_rm(char *file_path) {
    struct stat status;
    if (stat(file_path, &status) < 0) {
        mini_perror("Cannot identify if this is regular folder\n");
        mini_exit();
    }
    if (S_ISREG(status.st_mode)) {
        // printf("remove %s\n", file_path);
		if (remove(file_path) < 0) {
            mini_perror("Cannot remove file\n");
            mini_exit();
        }
    }
}

void mini_rmdir(char *folder_path) {
	struct stat status;
    if (stat(folder_path, &status) < 0) {
        mini_perror("Cannot identify if this is regular folder\n");
        mini_exit();
    }
    if (S_ISDIR(status.st_mode)) {
        if (remove(folder_path) < 0) {
            mini_perror("Cannot remove file\n");
            mini_exit();
        }
    }
}

void mini_mv(char *src, char *dest){
	struct stat src_status;
    if (stat(src, &src_status) < 0) {
        mini_perror("Cannot identify if this is regular folder\n");
        mini_exit();
    }
	struct stat dest_status;
    if (stat(dest, &dest_status) < 0) {
        mini_perror("Cannot identify if this is regular folder\n");
        mini_exit();
    }
    if (S_ISDIR(src_status.st_mode)) {
        mini_perror("Move folder to folder not yet available\n");
		mini_exit();
    }
	else if(S_ISREG(dest_status.st_mode)){
		if(rename(src, dest) < 0){
			mini_perror("Cannot rename file\n");
			mini_exit();
		}
	}
	else if(S_ISDIR(dest_status.st_mode)){
		
	}
}