#include "disk_info.h"
#include "help.h"
#include "explorer.h"
#include "format.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
Colors TextColors = {"\033[37m","\033[30m","\033[34m","\033[31m","\033[33m"};
Colors BackGroundColors = {"\033[47m","\033[40m","\033[44m","\033[41m","\033[43m"};
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
void parse_args(int argc, char* argv[]){

    for(int i = 1; i < argc; i++){
        printf("%s\n", argv[i]);
        if (strcmp(argv[i], "--version") == 0) {
            printf("%s\n",VERSION);
            exit(0);
        }
        if (strcmp(argv[i], "--theme1") == 0){
            printf(TextColors.colorBlack); 
            printf(BackGroundColors.colorWhite);
        }
        if (strcmp(argv[i], "--theme2") == 0){
            printf(TextColors.colorWhite); 
            printf(BackGroundColors.colorBlue);
        }
        if (strcmp(argv[i], "--theme3") == 0){
            printf(TextColors.colorYellow); 
            printf(BackGroundColors.colorRed);
        }
        if (strcmp(argv[i], "--bold") == 0){
            printf(BOLD);
        }
     }
    printf(LOGO);
    printf(STARTMESSAGE);
}
void parse_cli(char* input) {
    if(strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
        printf("Bye!");
    }
    else if(strcmp(input, "help") == 0) {
        printf(COMMANDS);
    }
    else if (strcmp(input, "diskinfo") == 0)
    {
        disk_info_print();
    }
    else if(strcmp(input, "textinvert") == 0) {
        printf(INVERT);
    }
    else if(strcmp(input, "textdefault") == 0) {
        printf(DEFAULT);
    }
    else {
        printf(" Bad usage. Type \"help\" to check yourself \n");
    }
}
