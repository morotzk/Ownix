#ifndef FS_CONST_H
#define FS_CONST_H

/* Note! This are octal numbers! These const are mode_t consts!	*/

/* Symbolic link	*/
#define S_IFLNK		0120000

/* Regular file		*/
#define S_IFREG		0100000

/* Block-special file	*/
#define S_IFBLK		0060000

/* Directory		*/
#define S_IFDIR		0040000

/* Character-special file	*/
#define S_IFCHR		0020000

/* root dev is ramdisk	*/
#define ROOT_DEV_MAJOR	11	

/* fs deal with standard 512 byte sectors which is the default sector size on most block device	*/
#define SECTOR_SIZE	512

/* Logical block size is 1024 byte, which means we handle 2 sectors as one block	*/
#define BLOCK_SIZE	1024

typedef unsigned long zone_t;

/* Our default file system is MiniX filesystem a little bit modified	
 * Below are the normal minix fs disk layout offsets.
*/
#define V2_NR_TZONES		9

#define MAGIC			0x7080

#define SUPER_OFF		1*BLOCK_SIZE
#define IBMP_OFF		2*BLOCK_SIZE
#define ZBMP_OFF		3*BLOCK_SIZE
#define IAREA_OFF		4*BLOCK_SIZE

/* Data zones started at 12th zone	*/
#define ZONES_OFF		12*BLOCK_SIZE

/* Number of inodes system-wide used	*/
#define NR_INODES		32

/* Maximal path length include the terminating '\0'!	*/
#define MAX_PATH_LENGTH		14

#endif
