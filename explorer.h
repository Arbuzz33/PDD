#ifndef EXPLORER_H_INCLUDED
#define EXPLORER_H_INCLUDED

typedef struct {
    char name[1024];
    unsigned long size;
} file;

typedef struct {
    char name[1024];
    unsigned long size;
} directory;

int get_files(file** files, const char* path);
void print_files(file* files, unsigned int count, char* opts);
int alphabet_compare(const void *a, const void *b); //add to help.h
int realphabet_compare(const void *a, const void *b); //add to help.h
void sort_files(file** files, unsigned int count, char order);
int get_dirs(directory **dirs, char* path);
void print_dirs(directory* dirs, unsigned int count, char* opts);
void sort_dirs(directory** dirs, unsigned int count, char order);

#endif