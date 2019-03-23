#include <fs/fs.h>
#include <kheap.h>
#include <mem_util.h>
#include <protect.h>
#include <printf.h>

#define INDEXOF(elem_addr,elem_sz,arr_base) ((elem_addr - arr_base)/elem_sz)

/* file descriptor table */
struct filed_tab fd_tab;

static u32_t get_free_fd() {
	u32_t ffd = first_set_bit(fd_tab.fbmp);
	if(ffd == 32) panic("Out of file descriptors.");
	
	return ffd;
}

static void res_fd(u32_t fd) {
	if(fd < 0 || fd >= SYS_OPEN_MAX) panic("Illegal fd reference.");	
	fd_tab.fbmp &= ~(1 << fd);
}

static void free_fd(u32_t fd) {
	if(fd < 0 || fd >= SYS_OPEN_MAX) panic("Illegal fd reference.");	
	fd_tab.fbmp |= (1 << fd);
}

void init_fs() {
	fd_tab.fbmp = 0xffffffff;
}

int open(char* path, mode_t mode, struct file* file) {
	int fd;
	struct core_inode f_inode;	
	/* get_inode will invoke an IO-request! */
	if(get_inode_by_path(path, &f_inode) != 0)
		/* No such file */
       		return ENOFILE;
       	
   fd = get_free_fd();       	
   res_fd(fd);       	
   fd_tab.fd[fd].open_count = 1;
   fd_tab.fd[fd].file_inode = f_inode;

	file->fdp = &fd_tab.fd[fd];
	file->open_mode = mode;
	file->file_pos = 0;
	
	/* OK */
	return 0;      		
}

int close(struct file* file) {		
	if(file->fdp->open_count == 1) {
		file->fdp->open_count = 0;
		free_fd(INDEXOF(file->fdp,sizeof(struct filed), fd_tab.fd));
	} else if(file->fdp->open_count > 1) {
		file->fdp->open_count -= 1;
	}
	
	/* OK */
	return 0;
}

int read(struct file* file, void* buffer, size_t nbytes) {			
	struct core_inode fin;
	
	/* Check read is allowed */
	if(!(file->open_mode & OMODE_RD))
		return EALLOW;
	
	fin = file->fdp->file_inode;

	/* read_file_blocks may invoke io_request also! */	
	s32_t rbytes = (s32_t)ll_read(&fin, buffer, file->file_pos, nbytes);
	
	/* error */
	if(rbytes < 0) return rbytes;
	file->file_pos += rbytes;
	
	return rbytes;
}

s32_t ll_read(struct core_inode* cip, void* buffer, off_t pos,size_t nbytes) {	
	u32_t fb;		/* first file block to read */	
	u32_t bpos;		/* position in the buffer from we copy nbytes data to buffer*/	
	u32_t nblocks;		/* total number of blocks to read */	
	char* ibuf;		/* internal buffer */
	u32_t b;
	
	int nb, rb;
	
	nb = 0;
	switch(cip->inode.i_mode) {
		case S_IFREG : {
			fb = pos / BLOCK_SIZE;
			nblocks = (fb + nbytes + pos % BLOCK_SIZE) / BLOCK_SIZE;
			if((fb + nbytes + pos % BLOCK_SIZE) % BLOCK_SIZE != 0)
				++nblocks;
				
			bpos = pos % BLOCK_SIZE;			
			ibuf = (char*)KMALLOC(nblocks * BLOCK_SIZE);

			if((void*)ibuf == NULL) panic("Cannot allocate buffer.");
			
			for(b = fb; (b - fb) < nblocks; ++b) {														
				rb = get_file_block(cip, b, (void*)(ibuf+(b - fb)*BLOCK_SIZE));				
				if(rb < 0) {
					return rb;
				}
				nb += rb;
			}

			if(nb < nbytes)	{		
				memcpy((void*)(ibuf+bpos), buffer, nb);
				return nb;
			} else {
				memcpy((void*)(ibuf+bpos), buffer, nbytes);
				return nbytes;
			}
			
			KFREE((void*)ibuf);			
			return nb;			
		}
		default:
			/* i_mode currently not supported */
			return -1;
	}
}

s32_t lseek(struct file* file, off_t seek_pos) {
	if(file == NULL_FILE) return -1;
	if(file->fdp == NULL_FILED) return -2;
	if(seek_pos >= file->fdp->file_inode.inode.i_size) {
		return -3;
	} 
	file->file_pos = seek_pos;
	return 0;
}

