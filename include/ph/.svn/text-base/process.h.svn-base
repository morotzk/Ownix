#ifndef PROCESS_H
#define PROCESS_H
#include <sys/types.h>
#include <ph/contexts.h>

#define PROC_POOL_SZ		32
#define NULL_PROC		(struct proc_context*)0

struct proc_tab {
	struct proc_context ppool[PROC_POOL_SZ];
	u32_t free_bmp;
};

void init_proc_handler();
struct proc_context* create_proc(char* name);
void delete_proc_context(pid_t pid);
void hold(pid_t pid, u8_t event);
void unhold(pid_t pid, u8_t event);
void mark_ready_sched(pid_t pid);
void finalize_syscall(pid_t pid, s32_t retval);
void complete_syscall();
struct proc_context* get_idle_proc();

#endif

