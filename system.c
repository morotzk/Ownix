/***********************************************
 * This module implements the system call
 * services defined by the OwniX 
 * operating system.
************************************************/ 
#include <sys/system.h>
#include <ph/process.h>
#include <ph/sched.h>
#include <mm/virt_mem.h>
#include <fs/fs.h>
#include <printf.h>

/* current proc abbreviation */
#define __cp						sched_get_curr_proc()				

/* do read system call */
void do_read(int fd, void* buffer, size_t len) {
	struct file_tab* ftp;	
	
	if(fd < 0 || fd >= PROC_OPEN_MAX) {
		finalize_syscall(__cp -> pid, -2);
		return;
	}
	
	ftp = &(__cp -> ftab);
	if(!(ftp->fbmp & (1 << fd))) {	
		/* buffer is on an illegal virt. mem addr. */
		if(vm_mapping(__cp -> vm.pd, (u32_t)buffer) == ILLEGAL_ACCESS) {			
			finalize_syscall(__cp -> pid, -3);		
		} else {			
			finalize_syscall(__cp -> pid, read(&ftp->ofile[fd], virt_to_phys(__cp -> vm.pd, (u32_t)buffer), len));
		}		
	} else {
		finalize_syscall(__cp -> pid, -2);
	}
}

/* do write to console system call */
void do_write_cons(char* s) {
	if(vm_mapping(& ( __cp -> vm ), (u32_t)s) == ILLEGAL_ACCESS) {
		finalize_syscall(__cp -> pid, -3);
	} else {
		finalize_syscall(__cp -> pid, printf((char*)virt_to_phys(__cp -> vm.pd, (u32_t)s)));
	}
}

/* do open system call */
void do_open(char* path, mode_t mode) {
	int fd;
	int success;

	if(vm_mapping(__cp -> vm.pd, (u32_t)path) == ILLEGAL_ACCESS) {
		finalize_syscall(__cp -> pid, -3);
		return;
	}
	
	/* get a free file pointer */
	struct file_tab* ftp = &(__cp -> ftab);
	
	fd = first_set_bit(ftp->fbmp);
	if(fd == 32) {
		finalize_syscall(__cp -> pid, -1);
	}
	
	/* reserve fd */
	ftp->fbmp &= ~(1 << fd);
	
	success = open(path, mode, &ftp->ofile[fd]);
	if(success != 0)
		finalize_syscall(__cp -> pid, success);
	else
		finalize_syscall(__cp -> pid, fd);
}

/* do close system call */
void do_close(int fd) {
	struct file_tab* ftp = &(__cp -> ftab);
	
	if(fd < 0 || fd >= PROC_OPEN_MAX) {
		finalize_syscall(__cp -> pid, -2);
		return;
	}
	
	if(!(ftp->fbmp & (1 << fd))) {
		/* free fd */
		ftp->fbmp |= (1 << fd);
		
		finalize_syscall(__cp -> pid, close(&ftp->ofile[fd]));
	} else {
		finalize_syscall(__cp -> pid, -2);
	}
}

/* do lseek system call */
void do_lseek(int fd, off_t offset) {
	struct file_tab* ftp;
	
	if(fd < 0 || fd >= PROC_OPEN_MAX) {
		finalize_syscall(__cp -> pid, -2);
		return;
	}		
		
	ftp = &(__cp -> ftab);	
	finalize_syscall(__cp -> pid, lseek(&ftp->ofile[fd], offset));
}

/* do exit system call */
void do_exit(int status) {
	delete_proc_context(__cp -> pid);
}

/* do getpid system call */
void do_getpid() {
	finalize_syscall(__cp -> pid, __cp -> pid);
}
