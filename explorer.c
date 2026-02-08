#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include "help.h"
#include "explorer.h"


int get_files(file** files, const char* path) { //get files in files array
    DIR *d;
    struct dirent *dir;
    struct stat file_stat;
    *files = NULL;
    unsigned int file_count = 0;
    char *full_path;

    if((d = opendir(path)) != NULL) {
        while((dir = readdir(d)) != NULL) {
            if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            }
            full_path = malloc(1024);
            if(!full_path) {
                perror("Can not allocate memory!");
                return -1;
            }
            snprintf(full_path, 1024, "%s/%s", path, dir->d_name);
            if(stat(full_path, &file_stat) == S_ISDIR(file_stat.st_mode)) {
                *files = realloc(*files, sizeof(file) * (file_count+1));
                if(!*files) {
                    perror("Can not reallocate memory!");
                    return -2;
                }
                strcpy((*files)[file_count].name, dir->d_name);
                (*files)[file_count].size = file_stat.st_size;
                file_count++;
            }
            free(full_path);
        }
        closedir(d);
    } else {
        printf("Could not open dir!\n");
    }
    return file_count;
}

long get_dir_size(const char* path) {
    struct stat statbuf;
    DIR *dir;
    struct dirent *entry;
    long dir_size = 0;
    char full_path[1024];

    if(lstat(path, &statbuf) == -1) {
        return -1;
    }
    dir_size += statbuf.st_size;
    if(S_ISLNK(statbuf.st_mode)) {
        return dir_size;
    }
    if(!S_ISDIR(statbuf.st_mode)) {
        return dir_size;
    }

    if((dir = opendir(path)) != NULL) {
        while((entry = readdir(dir)) != NULL) {
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            snprintf(full_path, 1024, "%s/%s", path, entry->d_name);
            long sub_size = get_dir_size(full_path);
            if(sub_size != -1) {
                dir_size += sub_size;
            }
        }
    }
    closedir(dir);
    return dir_size;
}

int get_dirs(directory **dirs, const char* path) { //get directories in directories array
    DIR *d;
    struct dirent *dir;
    struct stat dir_stat;
    *dirs = NULL;
    unsigned int dir_count = 0;
    char *full_path;

    if((d = opendir(path)) != NULL) {
        while((dir = readdir(d)) != NULL) {
            if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            }
            full_path = malloc(1024);
            if(!full_path) {
                perror("Can not allocate memory!");
                return -1;
            }
            snprintf(full_path, 1024, "%s/%s", path, dir->d_name);
            if(stat(full_path, &dir_stat) == !S_ISDIR(dir_stat.st_mode)) {
                *dirs = realloc(*dirs, sizeof(directory) * (dir_count+1));
                if(!*dirs) {
                    perror("Can not reallocate memory!");
                    return -2;
                }
                strcpy((*dirs)[dir_count].name, dir->d_name);
                (*dirs)[dir_count].size = get_dir_size(full_path);
                dir_count++;
            }
            free(full_path);
        }
        closedir(d);
    } else {
        printf("Could not open dir!\n");
    }
    return dir_count;
}

int alphabet_compare(const void *a, const void *b) { //add to help.c Compare in alphabet order
    return strcmp(((file *)a)->name, ((file *)b)->name);
}

int realphabet_compare(const void *a, const void *b) { //add to help.c Compare in reverse alphabet order
    return -(strcmp(((file *)a)->name, ((file *)b)->name));
}

int less_compare(const void *a, const void *b) { //add to help.c Compare from less to greater
    long res = ((file *)a)->size - ((file *)b)->size;
    if(res > 0) return 1;
    if(res < 0) return -1;
    return 0;
}

int greater_compare(const void *a, const void *b) { //add to help.c Compare from greater to less
    long res = ((file *)a)->size - ((file *)b)->size;
    if(res > 0) return -1;
    if(res < 0) return 1;
    return 0;
}

void print_files(file** files, unsigned int count, const char* opts) { //printing according to options
    const char* units[5] = {"B", "KB", "MB", "GB", "TB"};
    char* head;
    char* tail;
    char* size_str;
    int size = -1;
    unsigned int head_count, tail_count;
    unsigned int i = 0;

    if ((head = strchr(opts, 'h')) != NULL) {
        if (sscanf(head, "h%d", &head_count) == 1) 
            count = head_count;
    } else if ((tail = strchr(opts, 't')) != NULL) {
        if(sscanf(tail, "t%d", &tail_count) == 1)
            i = count - tail_count;
    }
    if((size_str = strchr(opts, 's')) != NULL) {
        size = 0;
        sscanf(size_str, "s%d", &size);
    }
    
    if(strchr(opts, 'a') != NULL) {
        qsort(*files, count, sizeof(file), alphabet_compare);
    } else if(strchr(opts, 'r') != NULL) {
        qsort(*files, count, sizeof(file), realphabet_compare);
    } else if(strchr(opts, 'l') != NULL) {
        qsort(*files, count, sizeof(file), less_compare);
    } else if(strchr(opts, 'g') != NULL) {
        qsort(*files, count, sizeof(file), greater_compare);
    }

    for(; i < count; i++) {
        printf("%s", (*files)[i].name);
        if(size != -1) {
            double temp_size = (*files)[i].size;
            for(byte j = 0; j < size; j++) {
                temp_size /= 1024.0f;
            }
            printf(" - %6.2f %s\n", temp_size, units[size]);
        }
        else printf("\n");
    }
}