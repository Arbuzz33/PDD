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
    printf(" Available commands:\n");
    printf("  help - prints explanation of all commands\n");
    printf("  quit/exit - quits the program\n");
    printf("  textinvert - inverts text\n");
    printf("  textdefault - text format and colors become default\n");
    printf("  diskinfo - prints info about all disks with their partitions (also lets you mount them to get more info)\n");
    printf("  unmountsdisks - use it after diskinfo if disks were mounted\n");
    printf("  printfiles - prints files in a given directory. Ex: \"printfiles -sgh5 /bin\" - prints first 5 files (h5) with sizes (s) from bigger to lesser (g)\n");
    printf("  printdirs - prints directories in a given directory. Ex: \"printdirs -slt5 /home/user/Documents\" - prints last 5 files (t5) with sizes (s) from lesser to bigger (l)\n");
    printf("  speedtest - does speed test of a given partition (need to be mounted). Ex: \"speedtest -sda1\" - makes speedtest of a first partition of disk sda\n");
    printf("  temperature - prints temperature of a given disk. Ex: \"temperature -nvme0n1\" - prints the temperature of disk nvme0n1\n");
    printf("  badblks - checks bad blocks on a disk. Ex: \"badblks -sdb\" - makes check of badblocks on disk sdb\n");
    printf("  checkio - checks input/output errors on a disk. Ex: \"checkio -nvme1n1\" - prints errors and recomendations on disk nvme1n1\n");
    printf("  format - formats the disks (at this moment only partition table). Ex: \"format -sdc\" - lets you create new partition table on sdc (uses fdisk)\n");
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