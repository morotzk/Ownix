#ifndef EXEC_H
#define EXEC_H
#include <ph/process.h>
#include <mm/virt_mem.h>
#include <fs/fs.h>
#include <protect.h>
#include <kheap.h>

/* segment type constants */
#define PT_NULL		0	/* entry unused */
#define PT_LOAD		1	/* loadable segment */
#define PT_DYNAMIC	2	/* dynamic linking info. */
#define PT_INTERP		3	
#define PT_NOTE		4
#define PT_SHLIB		5
#define PT_PHDR		6
#define PT_LOPROC		7
#define PT_HIPROC		8

/* Object file type constants in e_type */
#define ET_NONE		0	/* No file type 	*/
#define ET_REL			1	/* Relocatable file 	*/
#define ET_EXEC		2	/* Executable file 	*/
#define ET_DYN			3	/* Shared object file 	*/
#define ET_CORE		4	/* Core dump file 	*/

/* e_machine constants */
#define EM_NONE		0	/* No machine 		*/
#define EM_M32			1	/* AT&T WE32100 	*/
#define EM_SPARC		2	/* SPARC		*/
#define EM_386			3	/* Intel 80386		*/
#define EM_68K			4	/* Motorola 68000	*/
#define EM_88K			5	/* Motorola 88000	*/
#define EM_860			7	/* Intel 80860		*/
#define EM_MIPS		8	/* MIPS RS3000		*/

/* e_version constants */
#define EV_NONE		0	/* Invalid version */
#define EV_CURREMT	1	/* Current version */

/* e_ident identification indexes	*/
#define EI_MAG0		0
#define EI_MAG1		1
#define EI_MAG2		2
#define EI_MAG3		3
#define EI_CLASS		4
#define EI_DATA		5
#define EI_VERSION	6
#define EI_PAD			7

/* Required ELF magic numbers		*/
#define ELFMAG0		0x7f
#define ELFMAG1		'E'
#define ELFMAG2		'L'
#define ELFMAG3		'F'

#define FLAG_E			0x1
#define FLAG_W			0x2
#define FLAG_R			0x4

#define EI_NIDENT		16
#define ELF_HDR_SIZE	sizeof(ELF32_Ehdr)

typedef unsigned long 	Elf32_Addr;
typedef unsigned short 	Elf32_Half;
typedef unsigned long 	Elf32_Off;
typedef signed long	Elf32_SWord;
typedef unsigned long	Elf32_Word;

typedef struct __Elf32_Ehdr {
	unsigned char	e_ident[EI_NIDENT];	/* identification bytes */
	Elf32_Half	e_type;			/* object file type */		
	Elf32_Half	e_machine;		/* required architecture */
	Elf32_Word	e_version;		/* object file version */
	Elf32_Addr	e_entry;		/* entry point virtal address */
	Elf32_Off	e_phoff;		/* program header table file offset */
	Elf32_Off	e_shoff;		/* section header table file offset */
	Elf32_Word	e_flags;		/* processor-specific flags */
	Elf32_Half	e_ehsize;		/* elf header size in bytes */
	Elf32_Half	e_phentsize;		/* program header table entry size in bytes */
	Elf32_Half	e_phnum;		/* number of entries in the program header table */
	Elf32_Half	e_shentsize;		/* section header entry size in bytes */
	Elf32_Half	e_shnum;		/* number of entries in the section header table */
	Elf32_Half	e_shstrndx;		
} Elf32_Ehdr;

/* Program header structure */
typedef struct {
	Elf32_Word	p_type;			/* segment type */
	Elf32_Off	p_offset;		/* offset in the file */
	Elf32_Addr	p_vaddr;			/* virtual base address of the segment */
	Elf32_Addr	p_paddr;		
	Elf32_Word	p_filesz;		/* segment size in the file in bytes */	
	Elf32_Word	p_memsz;			/* segment size in memory in bytes(>f ==> pad 0) */
	Elf32_Word	p_flags;			/* segment flags */
	Elf32_Word	p_align;			/* segment alignment */
} Elf32_Phdr;

/* read an elf executable and prepare to running it */
int load_exec(char* path);

#endif
