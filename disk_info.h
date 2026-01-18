#ifndef DISK_INFO_INCLUDED
#define DISK_INFO_INCLUDED

#define KB_SIZE 1
#define MB_SIZE 2
#define GB_SIZE 3
#define TB_SIZE 4
#define disks_dir "/sys/block"
typedef unsigned char byte;

typedef struct {
    char nam[128];
    char mount_point[256];
    char fs[64];
    unsigned long long size_total;
    unsigned long long size_free;
    unsigned long long size_used;
} partition;

typedef struct { //add partitions
    byte parts_count;
    char name[128];
    unsigned long long size_total;
    unsigned long long size_free;
    unsigned long long size_used;
    partition parts[128];
} disk_info;

void get_swap();
unsigned long long get_disk_size(const char* dev_name);
int get_block_devs(disk_info **dsks);
void print_size(unsigned long long s, byte u);
void get_partitions(disk_info **disks, int c);
void mount_part(char* path, char* name);
void umount_part(char* path, char* name);
int get_mounted_info(char* path, disk_info *disk);
void print_info(disk_info *disks, int count);

#endif