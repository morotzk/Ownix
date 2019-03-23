#include <fs/file_open.h>

int file_open(char* path) {	
	int fd;

	/* Try to read the inode corresponding to path	*/
	struct core_inode* fip = read_inode(path);
	
	/* Check whether it was successful		*/
	if(fip == NULL_INODE)
		return ENOFILE;
		
	/* Try to reserve a free file descriptor	*/
	struct file_descriptor* fdp = find_and_reserve_file_descriptor(&fd);
	
	if(fdp == NULL_FD)
		return EMAX;
		
	/* Set fd's inode to fip which was the corresponding inode	*/
	fdp->file_inode = *fip;
	
	/* Free the inode from the inode table in order to reuse it later	*/
	free_inode(fip);
	
	/* Reset the fields of fdp	*/
	fdp->file_mode = 0;
	fdp->file_count = 1;
	fdp->file_pos = 0;
	
	return fd;
}

