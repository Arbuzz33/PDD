#include "help.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void get_program_path(char** p) {
    size_t size = 1024;
    *p = malloc(size);
    if(!(*p)) {
        perror("Memory allocation error");
        return;
    }
    if(!getcwd(*p, size)) {
        perror("Failed to get path");
        return;
    }
}


void parse_cli(char* input) {
    if(strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
        printf("Bye!\n");
    }
     else {
        printf("Bad usage. Type \"help\" to check yourself :3\n");
    }
}