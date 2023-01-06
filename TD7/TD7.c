#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define STR_PERE "bar"
#define STR_FILS "foo"

extern int errno;
extern char **environ;

int main() {
    pid_t pid;
    char cmd[1024];
    while (1) {
        scanf("%s", cmd);
        if (!strcmp(cmd, "exit")) {
            break;
        }
        pid = fork();
        switch (pid) {
            case -1:
                perror("Fork Error");
                exit(EXIT_FAILURE);
            case 0:
                char *binaryPath = "/bin/bash";
                char *const args[] = {binaryPath, "-c", cmd, NULL};
                if (execve(binaryPath, args, environ) < 0) {
                    exit(EXIT_FAILURE);
                }
			default:
				wait(&pid);
				if(WIFEXITED(pid) && WEXITSTATUS(pid)){
					// perror(strerror(errno));
				}
        }
    }
    exit(EXIT_SUCCESS);
}