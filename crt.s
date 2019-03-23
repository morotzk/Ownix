section .text

global unified_syscall
global unified_syscall0

SYSCALL_INT equ 0x32

;
; do a system call which has return value
;
unified_syscall:
	push ebx
	push ecx
	push edx
	
	mov edx, dword [esp+28]
	mov ecx, dword [esp+24]
	mov ebx, dword [esp+20] 
	mov eax, dword [esp+16]
	
	int SYSCALL_INT 
	
	pop edx		
	pop ecx
	pop ebx
		
ret

;
; do a system call which hasn't return value
;
unified_syscall0:
	push eax
	push ebx
	push ecx
	push edx
	
	mov edx, dword [esp+32]
	mov ecx, dword [esp+28]
	mov ebx, dword [esp+24]
	mov eax, dword [esp+20]
	
	int SYSCALL_INT
	
	pop edx
	pop ecx
	pop ebx
	pop eax
ret
