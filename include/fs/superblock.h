#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H
#include <sys/types.h>
#include <fs/fs_const.h>

struct super_block {
	ino_t s_ninodes;
	zone_t  s_nzones;
	short s_imap_blocks;
	short s_zmap_blocks;
	zone_t s_firstdatazone;
	short s_log_zone_size;
	short s_pad;
	off_t s_max_size;
	zone_t s_zones;
	short s_magic;	
};

struct super_block_entry {
	struct super_block sb;
	BIT_T(entry_is_free);
};

/* We allow max. 8 super block store in the super block table	*/
#define NR_SUPER_BLOCK			8

#define NULL_SUPER_BLOCK		(struct super_block*)0

/* Return with this value means the superblock table is full!	*/
#define ENOSUPER			-1

void init_super_block_table();
struct super_block* read_super_block(dev_t dev);
void write_super_block(struct super_block sb, dev_t dev);

#endif
