#include "driver.h"

extern driver_t driver_table[];
extern int get_nr_drivers();

#define NR_DRIVERS		get_nr_drivers()

char* get_device_name(dev_t major_dev) {
	if(major_dev >= NR_DRIVERS) return (char*)0;
	
	if(driver_table[major_dev].get_driver_name == 0)
		return ENOIMPL;
		
	return driver_table[major_dev].get_driver_name();
}

u8_t get_device_flags(dev_t major_dev) {
	if(major_dev >= NR_DRIVERS) return 0;
	
	return driver_table[major_dev].driver_flags;	
}

int device_open(dev_t major_dev) {
	if(major_dev >= NR_DRIVERS) return ENODEV;
	
	if(driver_table[major_dev].driver_open == 0)
		return ENOIMPL;
		
	return driver_table[major_dev].driver_open();
}

int device_close(dev_t major_dev) {
	if(major_dev >= NR_DRIVERS) return ENODEV;
	
	if(driver_table[major_dev].driver_close == 0)
		return ENOIMPL;
	
	return driver_table[major_dev].driver_close();	
}

int device_read(dev_t major_dev, void* buffer, size_t nbytes) {
	if(major_dev >= NR_DRIVERS) return ENODEV;
	
	if(driver_table[major_dev].driver_read == 0)
		return ENOIMPL;
	
	return driver_table[major_dev].driver_read(buffer, nbytes);
}

int device_write(dev_t major_dev, void* buffer, size_t nbytes) {
	if(major_dev >= NR_DRIVERS) return ENODEV;
	
	if(driver_table[major_dev].driver_write == 0)
		return ENOIMPL;
	
	return driver_table[major_dev].driver_write(buffer, nbytes);
}

int device_lseek(dev_t major_dev, off_t position) {
	if(major_dev >= NR_DRIVERS) return ENODEV;
	
	if(driver_table[major_dev].driver_lseek == 0)
		return ENOIMPL;
	
	return driver_table[major_dev].driver_lseek(position);
}

