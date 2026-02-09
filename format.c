#include "format.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void call_fdisk(const char* device) { // /dev/sda
    char com[512];
    snprintf(com, 512, "sudo fdisk %s", device);
    system(com);
}

void call_mkfs(const char* partition, const char* fs_name) { // /dev/sda1, fat
    char com[512];
    if(strcmp(fs_name, "fat") == 0)
        snprintf(com, 512, "sudo mkfs.%s -F 32 %s", fs_name, partition);
    else if(strcmp(fs_name, "ext4") == 0 || strcmp(fs_name, "ext3") == 0)
        snprintf(com, 512, "sudo mkfs.%s -F %s", fs_name, partition);
    else
        snprintf(com, 512, "sudo mkfs.%s -f %s", fs_name, partition);
    if(system(com) != 0) printf("Seems like you don't have tools for this fs installed\n");
}