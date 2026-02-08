#ifndef EXPLORER_H_INCLUDED
#define EXPLORER_H_INCLUDED


typedef struct {
    char name[1024];
    unsigned long size;
} file;

typedef file directory;


int get_files(file** files, const char* path);
long get_dir_size(const char* path);
int get_dirs(directory **dirs, const char* path);

int alphabet_compare(const void *a, const void *b); //add to help.h
int realphabet_compare(const void *a, const void *b); //add to help.h
int less_compare(const void *a, const void *b); //add to help.h
int greater_compare(const void *a, const void *b); //add to help.h

void print_files(file** files, unsigned int count, const char* opts);


#endif