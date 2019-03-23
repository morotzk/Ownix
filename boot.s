;-----------------------------------------------------------------------------;
; This files original version's source is :				      ;
; http://www.osdever.net/bkerndev/Docs/basickernel.htm			      ;
; Author : Brandon Friesen						      ;
;									      ;
; I made just little modifications		      			      ;
; within this file.							      ;
;-----------------------------------------------------------------------------;
MBOOT_PAGE_ALIGN    equ 1<<0    
MBOOT_MEM_INFO      equ 1<<1    
MBOOT_HEADER_MAGIC  equ 0x1BADB002 
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]                       

[GLOBAL mboot]                  
[EXTERN code]                   
[EXTERN bss]                    
[EXTERN end]                   
[EXTERN multiboot_ptr]

mboot:
    dd  MBOOT_HEADER_MAGIC      
                                
    dd  MBOOT_HEADER_FLAGS      
    dd  MBOOT_CHECKSUM          
    
    dd  mboot                   
    dd  code                    
    dd  bss                     
    dd  end                     
    dd  start                   

[GLOBAL start]                  	; Kernel entry point
[EXTERN init_desc]              

start:   
    cli                         
	mov [multiboot_ptr], ebx 	; store the GRUB multiboot info in memory
	
	mov ax, 0x10
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
    	call init_desc          

     	hang:     
		jmp hang                       
                                
