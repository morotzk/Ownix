#ifndef DRIVER_H
#define DRIVER_H
#include "../include/sys/types.h"
//#include "../iom/iom.h"

#define BLOCK_DEV		1
#define CHAR_DEV		0
#define RAM_DEV			2	/* It means for us it's not a blocking device	*/

/* Not exists a device with specified major id		*/
#define ENODEV			-4
/* Service not implemented by a particular driver	*/
#define ENOIMPL			-8
/* Lseek failed because the size of the disk is 
 * smaller then the current seek pos.
 * Possible on READ, WRITE, LSEEK.
 */
#define ESEEK			-16

typedef struct driver {
	/* function to get a drivers name */
	char* (*get_driver_name)(void);
	/* function to open a device */
	int   (*driver_open)(void);
	/* function to close a device */
	int   (*driver_close)(void);
	/* function to read from a device */
	int  (*driver_read)(void* buffer, size_t nbytes);
	/* function to write to a device */
	int  (*driver_write)(void* buffer, size_t nbytes);
	/* function to set the current read/write position */
	int  (*driver_lseek)(off_t position);
	/* flags */
	u8_t driver_flags;
} driver_t;

char* get_device_name(dev_t major_dev);
u8_t get_device_flags(dev_t major_dev);
int device_open(dev_t major_dev);
int device_close(dev_t major_dev);
int device_read(dev_t major_dev, void* buffer, size_t nbytes);
int device_write(dev_t major_dev, void* buffer, size_t nbytes);
int device_lseek(dev_t major_dev, off_t position);

#endif

