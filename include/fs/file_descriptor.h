#ifndef FILE_DESC_H
#define FILE_DESC_H
#include <sys/types.h>
#include <fs/inode.h>
#include <fs/fs_const.h>

struct file_descriptor {
	mode_t file_mode;
	nlink_t file_count;
	off_t file_pos;
	struct core_inode file_inode;
};

struct file_descriptor_entry {
	BIT_T(entry_is_free);
	struct file_descriptor fd;
};

#define NULL_FD				(struct file_descriptor*)0

/* System-wide use 128 file descriptors 	*/
#define NR_FILE_DESCRIPTORS		128

/* Use this error code when table is full	*/
#define ENOFD				-1

void init_file_descriptor_table();
struct file_descriptor* find_and_reserve_file_descriptor(int* fd_index);
struct file_descriptor* find_by_index(int index);
void free_file_descriptor(int index);
int is_file_descriptor_used(int index);

#endif
