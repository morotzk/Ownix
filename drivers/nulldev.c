#include "driver.h"

static const char* nulldev_name = "null";

char* get_nulldev_name();
int nulldev_open();
int nulldev_close();
int nulldev_read(void* buffer, size_t nbytes);
int nulldev_write(void* buffer, size_t nbytes);
int nulldev_lseek(off_t position);

driver_t nulldev_driver = {
	.get_driver_name = get_nulldev_name,
	.driver_open = nulldev_open,
	.driver_close = nulldev_close,
	.driver_read = nulldev_read,
	.driver_write = nulldev_write,
	.driver_lseek = nulldev_lseek,
	.driver_flags = CHAR_DEV | RAM_DEV
};

char* get_nulldev_name() {
	return (char*)nulldev_name;
}

int nulldev_open() {
	/* no action on nulldev open	*/
	return 0;
}

int nulldev_close() {
	/* no action on nulldev close	*/
	return 0;
}

int nulldev_read(void* buffer, size_t nbytes) {
	return 0;
}

int nulldev_write(void* buffer, size_t nbytes) {
	return 0;
}

int nulldev_lseek(off_t position) {
	return 0;
}

