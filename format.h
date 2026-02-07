#ifndef FORMAT_H_INCLUDED
#define FORMAT_H_INCLUDED

void call_fdisk(const char* device);
void call_mkfs(const char* partition, const char* fs_name);

#endif