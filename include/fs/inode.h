#ifndef INODE_H
#define INODE_H
			#include <sys/types.h>
			#include <fs/fs_const.h>
//#include <fs/path_operations.h>

//#include <fs/fs.h>


//#include <ph/sched.h>


/* Error code to show that inode table is full and we cannot allocate a slot  */
#define ENOINODE			-1

/* Inode reside on disk	*/
struct inode {
	mode_t i_mode;			//4
	nlink_t i_nlinks;		//4
	uid_t i_uid;			//4
	gid_t i_gid;			//4
	off_t i_size;			//4
	time_t i_atime;			//4
	time_t i_mtime;			//4
	zone_t i_zone[V2_NR_TZONES];	//9*4 = 36
};

/* Inode reside in core	*/
struct core_inode {
	BIT_T(free_slot);	
	struct inode inode;
	ino_t ino;
};

#define NULL_INODE 			((struct core_inode*)0)

struct dir_ent {
	unsigned int ino;
	unsigned char name[MAX_PATH_LENGTH];
};

#define DIRENT_PER_BLOCK		(BLOCK_SIZE / sizeof(struct dir_ent))

#define ROOTDEV_MAJOR			11

/* get a BLOCK_SIZE size block into buffer at block nr. #block */
int get_disk_block(void* buffer, zone_t block);

/* write a block back to the disk */
int flush_disk_block(void* buffer, zone_t block);

/* get an inode from the disk */
u32_t get_inode_by_path(char* path, struct core_inode* cip);

/* get an inode from the disk by inode-number */
u32_t get_inode_by_ino(ino_t inumber, struct core_inode* cip);

/* write an inode back to the disk */
void flush_inode(struct core_inode* cip);

/* read the file's #fblock_no block into buffer. 
 * fblock_no is the block number in the file 
 */
int get_file_block(struct core_inode* cip, zone_t fblock_no, void* buffer);

/* write the contents of buffer to the disk at file block number #fblock_no */
void flush_file_block(struct core_inode* cip, zone_t fblock_no, void* buffer);

void set_io_req_call_mode(u32_t io_mode);

#endif
