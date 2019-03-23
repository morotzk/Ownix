#ifndef FILE_OPEN_H
#define FILE_OPEN_H
#include <fs/inode.h>
#include <fs/file_descriptor.h>

/* File not exists		*/
#define E_NO_FILE			-1

/* Count of open-files reached the maximum allowed in the system	*/
#define E_MAX				-2

/* Return with a file descriptor index in the fd table	*/
int file_open(char* path);

#endif
