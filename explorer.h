#ifndef EXPLORER_H_INCLUDED
#define EXPLORER_H_INCLUDED


typedef struct {
    char name[1024];
    unsigned long size;
} file;

typedef file directory;


int get_files(file** files, const char* path);    //gets files in files array
long get_dir_size(const char* path);              //this needs only for get_dirs
int get_dirs(directory **dirs, const char* path); //gets dirs in dirs array

void print_files(file** files, unsigned int count, const char* opts); //prints files with options


#endif