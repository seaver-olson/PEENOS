#include "systimer.h"

void writeVirtualCounterTval(uint32_t val){
	asm volatile ("msr cntv_tval_el0, %0" :: "r" (val));
}

uint32_t readCounterFreq(void){
	uint32_t val;
	asm volatile ("mrs %0, cntfrq_el0" : "=r" (val));
	return val;
}

void enableVirtualCounter(void) {
	asm volatile("mov x1,#1\r\n" "msr cntv_ctl_el0, x1" ::: "x1" );
}

static uint32_t s_tickInterval = MPS;
static uint64_t s_counterFreq = 0;
int timer_init(int ms_interval){
	s_counterFreq = readCounterFreq();
	s_tickInterval = (s_counterFreq * ms_interval) / MPS;
	writeVirtualCounterTval(s_tickInterval);
	enableVirtualCounter();
	asm volatile(
        "msr cntv_tval_el0, %0    \n\t"	 // Initialize EL1 virtual timer value
        ""
        "mov x1, #1               \n\t"
        "msr cntv_ctl_el0, x1     \n\t"	 // Enable virtual timer
        :
        : "r"(s_tickInterval)
        : "x1");
	*CORE0_TIMER_IRQCNTL = CNTVIRQ_CTL;
	return 0;
}

