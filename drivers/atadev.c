#include "driver.h"
#include "atadev.h"
#include <sys/types.h>
#include <protect.h>

/* 	ATA Registers	*/
#define PRIMARY_ATA_BASE_REG		0x1F0
#define ATA_DATA_REG			PRIMARY_ATA_BASE_REG
#define ATA_ERROR_REG			PRIMARY_ATA_BASE_REG + 1
#define ATA_FEATURES_REG		PRIMARY_ATA_BASE_REG + 1
#define ATA_SECTOR_COUNT_REG		PRIMARY_ATA_BASE_REG + 2
#define ATA_LBA_LOW_REG			PRIMARY_ATA_BASE_REG + 3
#define ATA_LBA_MID_REG			PRIMARY_ATA_BASE_REG + 4
#define ATA_LBA_HIGH_REG		PRIMARY_ATA_BASE_REG + 5
#define ATA_DRIVE_HEAD_REG		PRIMARY_ATA_BASE_REG + 6
#define ATA_STATUS_REG			PRIMARY_ATA_BASE_REG + 7
#define ATA_COMMAND_REG			PRIMARY_ATA_BASE_REG + 7
#define ATA_ALTERNATE_STATUS_REG	0x3F6
#define ATA_DEVICE_CONTROL_REG		0x3F6

/* ATA Status register bits	*/
#define ATA_STATUS_BUSY			0x80
#define ATA_STATUS_DRDY			0x40
#define ATA_STATUS_DEV_FAULT		0x20
#define ATA_STATUS_SEEK_COMPLETE	0x10
#define ATA_STATUS_DRQ			0x08
#define ATA_STATUS_CORR			0x04
#define ATA_STATUS_IDX			0x02
#define ATA_STATUS_ERROR		0x01

/* ATA Error register bits	*/	                          
#define ATA_ERR_BBK			0x80
#define ATA_ERR_UNC			0x40
#define ATA_ERR_MC			0x20
#define ATA_ERR_IDNF			0x10
#define ATA_ERR_MCR			0x08
#define ATA_ERR_ABRT			0x04
#define ATA_ERR_TK0NF			0x02
#define ATA_ERR_AMNF			0x01

/* ATA Command codes		*/
#define ATA_COMMAND_NOP			0x00
#define ATA_COMMAND_RESET		0x08
#define ATA_COMMAND_READ_SECTORS	0x20
#define ATA_COMMAND_READ_DMA		0xC8
#define ATA_COMMAND_WRITE_SECTORS	0x30
#define ATA_COMMAND_WRITE_DMA		0xCA
#define ATA_COMMAND_EXEC_DIAG		0x90
#define ATA_COMMAND_IDENTIFY		0xEC
#define ATA_COMMAND_FORMAT		0x50
#define ATA_COMMAND_SEEK		0x70
#define ATA_COMMAND_RECALIBRATE		0x10

static const char* atadev_name = "ata";
static off_t seek_pos;			/* # actual block number */
static u32_t size_blocks;		/* The size of the disk in sectors */

char* get_atadev_name();
int atadev_open();
int atadev_close();
int atadev_read(void* buffer, size_t nbytes);
int atadev_write(void* buffer, size_t nbytes);
int atadev_seek(off_t position);

driver_t atadev_driver = {
	.get_driver_name = get_atadev_name,
	.driver_open = atadev_open,
	.driver_close = atadev_close,
	.driver_read = atadev_read,
	.driver_write = atadev_write,
	.driver_lseek = atadev_seek,
	.driver_flags = BLOCK_DEV | RAM_DEV	/* use PIO(Programmed IO) mode */
};

char* get_atadev_name() {
	return (char*)atadev_name;
}

int atadev_open() {
	u16_t ident_buf[256];
	ata_identify(ident_buf);
	u32_t* addr_sect = (u32_t*)&ident_buf[60];
	size_blocks = *addr_sect - 1;
	
	seek_pos = 0;	
	return 0;	
}

int atadev_close() {	
	return 0;
}

int atadev_read(void* buffer, size_t nbytes) {
	u8_t status;
	u16_t* data;
	u8_t lba_sector_address_byte;
	
	data = (u16_t*)buffer;
	
	status = inportb(ATA_STATUS_REG);
	while(status & ATA_STATUS_BUSY) {
		status = inportb(ATA_STATUS_REG);
	}
	
	lock();
	
	status = inportb(ATA_STATUS_REG);
	while(status & ATA_STATUS_DRDY != ATA_STATUS_DRDY) {
		status = inportb(ATA_STATUS_REG);
	}
	
	lba_sector_address_byte = seek_pos & 0xff;
	outportb(ATA_LBA_LOW_REG, lba_sector_address_byte);
	
	lba_sector_address_byte = (seek_pos >> 8) & 0xff;
	outportb(ATA_LBA_MID_REG, lba_sector_address_byte);
	
	lba_sector_address_byte = (seek_pos >> 16) & 0xff;
	outportb(ATA_LBA_HIGH_REG, lba_sector_address_byte);
	
	lba_sector_address_byte = (seek_pos >> 24) & 0x0f;
	outportb(ATA_DRIVE_HEAD_REG, 0xE0 | lba_sector_address_byte);

	outportb(ATA_COMMAND_REG, ATA_COMMAND_READ_SECTORS);
	
	status = inportb(ATA_STATUS_REG);
	while(status & ATA_STATUS_DRQ != ATA_STATUS_DRQ) {
		status = inportb(ATA_STATUS_REG);
	}
	
	inportw_copy(ATA_DATA_REG, data, 256);
	
	unlock();
	
	return 0;
}

int atadev_write(void* buffer, size_t nbytes) {
	u8_t status;
	u32_t lba_sector_address = 32;
	u8_t lba_sector_address_byte;
	u16_t* data;
	u32_t i;
	
	data = (u16_t*)buffer;	
	
	status = inportb(ATA_STATUS_REG);
	while(status & ATA_STATUS_BUSY) {
		status = inportb(ATA_STATUS_REG);
	}
	
	lock();
	
	status = inportb(ATA_STATUS_REG);
	while(status & ATA_STATUS_DRDY != 0) {
		status = inportb(ATA_STATUS_REG);
	}
	
	lba_sector_address_byte = lba_sector_address & 0xff;
	outportb(ATA_LBA_LOW_REG, lba_sector_address_byte);
	
	lba_sector_address_byte = (lba_sector_address >> 8) & 0xff;
	outportb(ATA_LBA_MID_REG, lba_sector_address_byte);
	
	lba_sector_address_byte = (lba_sector_address >> 16) & 0xff;
	outportb(ATA_LBA_HIGH_REG, lba_sector_address_byte);
	
	lba_sector_address_byte = (lba_sector_address >> 24) & 0x0f;
	outportb(ATA_DRIVE_HEAD_REG, 0xE0 | lba_sector_address_byte);
	
	outportb(ATA_COMMAND_REG, ATA_COMMAND_WRITE_SECTORS);
	
	status = inportb(ATA_STATUS_REG);
	while(status & ATA_STATUS_DRQ != ATA_STATUS_DRQ) {
		status = inportb(ATA_STATUS_REG);
	}
	
	outportw_copy(ATA_DATA_REG, data, 256);
		
	return 0;
}

int atadev_seek(off_t position) {
	if(seek_pos + position > size_blocks)
		return ESEEK;
		
	seek_pos = position;
	return 0;
}

int ata_identify(u16_t* buffer) {
	/* Read in the ATA Identification data which is 256 words long	*/
	u8_t status;
	
	status = inportb(ATA_STATUS_REG);
	while(status & ATA_STATUS_BUSY) {
		status = inportb(ATA_STATUS_REG);
	}
	
	lock();
	
	status = inportb(ATA_STATUS_REG);
	while(status & ATA_STATUS_DRDY != ATA_STATUS_DRDY) {
		status = inportb(ATA_STATUS_REG);
	}
	
	outportb(ATA_DRIVE_HEAD_REG, 0);
	outportb(ATA_COMMAND_REG, ATA_COMMAND_IDENTIFY);
	
	status = inportb(ATA_STATUS_REG);
	while(status & ATA_STATUS_DRQ != ATA_STATUS_DRQ) {
		status = inportb(ATA_STATUS_REG);
	}
	
	inportw_copy(ATA_DATA_REG, buffer, 256);
	
	unlock();
	return 0;
}

