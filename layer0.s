;-----------------------------------------------;
;	This is the lowest layer of the					;
;	OwniX operating system.								;
;																;
;	It includes low level exception and				;
;	interrupt handlers,  GDT entry					;
;	definitions, GDT, IDT pointers					;
;	statically defined.									;
;																;
;	Also responsible for save the current			;
;	process's state in case of interrupt,			;
;	and reload a (new) process's state				;
; 	after interrupt handlers do their				;
;	work. 													;
;-----------------------------------------------;
section .text

	; protect.c
	extern set_prot_env
	extern global_intr_handler
	extern gate_tab

	; main.c
	extern main
;------------------------------------------------;
;		Constants			 								 ;
;------------------------------------------------;
KERNEL_CODE_SEL equ		0x8
KERNEL_DATA_SEL	equ		0x10
USER_CODE_SEL	equ		0x18
USER_DATA_SEL	equ		0x20	
;------------------------------------------------;
;		Macros				 								 ;
;------------------------------------------------;
; prepare stack frame for C interrupt-handler(no error code)
%macro prepare 1
	
	push dword 0
	
	;for the kernel stack field, we assign it later 
	;just before call to global_intr_handler (marked with * )!
	push dword 0
	
	push ds
	push es
	push fs
	push gs
	
	pushad
	
	push dword %1 ; int_no	
	mov dword [esp+52], esp		; *
	
	mov ax, KERNEL_DATA_SEL
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	;******************************************;
	; our very first task is to disable paging
	mov edx, cr0
	and edx, 0x7fffffff
	mov cr0, edx
	;******************************************;	
%endm

; prepare stack frame for C interrupt-handler(error-code)
%macro prepare_errcode 1
	;for the kernel stack field, we assign it later 
	;just before call to global_intr_handler!
	push dword 0

	push ds
	push es
	push fs
	push gs
	
	pushad
	
	push dword %1
	
	mov dword [esp+52], esp
	
	mov ax, KERNEL_DATA_SEL
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	;******************************************;
	; our very first task is to disable paging
	mov edx, cr0
	and edx, 0x7fffffff
	mov cr0, edx
	;******************************************;
%endm

; restore stack frame after C-interrupt handler
%macro restore 0
	;add esp,4
	popad
	
	pop gs
	pop fs
	pop es
	pop ds
	
	add esp, 8 ; add esp, 4
%endm

%macro restore_no_errcode 0
	add esp, 4
	popad
	pop gs
	pop fs
	pop es
	pop ds
	; error code and restore_kstack_esp
	add esp, 8	
%endm

%macro restore_errcode 0
	add esp, 4
	popad
	pop gs
	pop fs
	pop es
	pop ds
	add esp, 4	
%endm

;------------------------------------------------;
;	 Low-level interrupt-handler functions	 		 ;
;	 and architecture specific init functions		 ;
;------------------------------------------------;
global divide_exception
global debugger_exception
global reserved_exception
global breakpoint_exception
global into_exception
global bounds_exception
global invalid_op_exception
global coproc_not_exception
global double_fault_exception
global coproc_seg_exception
global invalid_tss_exception
global segnot_present_exception
global stack_fault_exception
global general_protection_exception
global page_fault_exception
global num_coproc_exception
global hw_interrupt0
global hw_interrupt1
global hw_interrupt2
global hw_interrupt3
global hw_interrupt4
global hw_interrupt5
global hw_interrupt6
global hw_interrupt7
global hw_interrupt8
global hw_interrupt9
global hw_interrupt10
global hw_interrupt11
global hw_interrupt12
global hw_interrupt13
global hw_interrupt14
global hw_interrupt15
global syscall
global unused
global init_desc
global desc_tab
global gdt_ptr
global idt_ptr
global tss_descriptor
extern gdt_tab
global kernel_stack_top
global read_cr2
global register_state
global load_tss
global context_handler	
global first_set_bit

; u32_t first_set_bit(u32_t bitmap) ;
first_set_bit:
	bsf eax, [esp+4]
	jnz fin	
	mov eax, 32		
fin:
ret

;*****************************************************************************; 	
; void context_handler(u32_t restore_esp, u32_t paging, u32_t pdaddr);	      ;
;		       			  [esp+4], 	  			[esp+8],		  [esp+12]				   ;
;*****************************************************************************;
context_handler:
	; check enable paging or not
	mov ecx, [esp+8]
	cmp ecx, 0
	je __reload		
	; load page directory 	
	mov edx, [esp+12]
	mov cr3, edx
	; enable paging	
	mov edx, cr0
	or edx, 0x80000000 
	mov cr0, edx
	; restart interrupted proc
	__reload:		
	mov esp, [esp+4]		
	restore_no_errcode		
	sti
	iretd	
	
; void register_state(register_t *regs) ;
register_state:	
	push edi
	mov edi, [esp+8]
	mov [edi+4], esi
	mov [edi+8], ebp
	mov [edi+12], esp  
	mov [edi+16], ebx
	mov [edi+20], edx
	mov [edi+24], ecx
	mov [edi+28], eax
	
	pop edi
	push eax
	mov eax, [esp+8]
	mov [eax], edi
	
	pop eax
ret

read_cr2:
	mov eax, cr2
ret

load_tss:	
	xor eax, eax
	mov ax, 0x28 ; 0x2B
	ltr ax
ret	
;------------------------------------------------;
;	Set up correct GDT and IDT, and		 			 ;
;	start the kernel			 							 ;
;------------------------------------------------;
init_desc:
	cli
	
	call set_prot_env
	
	push eax
		mov eax, gdt_ptr
		lgdt [eax]
		
		jmp KERNEL_CODE_SEL:.ok
	
		.ok:	
		call load_tss		
	
		mov eax, idt_ptr
		lidt [eax]	
		
	pop eax

	mov ax, KERNEL_DATA_SEL
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov esp, kernel_stack_top

	call main	;main is the start of the kernel
	ret 

;------------------------------------------------;
;	Low-level interrupt-handlers		 ;
;	implementation				 ;
;------------------------------------------------;	
divide_exception:
	cli
	prepare 0	
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

debugger_exception:
	cli
	prepare 1
	
	call global_intr_handler
	
	restore_no_errcode
	sti	
iretd

; NMI
reserved_exception:
	cli
	prepare 2
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

breakpoint_exception:
	cli
	prepare 3
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

into_exception:
	cli
	prepare 4
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

bounds_exception:
	cli
	prepare 5
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

invalid_op_exception:
	cli
	prepare 6
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

coproc_not_exception:
	cli
	prepare 7
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

double_fault_exception:	
	cli
	
	prepare_errcode 8
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

coproc_seg_exception:
	cli
	prepare 9
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

invalid_tss_exception:	
	cli
	prepare_errcode 10
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

segnot_present_exception:	
	cli
	prepare_errcode 11
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

stack_fault_exception:	
	cli
	prepare_errcode 12
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

general_protection_exception:	
	cli
	prepare_errcode 13
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

page_fault_exception:
	cli
	prepare_errcode 14
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

num_coproc_exception:
	cli
	prepare 15
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt0:
	cli
	prepare 32	
	
	call global_intr_handler
	
	restore_no_errcode			
	sti
iretd

hw_interrupt1:
	cli
	prepare 33
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt2:
	cli
	prepare 34
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt3:
	cli
	prepare 35
	
	call global_intr_handler
	
	restore_no_errcode
	sti	
iretd

hw_interrupt4:
	cli
	prepare 36
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt5:
	cli
	prepare 37
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt6:
	cli
	prepare 38
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt7:
	cli
	prepare 39
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt8:
	cli
	prepare 40
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt9:
	cli
	prepare 41
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt10:
	cli
	prepare 42
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt11:
	cli
	prepare 43
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt12:
	cli
	prepare 44
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt13:
	cli
	prepare 45
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

; AT Winchester interrupt number
hw_interrupt14:
	cli
	prepare 46
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

hw_interrupt15:
	cli
	prepare 47
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

syscall:
	cli

	prepare 0x32
	
	call global_intr_handler
	
	restore_no_errcode
	sti
iretd

unused:
	cli
	push dword 0
	pusha
	push byte 15
	
	call global_intr_handler
	
	add esp, 1
	popa
	add esp, 4
	sti
iretd
	
section .data	
	;------------------------------------------------------;
	;		GDT, IDT pointers		       							 ;
	;------------------------------------------------------;
	gdt_ptr : dw 47 
		  dd gdt_tab
	idt_ptr : dw 2047 
		  dd gate_tab	
			  
section .bss
	resb 32764			
	kernel_stack_top:
