#include "driver.h"
#include <sys/types.h>
#include <mem_util.h>
#include "ramdiskdev.h"
#include <printf.h>

#define SEEK_POS(pos) (pos < ramdisk_size)
#define ASSERT_INITIALIZED   if(ramdisk_base == 0) return -1

static const char* ramdiskdev_name = "ramdisk";
/* These vars being initialized after 
 * setup_ramdisk called 
 * which get its parameters from GRUB	
 */
static u32_t ramdisk_base = 0;
static u32_t ramdisk_size = 0;
static off_t seek_pos = 0;

char* get_ramdiskdev_name();
int ramdiskdev_open();
int ramdiskdev_close();
int ramdiskdev_read(void* buffer, size_t nbytes);
int ramdiskdev_write(void* buffer, size_t nbytes);
int ramdiskdev_lseek(off_t position);

driver_t ramdiskdev_driver = {
	.get_driver_name = get_ramdiskdev_name,
	.driver_open = ramdiskdev_open,
	.driver_close = ramdiskdev_close,
	.driver_read = ramdiskdev_read,
	.driver_write = ramdiskdev_write,
	.driver_lseek = ramdiskdev_lseek,
	.driver_flags = BLOCK_DEV | RAM_DEV
};

char* get_ramdiskdev_name() {
	return (char*)ramdiskdev_name;
}

int ramdiskdev_open() {
	seek_pos = 0;
	return 0;
}

int ramdiskdev_close() {
	seek_pos = 0;
	return 0;
}

int ramdiskdev_read(void* buffer, size_t nbytes) {
	int nb;
	
	ASSERT_INITIALIZED;
			
	if(seek_pos + nbytes >= ramdisk_size) 
		nb = ramdisk_size - seek_pos;
	else 
		nb = (int)nbytes;	
	
	memcpy((void*)((u32_t)ramdisk_base+seek_pos), (void*)buffer, nb);	
	seek_pos += nb;
	
	return nb;	
}

int ramdiskdev_write(void* buffer, size_t nbytes) {
	int nb;
	
	ASSERT_INITIALIZED;
	if(seek_pos + nbytes >= ramdisk_size)
		nb = ramdisk_size - seek_pos;
	else
		nb = (int)nbytes; 
		
		
	memcpy((void*)buffer, (void*)((u32_t)ramdisk_base+seek_pos), nb);	
	/* Update seek position!	*/
	seek_pos += nb;
	
	return nb;
}

int ramdiskdev_lseek(off_t position) {	
	if(seek_pos >= ramdisk_size) {
		return ESEEK;
	}	
		
	seek_pos = position;
	return 0;				
}

void setup_ramdisk(u32_t ramdisk_start, u32_t ramdisk_len) {
	ramdisk_base = ramdisk_start;
	ramdisk_size = ramdisk_len;		
}

