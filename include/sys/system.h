#ifndef SYSTEM_H
#define SYSTEM_H
#include <sys/types.h>
//#include "../../iom/iom.h"

/* System call numbers	*/
#define SYS_WRITE_CONS		4
#define SYS_GETPID			5
#define SYS_IOREQ				6
#define SYS_READ				7
#define SYS_OPEN				8
#define SYS_CLOSE				9
#define SYS_LSEEK				10
#define SYS_EXIT				11

#define NR_SYSCALL	15

/* currently available and implemented system call serving methods */
void do_read(int fd, void* buffer, size_t len);
void do_write_cons(char* s);
void do_open(char* path, mode_t mode);
void do_close(int fd);
void do_lseek(int fd, off_t offset);
void do_exit(int status);
void do_getpid();
//void do_io_request(struct io_request* iorp);

//void do_write(int fd, void* buffer, size_t len);

///
///	Rejected in the final version
///
//#define SYS_READ			0
//#define SYS_WRITE			1
//#define SYS_OPEN			2
//#define SYS_CLOSE			3
//#define SYS_IOCTL			4
//#define SYS_LSEEK			5
//#define SYS_EXIT			6
//#define SYS_GETPID			7
//#define SYS_FORK			8
//#define SYS_IOREQ			9
//#define SYS_EXEC			10
//#define SYS_FORKEXEC		11
//#define SYS_STAT			12
//#define SYS_MMAP			13
//#define SYS_MKNOD			14
//#define SYS_WAITPID		15

#endif
