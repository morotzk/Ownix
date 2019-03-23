#include <fs/inode.h>
#include "../drivers/driver.h"
#include <lib/string.h>
#include <mem_util.h>
#include <printf.h>
#include <protect.h>
#include <kheap.h>
#include "../iom/iom.h"			/* include the io manager */

u32_t io_m;

extern u32_t unified_syscall(u32_t eax, u32_t ebx, u32_t ecx, u32_t edx);

static s32_t send_io_request(struct io_request* iorequest) {
	if(io_m == IOM_CALL) {
		serve_ior(iorequest);
		return iorequest->success;
	} else if(io_m == IOM_SYSCALL) {		
		return unified_syscall(SYS_IOREQ, (u32_t)iorequest, 0, 0);
	} else {
		panic("Unknown io call mode.");
	}	
}

int get_disk_block(void* buffer, zone_t block) {
	s32_t success;
	struct io_request dior;
	
	/* Send an lseek io-request */
	dior.ior_type = IOR_TYPE_LSEEK;
	dior.major_dev = ROOTDEV_MAJOR;
	dior.lseek_ior.position = block * BLOCK_SIZE;
	
	success = send_io_request(&dior);
	if(success < 0) return success;
	
	/* Send a read io-request */
	dior.ior_type = IOR_TYPE_READ;	
	dior.major_dev = ROOTDEV_MAJOR;
	dior.read_ior.buf = buffer;
	dior.read_ior.nbytes = BLOCK_SIZE;	
	
	success = send_io_request(&dior);	
	return success;
}

int flush_disk_block(void* buffer, zone_t block) {
	s32_t success;
	struct io_request dior;
	
	/* Send an lseek io-request */
	dior.ior_type = IOR_TYPE_LSEEK;	
	dior.major_dev = ROOTDEV_MAJOR;
	dior.lseek_ior.position = block * BLOCK_SIZE;
	
	success = send_io_request(&dior); 
	if(success < 0) return success;
	
	/* Send a read io-request */
	dior.ior_type = IOR_TYPE_WRITE;
	dior.major_dev = ROOTDEV_MAJOR;
	dior.write_ior.buf = buffer;
	dior.write_ior.nbytes = BLOCK_SIZE;	
	
	success = send_io_request(&dior);	
	return success;
}

/* helper function for get_inode_by_path */
static u32_t scan_dir(struct core_inode* cip, char* component, ino_t* inumber) {
	u32_t nr_blk, i;
	zone_t cb;
	struct dir_ent* de;
	u8_t dir_blk[BLOCK_SIZE];
	int s;
	if(cip->inode.i_mode != S_IFDIR)
		/* not a directory */
		return 1;
	
	nr_blk = cip->inode.i_size / BLOCK_SIZE;
	if(cip->inode.i_size % BLOCK_SIZE != 0)
		nr_blk += 1;
		
	for(cb = 0; cb < nr_blk; ++cb) {
		s = get_file_block(cip, cb, (void*)dir_blk);
		if(s < 0) return 1;

		de = (struct dir_ent*)dir_blk;
		
		for(i = 0; i < DIRENT_PER_BLOCK; ++i) {				
			if(strncmp(de[i].name, component, strlen(component)) == 0) {
				*inumber = de[i].ino;				
				return 0;
			}
		}
	}	
	
	/* NOT FOUND */
	return 1;
}

u32_t get_inode_by_path(char* path, struct core_inode* cip) {
	char* p = path;
	char *comp, *c;
	struct core_inode dinode;
	ino_t next_ino;
	u32_t found;
	
	if(!p) return 1;
	
	/* root dir inode has a fixed inode number here 1 */
	next_ino = 1;
	c = (char*)KMALLOC(2 * MAX_PATH_LENGTH + 1);
	
	/* split the next comp(onent) of the path */
	split:
		if(*p == '/') ++p;
	
		comp = c;
		while(*p != '/' && *p != '\0') {
			*comp++ = *p++;
		}
		*comp = '\0';
		get_inode_by_ino(next_ino, &dinode);
		found = scan_dir(&dinode, c, &next_ino);				
		if(found == 1) { 				/* error : component is not a directory or dir entry not found */
			KFREE((void*)c); 
			return 1; 
		}
		if(*p == '\0') {				
			get_inode_by_ino(next_ino, cip);			
			KFREE((void*)c);
			return 0;
		}
		goto split;
}

u32_t get_inode_by_ino(ino_t inumber, struct core_inode* cip) {
	zone_t iblock;
	u32_t ino_per_block;
	u32_t ino_id;
	u8_t inode_block[BLOCK_SIZE];
	
	ino_per_block = BLOCK_SIZE / sizeof(struct inode);
	iblock = (zone_t)(IAREA_OFF / BLOCK_SIZE) + (zone_t)(inumber / ino_per_block);
	ino_id = inumber % ino_per_block;
	get_disk_block((void*)inode_block, iblock);	
	
	cip->ino = inumber;
	memcpy((void*)(inode_block+ino_id*sizeof(struct inode)), &(cip->inode), sizeof(struct inode));
}

void flush_inode(struct core_inode* cip) {
	zone_t iblock;
	u32_t ino_per_block;
	u32_t ino_id;
	u8_t inode_block[BLOCK_SIZE];
	
	ino_per_block = BLOCK_SIZE / sizeof(struct inode);
	iblock = (zone_t)(IAREA_OFF / BLOCK_SIZE) + (zone_t)(cip->ino / ino_per_block);
	ino_id = cip->ino % ino_per_block;
	get_disk_block((void*)inode_block, iblock);
	
	memcpy(&(cip->inode), (void*)(inode_block+ino_id*sizeof(struct inode)), sizeof(struct inode));	
	flush_disk_block((void*)inode_block, iblock);	
}

#define CHECK_EOF_AND_RET(b,z) \
do { \
rbytes = get_disk_block((void*) b , z ); \
if(rbytes < 0) return -1; \
int sz_diff = cip->inode.i_size - (fblock_no * BLOCK_SIZE); \
if(sz_diff >= BLOCK_SIZE) sz_diff = BLOCK_SIZE; \
if(rbytes < sz_diff) return rbytes; \
return sz_diff; \
} while(0); \

int get_file_block(struct core_inode* cip, zone_t fblock_no, void* buffer) {
	int rbytes;
	zone_t ind_blk_no, dbl_ind_blk_no, abs_blk_no;
	u32_t ind_blk_buf[256];
	u32_t dbl_ind_blk_buf[256];	
	
	ind_blk_no = 0;
		
	/* how many blocks has the file? */
	if(cip->inode.i_size < (fblock_no * BLOCK_SIZE)) {		
		return -2;
	}
	
	/* Is the block address stored in the inode itself? */
	if(fblock_no >= 7) {		
		if(fblock_no >= 263) {
			dbl_ind_blk_no = fblock_no - 263 / 256;
			ind_blk_no = dbl_ind_blk_no % 256;
						
			/* read the double indirect block */
			rbytes = get_disk_block((void*)dbl_ind_blk_buf, cip->inode.i_zone[8]);
			if(rbytes != BLOCK_SIZE) return -1;
						
			/* read the corr. simple indirect block */
			rbytes = get_disk_block((void*)ind_blk_buf, dbl_ind_blk_buf[dbl_ind_blk_no]);
			if(rbytes != BLOCK_SIZE) return -1;	
			/* and finally read the addressed block */
			abs_blk_no = ind_blk_buf[ind_blk_no];
			
			CHECK_EOF_AND_RET(buffer,abs_blk_no)
		} else {
			/* address of the referenced block will be at #ind_blk_no index */
			ind_blk_no = fblock_no - 7;
			
			/* read the simple indirect block */
			rbytes = get_disk_block((void*)ind_blk_buf, cip->inode.i_zone[7]);
			if(rbytes != BLOCK_SIZE) return -1;
			
			/* get the absolute block number */
			abs_blk_no = ind_blk_buf[ind_blk_no];
			
			/* read the block */
			CHECK_EOF_AND_RET(buffer,abs_blk_no)
		}		
	} else {
		CHECK_EOF_AND_RET(buffer,cip->inode.i_zone[fblock_no])
	}
}

void flush_file_block(struct core_inode* cip, zone_t fblock_no, void* buffer) {
	zone_t ind_blk_no, dbl_ind_blk_no, abs_blk_no;
	u32_t ind_blk_buf[256];
	u32_t dbl_ind_blk_buf[256];	
	
	ind_blk_no = 0;
	
	/* Is the block address stored in the inode itself? */
	if(fblock_no >= 7) {		
		if(fblock_no >= 263) {
			dbl_ind_blk_no = fblock_no - 263 / 256;
			ind_blk_no = dbl_ind_blk_no % 256;
						
			/* read the double indirect block */
			get_disk_block((void*)dbl_ind_blk_buf, cip->inode.i_zone[8]);
						
			/* read the corr. simple indirect block */
			get_disk_block((void*)ind_blk_buf, dbl_ind_blk_buf[dbl_ind_blk_no]);
						
			/* and finally write the addressed block */
			abs_blk_no = ind_blk_buf[ind_blk_no];
						
			flush_disk_block((void*)buffer, abs_blk_no);
		} else {
			/* address of the referenced block will be at #ind_blk_no index */
			ind_blk_no = fblock_no - 7;
			
			/* read the simple indirect block */
			get_disk_block((void*)ind_blk_buf, cip->inode.i_zone[7]);
			
			/* get the absolute block number */
			abs_blk_no = ind_blk_buf[ind_blk_no];
			
			/* write the block */
			flush_disk_block((void*)buffer, abs_blk_no);			
		}		
	} else {
		flush_disk_block((void*)buffer, cip->inode.i_zone[fblock_no]);
	}	
}

void set_io_req_call_mode(u32_t io_mode) {
	io_m = io_mode;
}

