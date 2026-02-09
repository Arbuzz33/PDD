#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "help.h"
#include "explorer.h"

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

void print_help() {
    printf("  Available commands:\n");
    printf("  1.quit/exit - exit the application\n");
    printf("  2.diskinfo - get information about disks with option to mount them\n");
    printf("  3.unmountdisks - unmount all mounted disks\n");
    printf("  4.printfiles(path_to_directory,s[0-4]-size,t-tail[0-count],h-head[0-count]) - print all files in a directory (with options)\n");
    printf("  5.printdirs(path_to_directory,s[0-4]-size,t-tail[0-count],h-head[0-count]) - print all directories in a directory (with options)\n");
    printf("  printfiles and printdirs have sorting:a - alphabet,r - reversed alphabet; l - smaller files first, g - nigger files first\n");
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
        if (strcmp(argv[i], "--epsteintheme")==0){
            printf(TextColors.colorBlack);
            printf(BackGroundColors.colorBlack);
            printf(BOLD);
        }
        if (strcmp(argv[i], "--bold") == 0){
            printf(BOLD);
        }
     }
    printf(LOGO);
    printf(STARTMESSAGE);
}

int alphabet_compare(const void *a, const void *b) {
    return strcmp(((file *)a)->name, ((file *)b)->name);
}

int realphabet_compare(const void *a, const void *b) {
    return -(strcmp(((file *)a)->name, ((file *)b)->name));
}

int less_compare(const void *a, const void *b) {
    long res = ((file *)a)->size - ((file *)b)->size;
    if(res > 0) return 1;
    if(res < 0) return -1;
    return 0;
}

int greater_compare(const void *a, const void *b) {
    long res = ((file *)a)->size - ((file *)b)->size;
    if(res > 0) return -1;
    if(res < 0) return 1;
    return 0;
}