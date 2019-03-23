#include "driver.h"
#include <mem_util.h>

static const char* memdev_name = "mem";
static off_t current_position = 0;

char* get_memdev_name();
int memdev_open();
int memdev_close();
int memdev_read(void* buffer, size_t nbytes);
int memdev_write(void* buffer, size_t nbytes);
int memdev_lseek(off_t position);

char* get_memdev_name() {
	return (char*)memdev_name;
}

int memdev_open() {
	/* No Action on open	*/
	return 0;
}

int memdev_close() {
	/* No Action on close	*/
	return 0;
}

int memdev_read(void* buffer, size_t nbytes) {
	memcpy((void*)current_position, buffer, nbytes);
	current_position += nbytes;
	return nbytes;
}

int memdev_write(void* buffer, size_t nbytes) {
	memcpy(buffer, (void*)current_position, nbytes);
	current_position += nbytes;
	return nbytes;	
}

int memdev_lseek(off_t position) {
	current_position = position;
	return 0;
}

