#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "disk_info.h"
#include "help.h"
#include "explorer.h"
#include "format.h"

disk_info *system_disks = NULL;
int count = 0;
char command[256];
char *path;
char input[80] = "pddc";
file *files;
directory *dirs;
unsigned int file_count = 0;
unsigned int dir_count = 0;

int main(int argc, char* argv[]) {
    parse_args(argc,argv);
    get_program_path(&path);
    //call_fdisk("/dev/sda");
    //call_mkfs("/dev/sda1", "ext4");
    
    //if((file_count = get_files(&files, "/home/nigger/Desktop")) > 0) {
    //    print_files(files, file_count, "s");
    //}

    //if((dir_count = get_dirs(&dirs, "/home/nigger/Desktop")) > 0) {
    //    print_files(dirs, dir_count, "t3s2");
    //}

    //get_disk_stats("sda");
    //printf("nvme0n1 temperature: %f\n", get_disk_temp("nvme0n1"));
    //call_badblocks("nvme0n1");

    while(strncmp(input, "quit", 4) != 0 && strncmp(input, "exit", 4) != 0) {
        printf("pdd> ");
        scanf("%s", input);
        if(strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            printf("Bye!\n");
        }
        else if(strcmp(input, "help") == 0) {
            print_help();
        }
        else if (strcmp(input, "diskinfo") == 0)
        {
            char option;
            count = get_block_devs(&system_disks);
            get_partitions(&system_disks, count);
            printf("Do you want to mount the disks? [Y/N] ");
            scanf("%s", &option);
            if (option == 'Y' || option == 'y'){
                for(int i = 0; i < count; i++)
                    for(int j = 0; j < system_disks[i].parts_count-1; j++)
                        mount_part(path, system_disks[i].parts[j].nam);
                for(int i = 0; i < count; i++)
                    get_mounted_info(path, &(system_disks[i]));
            }
            print_disk_info(system_disks, count);
        }
        else if (strcmp(input, "unmountdisks") == 0)
        {   
            char option;
            printf("Are you sure? [Y/N] ");
            scanf("%s", &option);
            if (option == 'Y' || option == 'y'){
                for(int i = 0; i < count; i++)
                    for(int j = 0; j < system_disks[i].parts_count-1; j++)
                        umount_part(path, system_disks[i].parts[j].nam);
            }
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
    return 0;
    /*commands: printfiles(s[0-4]-size,t-tail[0-count],h-head[0-count]),
    printdirs(s[0-4]-size,t-tail[0-count],h-head[0-count]),
    */
}