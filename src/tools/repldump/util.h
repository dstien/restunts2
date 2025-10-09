#pragma once

#include <dos.h>

typedef unsigned size_t;
typedef unsigned FILE;

void memcpy(void* dst, const void* src, size_t n);
FILE* fopen(const char* path, const char* mode);
int fclose(FILE* file);
size_t fwrite(const void far* src, size_t size, size_t nmemb, FILE* file);
