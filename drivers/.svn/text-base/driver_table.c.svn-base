#include "driver.h"
#include "driver_table.h"

extern driver_t nulldev_driver;
extern driver_t zerodev_driver;
extern driver_t ramdiskdev_driver;
extern driver_t atadev_driver;
extern driver_t timedev_driver;
extern driver_t screendev_driver;

#define NR_DRIVERS 		16

driver_t driver_table[NR_DRIVERS];

int get_nr_drivers();

void init_drivers();

int get_nr_drivers() {
	return NR_DRIVERS;
}

void register_driver(int dev_major, driver_t driver) {
	driver_table[dev_major] = driver;
}

void init_drivers() {
	register_driver(NULLDEV_MAJOR, nulldev_driver);
	register_driver(ZERODEV_MAJOR, zerodev_driver);
	register_driver(RAMDISKDEV_MAJOR, ramdiskdev_driver);
	register_driver(ATADEV_MAJOR, atadev_driver);
	register_driver(TIMEDEV_MAJOR, timedev_driver);
	register_driver(SCREENDEV_MAJOR, screendev_driver);
}

