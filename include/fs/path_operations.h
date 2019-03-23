#ifndef PATH_OP_H
#define PATH_OP_H
#include <fs/fs_const.h>

#define NULL_PATH		(char*)0

#define EPATHLONG		-1
#define ENODIR			-2

/* Get the next directory name in a given path. 
 * Directory name is copied into memory address pointed by next_dir!
 * If path is the last part of the 
 * directory tree then return with ENODIR.
 * If directory name is too long to be a valid directory name
 * then return with EPATHLONG. 
 * For example : 
 * get_next_dir_in_path("/usr/ownix/src",...) return with 0
 * get_next_dir_in_path("/ownix",...) return with ENODIR
 * get_next_dir_in_path("/akjsdfkjasdkfkasdjasdfasdfasdf",...) return with EPATHLONG
 */
int get_next_dir_in_path(char* path, char* next_dir);

/* This method will be useful when process a path.
 * Combine with the above we can recursively use this two
 * method, and process the whole path.
 */
char* skip_chars_while_equals(char* path, char* pattern);

#endif
