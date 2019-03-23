#ifndef SYS_TYPES_H
#define SYS_TYPES_H

typedef unsigned char 		u8_t;
typedef char	  		s8_t;

typedef unsigned short 		u16_t;
typedef short 			s16_t;

typedef unsigned long 		u32_t;
typedef long			s32_t;

typedef unsigned long long	u64_t;
typedef long long 		s64_t;

typedef u32_t 			size_t;
typedef signed long 		ssize_t;

/* POSIX type definitions	*/
typedef s32_t			blkcnt_t;
typedef u32_t 			blksize_t;
typedef u64_t			clock_t;
typedef u16_t			clockid_t;
typedef u32_t			dev_t;
typedef u32_t			fsblkcnt_t;
typedef u32_t			fsfilcnt_t;

/*typedef u16_t			gid_t;*/
typedef unsigned int		gid_t;

typedef u32_t			id_t;
typedef u32_t			ino_t;
/*typedef u32_t 			mode_t;*/
typedef unsigned int		mode_t;
/*typedef u32_t			zone_t;*/

/*typedef u16_t			nlink_t;*/
typedef unsigned int		nlink_t;


/*typedef s32_t			off_t;*/
typedef long int		off_t;

typedef id_t			pid_t;

/*typedef u8_t			time_t;*/
typedef long int		time_t;

/*typedef u16_t			uid_t;*/
typedef unsigned int		uid_t;

#define BIT_T(name)		unsigned long name : 1
#define BITS_T(name, bitcount)	unsigned long name : bitcount

struct time {
	u8_t hour;
	u8_t minute;
	u8_t second;
	u8_t day;
	u8_t month;
	u32_t year;
};

#endif
