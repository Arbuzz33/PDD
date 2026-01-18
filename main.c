#include "disk_info.h"
#include "help.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

disk_info *system_disks = NULL;
int count = 0;
char command[256];
char *path;
char input[80] = "pddc";

int main(int argc, char* argv[]) {
    count = get_block_devs(&system_disks);
    get_partitions(&system_disks, count);
    get_program_path(&path);
    if(argc < 2) {
        printf("%s\n", VERSION);
    } else {
        //for(int i = 1; i < argc; i++)
        //    printf("%s\n", argv[i]);
        while(strncmp(input, "quit", 4) != 0 && strncmp(input, "exit", 4) != 0) {
            printf("pdd> ");
            scanf("%s", input);
            printf("%s\n", input);
            parse_cli(input);
        }

        //for(int i = 0; i < count; i++)
        //    for(int j = 0; j < system_disks[i].parts_count-1; j++)
        //        mount_part(path, system_disks[i].parts[j].nam);

        //for(int i = 0; i < count; i++)
        //  get_mounted_info(path, &(system_disks[i]));
        //print_info(system_disks, count);
    }
    return 0;
}