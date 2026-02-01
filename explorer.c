#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
//#include "help.h"
#include "explorer.h"
//TODO: printing with metrics(B, KB, MB, etc.)

int get_files(file** files, char* path) { //get files in files array
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

int get_dirs(directory **dirs, char* path) { //get directories in directories array
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
            if(stat(full_path, &dir_stat) == !(S_ISDIR(dir_stat.st_mode))) {
                DIR *cur_d;
                struct dirent *cur_dir;
                struct stat st;
                *dirs = realloc(*dirs, sizeof(directory) * (dir_count+1));
                if(!*dirs) {
                    perror("Can not reallocate memory!");
                    return -2;
                }
                strcpy((*dirs)[dir_count].name, dir->d_name);
                (*dirs)[dir_count].size = dir_stat.st_size;
                if((cur_d = opendir(full_path)) != NULL) {
                    while((cur_dir = readdir(cur_d)) != NULL) {
                        if(strcmp(cur_dir->d_name, ".") == 0 || strcmp(cur_dir->d_name, "..") == 0) {
                            continue;
                        }
                        char cur_full_path[1024];
                        snprintf(cur_full_path, 1024, "%s/%s", full_path, cur_dir->d_name);
                        if(stat(cur_full_path, &st) == S_ISDIR(st.st_mode)) {
                            (*dirs)[dir_count].size += st.st_size;
                        }
                    }
                    closedir(cur_d);
                }
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

void sort_files(file** files, unsigned int count, char order) { //sorting files according to order
    if(order == 'a') {
        qsort(*files, count, sizeof(file), alphabet_compare);
    } else if(order == 'r') {
        qsort(*files, count, sizeof(file), realphabet_compare);
    } else if(order == 'l') {
        qsort(*files, count, sizeof(file), less_compare);
    } else if(order == 'g') {
        qsort(*files, count, sizeof(file), greater_compare);
    } else {
        printf("Wrong sort mode");
    }
}

void print_files(file* files, unsigned int count, char* opts) { //printing according to options
    char* head;
    char* tail;
    unsigned int head_count, tail_count;
    unsigned int i = 0;

    if ((head = strchr(opts, 'h')) != NULL) {
        if (sscanf(head, "h%d", &head_count) == 1) 
            count = head_count;
    } else if ((tail = strchr(opts, 't')) != NULL) {
        if(sscanf(tail, "t%d", &tail_count) == 1)
            i = count - tail_count;
    }

    for(; i < count; i++) {
        printf("%s", files[i].name);
        if(strchr(opts, 's') != NULL) printf(" - %lu B\n", files[i].size);
        else printf("\n");
    }
}