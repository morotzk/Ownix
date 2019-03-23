#include <sys/types.h>
#include <protect.h>
#include <port_io.h>
#include <ph/sched.h>

#define SYSTEM_TIMER_BASE_FREQ		1193180	/* Base frequency of the system timer in Hz */
#define SYSTEM_TIMER_PORT_COMMAND	0x43	/* Command port address of the system timer */
#define SYSTEM_TIMER_PORT_DATA		0x40    /* Data port address of the system timer */

void system_timer_setup(u32_t frequency);
void system_timer_start();
void system_timer_stop();
void system_timer_millisleep(u32_t milliseconds);
void system_timer_start_counter();
u32_t system_timer_get_counter_and_stop();
u32_t system_timer_get_frequency();
