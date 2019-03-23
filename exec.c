#include <exec.h>
#include <ph/contexts.h>
#include <printf.h>

int load_exec(char* path) {
	struct file file;
	Elf32_Ehdr ehdr;
	int fd, i;
	Elf32_Phdr* phdr;
	/* program header table offset, number of program header */
	int phtoff, phnum, phentsz;
	u32_t flags;
	
	struct proc_context* p = create_proc("ex");
	
	if(open(path, OMODE_RD, &file) != 0) {
		printf("cannot open file %s\n", path) ;
		return -1;
	}	  

	int nbytes = read(&file, (void*)&ehdr, sizeof(Elf32_Ehdr));
	if(ehdr.e_ident[EI_MAG0] != (unsigned char)ELFMAG0 ||
		ehdr.e_ident[EI_MAG1] != (unsigned char)ELFMAG1 ||
		ehdr.e_ident[EI_MAG2] != (unsigned char)ELFMAG2 ||
		ehdr.e_ident[EI_MAG3] != (unsigned char)ELFMAG3) {
			return -1;
	}
	
	p->ec.user_context->eip = ehdr.e_entry;	

	/* Read the program header table */
	phtoff = ehdr.e_phoff;
	phnum = ehdr.e_phnum;
	phentsz = ehdr.e_phentsize;

	phdr = (Elf32_Phdr*)KMALLOC(phnum * phentsz);
	lseek(&file, (off_t)phtoff);
	read(&file, (void*)phdr, phnum*phentsz);

	/* Create the memory mappings */
	for(i = 0; i < phnum; ++i) {
		if(phdr[i].p_type == PT_LOAD) {
			if(phdr[i].p_flags & FLAG_W)
				flags |= FLAG_RW;
				
			create_file_map(&p->vm, file, phdr[i].p_vaddr, phdr[i].p_offset, phdr[i].p_filesz, flags);			
		}
	}
	
	/* init file table */
	p->ftab.fbmp = 0xff;
	for(i = 0; i < PROC_OPEN_MAX; ++i) {
		p->ftab.ofile[i].fdp = NULL_FILED;
	}
	
	mark_ready_sched(p->pid);
	
	return 0;
}

