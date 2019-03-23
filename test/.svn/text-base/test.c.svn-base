#include <test/test.h>
#include <mm/virt_mem.h>

#define TEST_BEGIN		screen_attr_t old_attr = get_attribute()
#define TEST_END		set_attribute(old_attr)	

#define ATADEV_MAJOR		7

#define typed_equals(type_name) \
int type_name ## _equals(void* a, void* b) { \
type_name *  ptr_a = (type_name *)a; \
type_name *  ptr_b = (type_name *)b; \
if(*ptr_a == *ptr_b) \
return 1; \
else \
return 0; \
} \

typed_equals(int)

typed_equals(short)

typed_equals(mode_t)

typedef int (*equals_t)(void* a, void* b);

static int nr_tests = 0;
static int nr_succ  = 0;
static int nr_fail  = 0;

void print_test_header() {
	clear_screen();
	printf("***********************************************************************\n");
	printf("                OwniX Operating system TEST Mode!\n");
	printf("***********************************************************************\n");
}

void print_test_summary() {
	printf("\nSummary of test results: \n---------------------------\n");
	printf("Succeeded: \t%d\nFailed:\t\t%d\n", nr_succ, nr_fail);
	printf("---------------------------\nSum : \t\t%d\n", nr_tests);
}

void print_finish_time() {
	struct time t;
	
	int status = device_read(15, (void*)&t, sizeof(struct time));

	printf("\nTest execution finished at %d-0%d-0%d %d:%d:%d .\n", t.year, t.month, t.day, t.hour, t.minute, t.second);
}

void assert_not_null(void* p, char* message) {
	TEST_BEGIN;
	++nr_tests;

	if(p == NULL) {
		set_attribute(NEW_ATTR(COLOR_RED));
		printf("ASSERTION [%s] FAILED.\n", message);
		++nr_fail;
	} else {
		set_attribute(NEW_ATTR(COLOR_GREEN));
		printf("ASSERTION [%s] SUCCEEDED.\n", message);
		++nr_succ;
	}
	
	TEST_END;
}

void assert_equals(equals_t equals_function, void* a, void* b, char* message) {
	TEST_BEGIN;
	++nr_tests;

	if(equals_function(a, b) == 0) {
		set_attribute(NEW_ATTR(COLOR_RED));
		printf("ASSERTION [%s] FAILED.\n", message);
		++nr_fail;		
	} else {
		set_attribute(NEW_ATTR(COLOR_GREEN));
		printf("ASSERTION [%s] SUCCEEDED.\n", message);
		++nr_succ;
	}
	
	TEST_END;
}

/* Assertion for functions returned 0 for success 
 * or a negative number on error.
 */
void assert_zero(int a, char* message) {
	TEST_BEGIN;
	++nr_tests;
	
	if(a == 0) {
		set_attribute(NEW_ATTR(COLOR_GREEN));
		printf("ASSERTION [%s] SUCCEEDED.\n", message);
		++nr_succ;
	} else {
		set_attribute(NEW_ATTR(COLOR_RED));
		printf("ASSERTION [%s] FAILED.\n", message);
		++nr_fail;
	}		
	
	TEST_END;
}

void assert_not_zero(int a, char* message) {
	TEST_BEGIN;
	++nr_tests;
	
	if(a != 0) {
		set_attribute(NEW_ATTR(COLOR_GREEN));
		printf("ASSERTION [%s] SUCCEEDED.\n", message);
		++nr_succ;
	} else {
		set_attribute(NEW_ATTR(COLOR_RED));
		printf("ASSERTION [%s] FAILED.\n", message);
		++nr_fail;
	}	
	
	TEST_END;
}

void assert_positive(int a, char* message) {
	TEST_BEGIN;
	++nr_tests;
	
	if(a > 0) {
		set_attribute(NEW_ATTR(COLOR_GREEN));
		printf("ASSERTION [%s] SUCCEEDED.\n", message);
		++nr_succ;
	} else {
		set_attribute(NEW_ATTR(COLOR_RED));
		printf("ASSERTION [%s] FAILED.\n", message);
		++nr_fail;
	}	
	
	TEST_END;
}

void assert_negative(int a, char* message) {
	TEST_BEGIN;
	++nr_tests;
	
	if(a < 0) {
		set_attribute(NEW_ATTR(COLOR_GREEN));
		printf("ASSERTION [%s] SUCCEEDED.\n", message);
		++nr_succ;
	} else {
		set_attribute(NEW_ATTR(COLOR_RED));
		printf("ASSERTION [%s] FAILED.\n", message);
		++nr_fail;
	}	
	
	TEST_END;
}

int read_block_equals(void* a, void* b) {
	u16_t* disk_block = (u16_t*)a;
	u16_t* expected_block = (u16_t*)b;
	int i;
	
	for(i = 0; i < 256; ++i) {
		if(disk_block[i] != expected_block[i])
			return 0;
	} 
	
	return 1;
}

int super_block_equals(void* a, void* b) {
	struct super_block* sba = (struct super_block*)a;
	struct super_block* sbb = (struct super_block*)b;
	
	if(sba->s_ninodes != sbb->s_ninodes ||
	   sba->s_nzones  != sbb->s_nzones  ||
	   sba->s_imap_blocks != sbb->s_imap_blocks ||
	   sba->s_zmap_blocks != sbb->s_zmap_blocks ||
	   sba->s_firstdatazone != sbb->s_firstdatazone ||
	   sba->s_log_zone_size != sbb->s_log_zone_size ||
	   sba->s_pad != sbb->s_pad ||
	   sba->s_max_size != sbb->s_max_size || 
	   sba->s_zones != sbb->s_zones ||
	   sba->s_magic != sbb->s_magic) {
	 	return 0;  
	 } else {
	 	return 1;
	 }
}

int path_equals(void* a, void* b) {
	char* patha = (char*)a;
	char* pathb = (char*)b;
	
	while(*patha != '\0' && *pathb != '\0' && *patha++ == *pathb++) ;
	if(*patha == '\0' && *pathb == '\0')
		return 1;
	else
		return 0;
}

void test_ata() {
	u16_t expected_data[256];
	u16_t on_disk_data[256];
	int i;
	
	for(i = 0; i < 256; ++i) {
		expected_data[i] = i % 256;
	}

	device_open(ATADEV_MAJOR);

	device_lseek(ATADEV_MAJOR, 32);
	int success = device_write(ATADEV_MAJOR, expected_data, 512);
	
	device_lseek(ATADEV_MAJOR, 32);
	device_read(ATADEV_MAJOR, on_disk_data, 512);

	device_close(ATADEV_MAJOR);
	
	assert_equals(read_block_equals, (void*)on_disk_data, (void*)expected_data, 
	"Write a block to the disk then read this block and verify they are equal.");
}

void test_superblock_handling() {
	short magic = 0x7080;
	
	/* The expected super block struct	*/
	struct super_block minix_super_block;
	minix_super_block.s_ninodes = 128;
	minix_super_block.s_nzones  = 360;
	minix_super_block.s_imap_blocks = 1;
	minix_super_block.s_zmap_blocks = 1;
	minix_super_block.s_firstdatazone = 12;
	minix_super_block.s_log_zone_size = 0;
	minix_super_block.s_pad = 0;
	minix_super_block.s_max_size = 64*1024*1024;
	minix_super_block.s_zones = 360-12;
	minix_super_block.s_magic = magic;

	struct super_block* on_disk_super = read_super_block((dev_t)11);
	assert_not_null((void*)on_disk_super, "Test superblock table is empty or not initially.");
	
	assert_equals(short_equals, (void*)&magic, (void*)&(on_disk_super->s_magic),
	"Magic number in the superblock MUST BE 0x7080");
	
	assert_equals(super_block_equals, (void*)&minix_super_block, (void*)on_disk_super,
	"Read in the superblock, and check equals to expected.");
}

void test_path_operations() {
	char obtained[MAX_PATH_LENGTH];

	char* fullpath = "/usr/src/ownix";
	char* expected = "usr";
	char* fullpath_nodir = "ramdisk";
	char* expected_nodir = "ramdisk";
	char obtained_nodir[MAX_PATH_LENGTH];
	char* expected_skipped_path = "/src/ownix";
	char* obtained_skipped_path;
	
	int obtained_nodir_ret;
	int expected_nodir_ret = ENODIR;
	int expected_ret = 0;
	int obtained_ret;
	
	obtained_ret = get_next_dir_in_path(fullpath, obtained);
	assert_equals(int_equals, (void*)&expected_ret, (void*)&obtained_ret,
	"Return value must be equal to 0.");
	assert_equals(path_equals, (void*)expected, (void*)obtained, 
	"next dir name must be usr.");
	
	obtained_nodir_ret = get_next_dir_in_path(fullpath_nodir, obtained_nodir);
	assert_equals(int_equals, (void*)&expected_nodir_ret, 
	(void*)&obtained_nodir_ret, "Return value must be -1.");
	assert_equals(path_equals, (void*)expected_nodir, (void*)obtained_nodir,
	"next dir name must be ramdisk.");
	
	obtained_skipped_path = skip_chars_while_equals(fullpath, expected);
	assert_equals(path_equals, (void*)obtained_skipped_path,
	(void*)expected_skipped_path, "Test skip_char_while_equals.");
}

/*void test_inode_handling() {*/
/*	mode_t expected_mode = S_IFBLK;*/
/*	mode_t obtained_mode;*/
/*	*/
/*	char* one = "/usr";*/
/*	char* two = "/usr";*/
/*	*/
/*	int expected = 0;*/
/*	int obtained = strcmp(one, two);*/
/*	*/
/*	assert_equals(int_equals, (void*)&expected, (void*)&obtained,*/
/*	"strcmp test..");*/
/*	*/
/*	struct core_inode* cip = read_inode("/dev/ramdisk");*/
/*	*/
/*	assert_not_null((void*)cip, "/dev/ramdisk inode must exist.");*/
/*	obtained_mode = cip->inode.i_mode;*/
/*	assert_equals(mode_t_equals, (void*)&expected_mode, (void*)&obtained_mode,*/
/*	"/dev/ramdisk must be a BLOCK DEVICE inode. ");	*/
/*}*/

void test_get_time() {
	struct time t;
	int obtained, expected;
	
	expected = 0;
	
	obtained = device_read(15, (void*)&t, sizeof(struct time));
	assert_equals(int_equals, (void*)&expected, (void*)&obtained,
	"Get the current time. Must return with 0.");
}

/*void test_fd_handling() {*/
/*	int fd;*/
/*	struct file_descriptor* fdp = find_and_reserve_file_descriptor(&fd);*/
/*	assert_not_null((void*)fdp, "file descriptor must be not null.");*/
/*	free_file_descriptor(fd);*/
/*}*/

/*void test_file_open() {*/
/*	int expected = 1;*/
/*	int obtained;*/
/*	*/
/*	int fd = file_open("/dev/ramdisk");*/
/*	struct file_descriptor* fdp = find_by_index(fd);*/
/*	assert_not_null((void*)fdp, "file descriptor for /dev/ramdisk must be not null.");*/
/*	obtained = fdp->file_count;*/
/*	assert_equals(int_equals, (void*)&expected, (void*)&obtained, */
/*	"Freshly opened file must have its file_count = 1.");*/
/*	*/
/*	free_file_descriptor(fd);*/
/*}*/

void test_kheap_functions() {
	int i;
	
	init_heap((char*)0x200000, 0x1000);
	
	int* int_vector	= (int*)KMALLOC(16*sizeof(int));
	assert_not_null((void*)int_vector, "kmalloc test : alloc 64 bytes from the heap.");
	
	/*for(i = 0; i < 16; ++i) 
		int_vector[i] = i + 1;	
	for(i = 0; i < 16; ++i)
		printf("int_vector[%d]\t=\t%d\n", i, int_vector[i]);
	*/	
	
	int success = add_to_heap((char*)(0x200000 + 0x1000), 0x1000);
	assert_zero(success, "add_to_heap test : add another 4K to the heap.");
	
	KFREE((void*)int_vector);
}

void test_system_timer() {
	u32_t i;

	/* Set the system timer frequency to 1KHz = 1000Hz.
	 * Which means timer will tick in every 
	 * millisecond(1/1000 second).
	 */
	system_timer_setup(1000);
	
	system_timer_start();
	
	system_timer_start_counter();
	printf("We will wait for 20 millisecond...\n");
	system_timer_millisleep(20);
	printf("2 second time elapsed!\n");	
	
	u32_t elapsed = system_timer_get_counter_and_stop();
	printf("Elapsed time in milliseconds is : %d ms. \n", elapsed);
	
	for(i = 0; i < 100; ++i) {
		printf("%d\n", i);
		system_timer_millisleep(50);
	}
	
	char* text = "This is a simple text. This is a simple text. This is a simple text. This is a simple text. This is a simple text. This is a simple text. This is a simple text";
	
	char* s = text;
	while(*s != '\0') {
		putchar(*s);
		++s;
		system_timer_millisleep(50);
	}
	
	system_timer_stop();
	lock();
}

void test_screendev() {
	char* text = "Hello from screendev!\n";	
	device_write(12, (void*)text, strlen(text));
}

extern u32_t first_set_bit(u32_t bitmap); 

void test_bittest() {
	u32_t source = 1 << 16;
	u32_t set_bit = first_set_bit(source);
	printf("The first set bit index is %d.\n", set_bit);
	source = 0;
	set_bit = first_set_bit(source);
	printf("The first set bit index is %d.\n", set_bit);
}

static void switch_pgd(u32_t pdaddr)
{
   asm volatile("mov %0, %%cr3":: "r"(pdaddr));
   u32_t cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; // Enable paging!
   asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void test_vm() {
	u32_t vaddr, paddr;
	pde_t* pgd = create_vas();
	pte_t* pte;
	
	for(vaddr = 0; vaddr < 4 * 1024 * 1024; vaddr += 4096) {
		paddr = virt_to_phys(pgd, vaddr);		
		printf("\tvaddr = %d\tpaddr = %d\t\n", vaddr, paddr);		
	}
	
	pte = PG_FA(pgd[0]);
	if(PG_PRES(pgd[0])) {
		printf("first pt is PRESENT!\n");
	} else {
		printf("first pt is NOT PRESENT!\n");
	}
	
	if(PG_RW(pgd[0])) {
		printf("first pt is RW!\n");
	} else {
		printf("first pt is READ-ONLY!\n");
	}
	
	if(PG_US(pgd[0])) {
		printf("first pt is USER \n");		
	} else {
		printf("first pt is SUPERVISOR \n");		
	}
	
	printf("first pte address = %d\n", (u32_t)pte);
	
	if(PG_PRES(pgd[1])) {
		printf("second pt is PRESENT!\n");
	} else {
		printf("second pt is NOT PRESENT!\n");
	}
	
	if(PG_PRES(pte[0])) {
		printf("first page is PRESENT!\n");
	} else {
		printf("first page is NOT PRESENT!\n");
	}
	
	if(PG_RW(pte[0])) {
		printf("first page is RW!\n");
	} else {
		printf("first page is READ-ONLY!\n");
	}
	
	if(PG_US(pte[0])) {
		printf("first page is USER \n");		
	} else {
		printf("first page is SUPERVISOR \n");		
	}
	
	switch_pgd((u32_t)pgd);
	printf("Paging is enabled now!\n");
	
	__asm__ __volatile__(".intel_syntax noprefix\n\t"
								"mov eax, eax\n\t"
								".att_syntax \n\t"
	);
}

int testmain() {
	print_test_header();
	test_vm();	
/* Test cases we want to run	*/
/*	test_ata();	*/
/*	test_superblock_handling();*/
/*	test_path_operations();*/
/*	test_inode_handling();*/
/*	test_get_time();*/
/*	test_fd_handling();*/
/*	test_file_open();*/
/*	test_kheap_functions();*/
/*	test_screendev();*/
/*	test_bittest();*/
	//test_system_timer();
	
	print_test_summary();
	print_finish_time();
	return 0;
}

