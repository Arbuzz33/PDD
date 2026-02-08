#include "disk_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <mntent.h>
#include <sys/sysinfo.h>
#include "help.h"


int get_block_devs(disk_info **dsks) {
    DIR *dir;
    struct dirent *entry;
    char path[512];
    int count = 0;
    *dsks = NULL;

    dir = opendir(disks_dir);
    if(!dir) {
        perror("Can not get disks");
        return -1;
    }
    while((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || 
            strcmp(entry->d_name, "..") == 0 ||
            strncmp(entry->d_name, "loop", 4) == 0 ||
            strncmp(entry->d_name, "ram", 3) == 0) {
            continue;
        }
        if (strchr(entry->d_name, '/') != NULL) continue;

        *dsks = realloc(*dsks, sizeof(disk_info) * (count+1));
        if(!*dsks) {
            perror("Error of memory allocation!");
            return -2;
        }

        disk_info *disk = &(*dsks)[count];
        memset(disk, 0, sizeof(disk_info));
        snprintf(disk->name, sizeof(disk->name), "/dev/%s", entry->d_name);
        disk->size_total = get_disk_size(entry->d_name);
        count++;
    }
    closedir(dir);
    return count;
}

unsigned long long get_disk_size(const char* dev_name) {
    FILE *fp;
    char path[512];
    unsigned long long size = 0;
    
    // Попробовать прочитать размер из sysfs
    snprintf(path, sizeof(path), "/sys/block/%s/size", dev_name);
    fp = fopen(path, "r");
    if (fp) {
        if (fscanf(fp, "%llu", &size) == 1) {
            fclose(fp);
            return size * 512; // Размер в секторах по 512 байт
        }
        fclose(fp);
    }
    
    return 0;
}

void print_size(unsigned long long s, byte u) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"}; 
    double size = s;
    for(byte i = 0; i < u; i++) {
        size /= 1024.0;
    }
    printf("%-6.2f %-6s", size, units[u]);
}

void print_disk_info(disk_info *disks, int count) {
    printf("%-20s %-12s %-12s %-12s %-6s %s\n", "Name", "Size", "Used", "Free", "FS", "Mountpoint");
    printf("--------------------------------------------------------------------------------------------\n");
    for(int i = 0; i < count; i++) {
        printf("%-21s", disks[i].name);
        print_size(disks[i].size_total, GB_SIZE);
        printf("\n");
        for(int j = 0; j < disks[i].parts_count-1; j++) {
            partition pt = disks[i].parts[j];
            printf("  %-19s", pt.nam);
            print_size(pt.size_total, GB_SIZE);
            print_size(pt.size_used, GB_SIZE);
            print_size(pt.size_free, GB_SIZE);
            printf("%-6s %s\n", pt.fs, pt.mount_point);
        }
    }
}

void get_partitions(disk_info **disks, int c) {
    for(int i = 0; i < c; i++) {
        int next = 0;
        int count = 1;

        while(next == 0) {
            char buf1[128];
            if(strstr((*disks)[i].name, "nvme")) {
                snprintf(buf1, sizeof(buf1), "%sp%d", (*disks)[i].name, count);
            } else {
                snprintf(buf1, sizeof(buf1), "%s%d", (*disks)[i].name, count);
            }
            
            if(access(buf1, 0) == 0) {
                strncpy((*disks)[i].parts[count-1].nam, buf1, 128);
                count++;
            } else {
                (*disks)[i].parts_count = count;
                count = 1;
                next = 1;
            }
        }
    }
}

void mount_part(char* path, char* name) {
    char command[256];
    snprintf(command, sizeof(command), "cd %s && mkdir mnt/%s && sudo mount %s mnt/%s", path, name+5, name, name+5);
    printf("%s\n", command);
    system(command);
}

void umount_part(char* path, char* name) {
    char command[256];
    snprintf(command, sizeof(command), "cd %s && sudo umount mnt/%s", path, name+5);
    printf("%s\n", command);
    system(command);
}

int get_mounted_info(char* path, disk_info *disk) {
    FILE *f;
    struct mntent *mnt;

    f = setmntent("/proc/mounts", "r");
    if(!f) {
        perror("Can not open /proc/mounts");
        return -1;
    }

    while((mnt = getmntent(f)) != NULL) {
        if (strncmp(mnt->mnt_fsname, "/dev/", 5) != 0) {
            continue;
        }

        if (strstr(mnt->mnt_fsname, "/dev/sr") != NULL ||
            strstr(mnt->mnt_fsname, "/dev/fd") != NULL) {
            continue;
        }

        struct statvfs info;
        if(statvfs(mnt->mnt_dir, &info) == 0) {
            int count = 0;
            while(strcmp(mnt->mnt_fsname, disk->parts[count].nam) != 0) {
                count++;
                if(count > disk->parts_count) break;
            }
            if(count > disk->parts_count) continue;

            strcpy(disk->parts[count].mount_point, mnt->mnt_dir);
            strcpy(disk->parts[count].fs, mnt->mnt_type);
            disk->parts[count].size_total = info.f_frsize * info.f_blocks;
            disk->parts[count].size_free = info.f_frsize * info.f_bfree;
            disk->parts[count].size_used = info.f_frsize * info.f_blocks - info.f_frsize * info.f_bfree;
        }
    }
    endmntent(f);
    return 0;
}



void get_swap() {
    struct statvfs info;
    if(statvfs("/dev/nvme1n1p2", &info) == 0) {
        printf("%.2f\n", (info.f_blocks * info.f_frsize) / (1024.0 * 1024 * 1024));
    }
}