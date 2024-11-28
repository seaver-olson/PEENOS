#include "systimer.h"

void writeVirtualCounterTval(uint32_t val) {
	asm volatile ("msr cntv_tval_el0, %0" :: "r" (val));
	return;
}

uint32_t readCounterFrequency(void) {
	uint32_t val;
	asm volatile ("mrs %0, cntfrq_el0" : "=r" (val));
	return val;
}
#if 0
void init_timer(void) {
	timer_cntfrq = timer_tick = read_cntfrq();
	write_cntv_tval(timer_cntfrq);    // clear cntv interrupt and set next 1 sec timer.
	return;
}
#endif

void enableVirtualCounter(void) {
	asm volatile ( "mov x1,#1\r\n"
                 "msr cntv_ctl_el0, x1" ::: "x1" );
}

static uint32_t s_tickInterval = 1000;
static uint64_t s_counterFrequency = 0;
void timer_setup(int ms_interval) {

  s_counterFrequency = readCounterFrequency();

  s_tickInterval = (s_counterFrequency * ms_interval) / MILLISECONDS_IN_SECONDS;
  writeVirtualCounterTval(s_tickInterval);
  enableVirtualCounter();
#if 0
    asm volatile(
        "msr cntv_tval_el0, %0    \n\t"	 // Initialize EL1 virtual timer value
        ""
        "mov x1, #1               \n\t"
        "msr cntv_ctl_el0, x1     \n\t"	 // Enable virtual timer
        :
        : "r"(s_tickInterval)
        : "x1");
#endif

    *CORE0_TIMER_IRQCNTL = CNTVIRQ_CTL;
}
