#include <fs/file_descriptor.h>

struct file_descriptor_entry fd_table[NR_FILE_DESCRIPTORS];

static int initialized = 0;

void init_file_descriptor_table() {
	int i;
	for(i = 0; i < NR_FILE_DESCRIPTORS; ++i) {
		fd_table[i].entry_is_free = 1;
	}
	
	initialized = ~0;
}

struct file_descriptor* find_and_reserve_file_descriptor(int* fd_index) {
	int i;

	if(initialized == 0)
		return NULL_FD;
		
	for(i = 0; i < NR_FILE_DESCRIPTORS; ++i) {
		if(fd_table[i].entry_is_free == 1) {
			fd_table[i].entry_is_free = 0;
			*fd_index = i;
			return &(fd_table[i].fd);
		}
	}
	
	return NULL_FD;
}

struct file_descriptor* find_by_index(int index) {
	if(initialized == 0)
		return NULL_FD;
		
	//Invalid fd index
	if(index < 0 || index >= NR_FILE_DESCRIPTORS)
		return NULL_FD;
		
	return &(fd_table[index].fd);
}

void free_file_descriptor(int index) {
	if(initialized == 0)
		return;
		
	//Invalid fd index
	if(index < 0 || index >= NR_FILE_DESCRIPTORS)
		return;
		
	//TODO : Write the inode stored in the
	//file descriptor back to disc, if 
	//not referenced by others!
	
	fd_table[index].entry_is_free = 1;	
}

int is_file_descriptor_used(int index) {
	//Invalid fd index
	if(index < 0 || index >= NR_FILE_DESCRIPTORS)
		return 1;	
		
	return ~fd_table[index].entry_is_free;
}

