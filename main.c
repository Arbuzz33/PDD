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
void disk_info_print(){
    count = get_block_devs(&system_disks);
    get_partitions(&system_disks, count);
    get_program_path(&path);
    for(int i = 0; i < count; i++)
        for(int j = 0; j < system_disks[i].parts_count-1; j++)
            mount_part(path, system_disks[i].parts[j].nam);

    for(int i = 0; i < count; i++)
        get_mounted_info(path, &(system_disks[i]));
    print_info(system_disks, count);
}
int main(int argc, char* argv[]) {
    parse_args(argc,argv);
    //call_fdisk("/dev/sda");
    //call_mkfs("/dev/sda1", "ext4");
    
    //if((file_count = get_files(&files, "/home/nigger/Desktop")) > 0) {
    //    sort_files(files, file_count, 'g');
    //    print_files(files, file_count, "s");
    //}

    //if((dir_count = get_dirs(&dirs, "/home/nigger/Desktop")) > 0) {
    //    sort_files(&dirs, dir_count, 'l');
    //    print_files(dirs, dir_count, "t3s2");
    //}

    while(strncmp(input, "quit", 4) != 0 && strncmp(input, "exit", 4) != 0) {
        printf("pdd> ");
        scanf("%s", input);
        printf("%s\n", input);
        parse_cli(input);
    }
    return 0;
}