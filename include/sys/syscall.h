#ifndef SYSCALL_H
#define SYSCALL_H
#include <sys/types.h>

extern u32_t unified_syscall(u32_t eax, u32_t ebx, u32_t ecx, u32_t edx);

#endif
