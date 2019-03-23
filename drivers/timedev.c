#include <port_io.h>
#include <sys/types.h>
#include "driver.h"

#define CMOS_COMMAND		0x70
#define CMOS_DATA		0x71
#define CMOS_REG_B 		0xB
#define CMOS_HOUR_INDEX 	0x04
#define CMOS_MINUTE_INDEX	0x02
#define CMOS_SECOND_INDEX	0x00
#define CMOS_YEAR_INDEX		0x09
#define CMOS_MONTH_INDEX	0x08
#define CMOS_DAY_INDEX		0x07

//from A.S.T Operating System book
static int bcd_to_dec(int n) {
	return ((n >> 4) & 0x0F) * 10 + (n & 0x0F);
}

static const char* timedev_name = "time";

char* get_timedev_name();
int timedev_read(void* buffer, size_t nbytes);

driver_t timedev_driver = {
	.get_driver_name 	= get_timedev_name,
	.driver_open 		= 0,
	.driver_close 		= 0,	
	.driver_read 		= timedev_read,
	.driver_write 		= 0,
	.driver_lseek 		= 0,
	.driver_flags 		= CHAR_DEV | RAM_DEV
};

char* get_timedev_name() {
	return timedev_name;
}

int timedev_read(void* buffer, size_t nbytes) {
	struct time* time = (struct time*)buffer;
	if(nbytes < sizeof(struct time))
		return -1;
		
	outportb(CMOS_COMMAND, CMOS_HOUR_INDEX);
	time->hour = bcd_to_dec(inportb(CMOS_DATA));
	
	outportb(CMOS_COMMAND, CMOS_MINUTE_INDEX);
	time->minute = bcd_to_dec(inportb(CMOS_DATA));
	
	outportb(CMOS_COMMAND, CMOS_SECOND_INDEX);
	time->second = bcd_to_dec(inportb(CMOS_DATA));
	
	outportb(CMOS_COMMAND, CMOS_YEAR_INDEX);
	time->year = bcd_to_dec(inportb(CMOS_DATA))+2000;
	
	outportb(CMOS_COMMAND, CMOS_MONTH_INDEX);
	time->month = bcd_to_dec(inportb(CMOS_DATA));
	
	outportb(CMOS_COMMAND, CMOS_DAY_INDEX);
	time->day = bcd_to_dec(inportb(CMOS_DATA));
	
	return sizeof(struct time);
}

