#ifndef STRING_H
#define STRING_H
#include <sys/types.h>

char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
int strcmp(const char *s, const char *d);
int strncmp(char* s, char* d, size_t n);
char* strcat(char* dest, const char* src);
size_t strlen(char* s);

#endif
