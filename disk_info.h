#ifndef DISK_INFO_INCLUDED
#define DISK_INFO_INCLUDED

#include <stdbool.h>
#include "help.h"
#define disks_dir "/sys/block"

typedef struct {
    char nam[128];
    char mount_point[256];
    char fs[16];
    unsigned long long size_total;
    unsigned long long size_free;
    unsigned long long size_used;
} partition;

typedef struct {
    byte parts_count;
    char name[128];
    unsigned long long size_total;
    unsigned long long size_free;
    unsigned long long size_used;
    partition parts[128];
    bool type;
    char model[128];
} disk_info;

void get_swap();                                        //gets all swaps
unsigned long long get_disk_size(const char* dev_name); //gets size of this disk
int get_block_devs(disk_info **dsks);                   //gets disks in disks array
void print_size(unsigned long long s, byte u);          //prints size with metrics
void get_partitions(disk_info **disks, int c);          //gets partitions of disks
void mount_part(char* path, char* name);                //mounts disk partition to path
void umount_part(char* path, char* name);               //unmounts disk partition from path
int get_mounted_info(char* path, disk_info *disk);      //gets more info from mounted partitions
void print_disk_info(disk_info *disks, int count);      //prints disks' info
void get_disk_stats(const char* disk_name);             //prints read/write stats
void call_badblocks(const char* disk_name);             //calls badblocks on disk
float get_disk_temp(const char* disk_name);             //returns temp of disk

#endif