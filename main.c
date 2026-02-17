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
int new_len;
file *files;
directory *dirs;
unsigned int file_count = 0;
unsigned int dir_count = 0;

int main(int argc, char* argv[]) {
    parse_args(argc,argv);
    get_program_path(&path);
    //call_fdisk("/dev/sda");
    //call_mkfs("/dev/sda1", "ext4");
    
    /*if((file_count = get_files(&files, "/home/danila/Desktop/pdd")) > 0) {
        print_files(&files, file_count, "s");
    }*/

    /*if((dir_count = get_dirs(&dirs, "/home/danila")) > 0) {
        print_files(&dirs, dir_count, "t3s2");
    }*/
    /*get_disk_stats("sdb3");
    printf("nvme0n1 temperature: %f\n", get_disk_temp("sdb3"));
    call_badblocks("sdb3");*/

    while(strncmp(input, "quit", 4) != 0 && strncmp(input, "exit", 4) != 0) {
        printf("pdd> ");
        //scanf("%s", input);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
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
        else if (strstr(input, "printfiles") != 0) {
            char* sort_file_args;
            char* file_args_raw = strchr(input, '-');
            char* file_path = strchr(input, '/');
            new_len = strlen(file_args_raw) - strlen(strchr(file_args_raw, ' '));
            sort_file_args = malloc(new_len + 1);
            strncpy(sort_file_args, file_args_raw, new_len+1);
            memmove(sort_file_args, sort_file_args + 1, strlen(sort_file_args));
            if((file_count = get_files(&files, file_path)) > 0) {
                print_files(&files, file_count, sort_file_args);
            }
            free(sort_file_args);
        }
        else if (strstr(input, "printdirs") != 0) {
            char* sort_dir_args;
            char* dir_args_raw = strchr(input, '-');
            char* dir_path = strchr(input, '/');
            new_len = strlen(dir_args_raw) - strlen(strchr(dir_args_raw, ' '));
            sort_dir_args = malloc(new_len + 1);
            strncpy(sort_dir_args, dir_args_raw, new_len+1);
            memmove(sort_dir_args, sort_dir_args + 1, strlen(sort_dir_args));
            if((dir_count = get_dirs(&dirs, dir_path)) > 0) {
                print_files(&dirs, dir_count, sort_dir_args);
            }
            free(sort_dir_args);
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