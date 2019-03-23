////////////////////////////////////////////
/* OwniX Operating system
 * Kenez Morotz
 * 2011
 *
 * This module is responsible for setup,
 * manage, and handle the system timer.
 * System timer is a hardware element,
 * it is a PIT(Programmable Interval Timer). 
 *
 * TODO : Add some more sophisticated
 * sleep methods with read the current
 * counter value of the pit("the latch").
 */
////////////////////////////////////////////
#include <system_timer.h>

static u8_t ticking = 0;
static u32_t counter = 0;
static u8_t counting = 0;
static u32_t timer_frequency = 0;

static void system_timer_handle_interrupt(intr_stackframe_t stackframe);

void system_timer_setup(u32_t frequency) {	
	/* Disable timer irq line	*/
	disable_irq(TMR_IRQ);
		
	timer_frequency = frequency;
	u32_t freq_div = SYSTEM_TIMER_BASE_FREQ / frequency;
	
	/* Initialize the system timer to square wawe-mode */	
	outportb(SYSTEM_TIMER_PORT_COMMAND, 0x36);

	/* Send the frequency divisor value */
	outportb(SYSTEM_TIMER_PORT_DATA, freq_div & 0xff);
	outportb(SYSTEM_TIMER_PORT_DATA, (freq_div >> 8) & 0xff);
	
	register_intr_handler(TIMER_INT_NUM, system_timer_handle_interrupt);
}

void system_timer_start() {
	enable_irq(TMR_IRQ);
}

void system_timer_stop() {
	disable_irq(TMR_IRQ);
}

void system_timer_handle_interrupt(intr_stackframe_t stackframe) {
	ticking = 1;
	
	if(counting)
		++counter;
		
	sched_on_tick();	
}

/* Suppose we set the frequency to 1000Hz!	*/
void system_timer_millisleep(u32_t milliseconds) {
	if(system_timer_get_frequency() % 1000 != 0)
		return;

	u32_t millis_remain = milliseconds;
	
	while(millis_remain > 0) {
		ticking = 0;
		while(ticking != 1);
		--millis_remain;
	}
}

void system_timer_start_counter() {
	counter = 0;
	counting = 1;
}

u32_t system_timer_get_counter_and_stop() {
	u32_t counter_value = counter;
	
	counter = 0;
	counting = 0;
		
	return counter_value;
}

u32_t system_timer_get_frequency() {
	return timer_frequency;
}

