#include <sys/types.h>
#include <ds/queue.h>
#include "../drivers/driver.h"

#define EBUSY					-3
#define ENTRY_Q_SZ			5

#define IOR_TYPE_BASE		32
#define IOR_TYPE_READ		IOR_TYPE_BASE + 1
#define IOR_TYPE_WRITE		IOR_TYPE_BASE + 2
#define IOR_TYPE_OPEN		IOR_TYPE_BASE + 3
#define IOR_TYPE_CLOSE		IOR_TYPE_BASE + 4
#define IOR_TYPE_LSEEK		IOR_TYPE_BASE + 5

#define IO_LOGIC_SYNC		1		/* Every IO-request will busy wait (SYNCHRONE IO) */
#define IO_LOGIC_ASYNC		0		/* Asynchron IOR */
#define IOR_COMPLETED		16		/* IOR_COMPLETED event const */

typedef dev_t iorid_t;

struct io_request {
	u32_t ior_type;
	pid_t reqor_pid;
	dev_t major_dev;
	union {
		struct rd_ios {
			void* buf;
			size_t nbytes;
		} read_ior;
		
		struct wr_ios {
			void* buf;
			size_t nbytes;
		} write_ior;
		
		struct lseek_ios {
			off_t position;
		} lseek_ior;
	};
	
	/* unused now */
	u8_t flags;
	/* field is used when we call io manager as a function */
	s32_t success;
};

void serve_ior(struct io_request* ior);
void set_io_logic(u32_t iol);
