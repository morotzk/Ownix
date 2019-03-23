#ifndef FS_H
#define FS_H
#include <sys/types.h>
#include <fs/inode.h>

/* max number of open files in the whole system globally */
#define SYS_OPEN_MAX		32

/* max number of open files per process */
#define PROC_OPEN_MAX		8

/* system file descriptor structure */
struct filed {
	struct core_inode file_inode;
	u32_t open_count;
};

#define NULL_FILED		(struct filed *)0
#define NULL_FILE			(struct file *)0	

/* user file descriptor */
struct file {
	mode_t open_mode;
	off_t file_pos;
	struct filed* fdp;
};

/* stored in the process descriptor */
struct file_tab {
	u8_t fbmp;
	/* opened files */
	struct file ofile[PROC_OPEN_MAX];
};

/* system file descriptor table */
struct filed_tab {
	struct filed fd[SYS_OPEN_MAX];
	u32_t fbmp;
};

#define ENOFILE			(-1)
#define EFMAX				(-2)	
#define EALLOW				(-3)

extern u32_t first_set_bit(u32_t bitmap);

#define IOM_SYSCALL		0
#define IOM_CALL			1

#define OMODE_RD			1
#define OMODE_WR			2

void init_fs();

int open(char* path, mode_t mode, struct file* file);
int close(struct file* file);
int read(struct file* file, void* buffer, size_t nbytes);
s32_t lseek(struct file* file, off_t seek_pos);
s32_t ll_read(struct core_inode* cip, void* buffer, off_t pos,size_t nbytes);
/* set wether we access io-manager via system call or normal call */
void set_io_req_call_mode(u32_t io_mode);

#endif
