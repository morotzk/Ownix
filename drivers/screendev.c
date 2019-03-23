#include <screen.h>
#include "driver.h"

static const char* screendev_name = "screen";

char* get_screendev_name();
int screendev_write(void* buffer, size_t nbytes);

driver_t screendev_driver = {
	.get_driver_name = get_screendev_name,
	.driver_open = 0,
	.driver_close = 0,
	.driver_read = 0,
	.driver_write = screendev_write,
	.driver_lseek = 0,
	.driver_flags = CHAR_DEV | RAM_DEV
};

char* get_screendev_name() {
	return screendev_name;
}

int screendev_write(void* buffer, size_t nbytes) {
	u32_t i;
	char* s;
	
	s = (char*)buffer;
	for(i = 0; i < nbytes && *s != '\0'; ++i)
		write_screen(*s++);
		
	return i;
}

