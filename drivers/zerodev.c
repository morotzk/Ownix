#include "driver.h"

static const char* zerodev_name = "zero";

char* get_zerodev_name();
int zerodev_open();
int zerodev_close();
int zerodev_read(void* buffer, size_t nbytes);
int zerodev_write(void* buffer, size_t nbytes);
int zerodev_lseek(off_t position);

driver_t zerodev_driver = {
	.get_driver_name = get_zerodev_name,
	.driver_open = zerodev_open,
	.driver_close = zerodev_close,
	.driver_read = zerodev_read,
	.driver_write = zerodev_write,
	.driver_lseek = zerodev_lseek,
	.driver_flags = CHAR_DEV | RAM_DEV
};

char* get_zerodev_name() {
	return (char*)zerodev_name;
}

int zerodev_open() {
	/* No action on open	*/
	return 0;
}

int zerodev_close() {
	/* No action on close	*/
	return 0;
}

int zerodev_read(void* buffer, size_t nbytes) {
	int i;
	char* buf;
	
	if(nbytes < 0)
		return -1;
	
	buf = (char*)buffer;
	for(i = 0; i < nbytes; ++i) {
		buf[i] = 0;
	}
	
	return nbytes;
}

int zerodev_write(void* buffer, size_t nbytes) {
	/* zerodev is not writable just readable	*/
	return ENOIMPL;
}

int zerodev_lseek(off_t position) {
	/* zerodev doesn't support lseek because it is a character device */
	return ENOIMPL;
}

