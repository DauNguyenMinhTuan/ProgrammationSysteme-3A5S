#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "new_lib.h"

static char *buffer;
static int ind = -1;

void *new_malloc(int b_size){
	if(b_size < 0){
		puts("Taille Invalide");
		return NULL;
	}
	void *res;
	if((res = sbrk(b_size)) == (void *)(-1)){
		perror("Error: ");
		return NULL;
	}
	return res;
}

void new_printf(char *msg){
	if(ind == -1){
		buffer = new_malloc(BUF_SIZE);
		ind = 0;
	}
	for(int i = 0; i < strlen(msg); i++){
		if(ind == BUF_SIZE || msg[i] == '\n' || msg[i] == '\0'){
			if(write(1, buffer, ind) < 0){
				perror("Error: ");
			}
			ind = 0;
			if(msg[i] == '\n'){
				if(write(1, "\n", 1) < 0){
					perror("Error: ");
				}
			}
		}
		if(msg[i] != '\n'){
			buffer[ind++] = msg[i];
		}
	}
}

void new_exit(){
	_exit(0);
}
