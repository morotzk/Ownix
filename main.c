#include <screen.h>
#include <printf.h>
#include <protect.h>
#include <sys/syscall.h>
#include "drivers/driver.h"
#include "drivers/driver_table.h"
#include "drivers/atadev.h"
#include <boot/multiboot.h>
#include <test/test.h>
#include <fs/superblock.h>
#include <fs/inode.h>
#include <fs/fs.h>
#include <fs/file_descriptor.h>
#include <mm/phys_mem.h>
#include <ph/process.h>
#include <ph/sched.h>
#include <system_timer.h>
#include <kheap.h>
#include "iom/iom.h"

/* OwniX version numbers hardcoded		*/
#define RELEASE		1
#define VERSION		0
#define SUBVERSION	1

#define TEST_MODE	

struct multiboot* multiboot_ptr;

extern void init_drivers();

int main() {	
	/* Setup physical memory map */
	fill_phys_mem_map(multiboot_ptr->mem_upper);
	
	/* Initialize the kernel heap */
	init_heap(get_mem_region(PMM_IDX_KHEAP).base,
		  get_mem_region(PMM_IDX_KHEAP).size);	
	
	/* Initialise the console service	*/
	init_screen();
	
	/* count the number of free page frames and mark them as free */
	count_free_page_frames();
	
	/* Print some welcome message		*/
	printf("OwniX operating system %u.%u.%u\n",RELEASE, VERSION, SUBVERSION);
	printf("Copyright (C) 2011 - Kenez Morotz\n");
	/* Show the size of available memory	*/
	printf("%d Kilobytes of available memory.\n", get_phys_mem_size());	
	
	/* Initialize the driver table		*/
	init_drivers();
	
	/*Show initrd info and setup ramdisk!	*/
	u32_t ramdisk_base = *((u32_t*)multiboot_ptr->mods_addr);
	u32_t ramdisk_end = *(u32_t*)(multiboot_ptr->mods_addr + 4);
	u32_t ramdisk_size = ramdisk_end - ramdisk_base; 
	printf("\ninitrd loaded at 0x%x\ninitrd size is 0x%x Kilobytes\n\n", ramdisk_base, ramdisk_size);
	
	setup_ramdisk(ramdisk_base, ramdisk_size);
	
	init_fs();

	/* do the first executable files loading with SYNCHRONE I/O logic */
	set_io_logic(IO_LOGIC_SYNC);
	set_io_req_call_mode(IOM_CALL);
	init_proc_handler();	
	
	#ifdef TEST_MODE
		testmain();
		panic("TEST mode execution finished!\n");
	#endif
	
	
	if(load_exec("/etc/oexec") != 0) {
		panic("Cannot load the specified executable file!\n");
	}

	/* in multitasking use the ASYNC logic */
	set_io_logic(IO_LOGIC_ASYNC);
	set_io_req_call_mode(IOM_SYSCALL);	
	set_multitasking_mode(1);	
	
	/* raise a timer interrupt (theoretically) in every millisecond */
	system_timer_setup(1000);

	/* start multitasking */
	system_timer_start();
	while(1) ;
	
	panic("Kernel execution finished!\nKernel IDLE process failed to start!");
	
	return 0;
}
