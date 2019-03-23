#include <fs/superblock.h>

struct super_block_entry super_block_table[NR_SUPER_BLOCK];

static int find_free_superblock_index() {
	int i;

	for(i = 0; i < NR_SUPER_BLOCK; ++i) {
		if(super_block_table[i].entry_is_free == 1)
			return i;
	}
	
	return ENOSUPER;
}

void init_super_block_table() {
	int i;
	
	/* Mark all superblock entry as free	*/
	for(i = 0; i < NR_SUPER_BLOCK; ++i)
		super_block_table[i].entry_is_free = 1;
}

/* Read the superblock from device dev.
 * Normally return the superblock which is already stored in the table.
 * When table is full, return with NULL_SUPER_BLOCK!
 */	
struct super_block* read_super_block(dev_t dev) {
	int table_index;
	
	table_index = find_free_superblock_index();
	if(table_index == ENOSUPER)
		return NULL_SUPER_BLOCK;
		
	super_block_table[table_index].entry_is_free = 0;
	
	/* Superblock resides at offset 1K	*/
	device_lseek(dev, 1024);
	
	device_read(dev, (void*)&(super_block_table[table_index].sb), sizeof(struct super_block));
	
	return &(super_block_table[table_index].sb);
}

void write_super_block(struct super_block sb, dev_t dev) {
	device_lseek(dev, 1024);
	
	device_write(dev, (void*)&sb, sizeof(struct super_block));
}

