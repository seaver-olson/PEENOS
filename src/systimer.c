#include "systimer.h"
#include "rprintf.h"

void writeVirtualCounterTval(uint32_t val) {
	asm volatile ("msr cntv_tval_el0, %0" :: "r" (val));
	return;
}

uint32_t readCounterFrequency(void) {
	uint32_t val;
	asm volatile ("mrs %0, cntfrq_el0" : "=r" (val));
	return val;
}

void enableVirtualCounter(void) {
	unsigned int val;
	asm volatile ("msr cntv_ctl_el0, %0" :: "r"(1)); // Enable the timer

    	asm volatile ("mrs %0, cntv_ctl_el0" : "=r" (val));
	esp_printf(putc, "CNTV_CTL_EL0: %u\n", val);
}

static uint32_t s_tickInterval = 1000;
static uint64_t s_counterFrequency = 0;
void timer_setup(int ms_interval) {

  s_counterFrequency = readCounterFrequency();

  s_tickInterval = (s_counterFrequency * ms_interval) / MILLISECONDS_IN_SECONDS;
  esp_printf(putc, "Timer setup: s_counterFrequency = %u, s_tickInterval = %u\n", s_counterFrequency, s_tickInterval);
  writeVirtualCounterTval(s_tickInterval);
  enableVirtualCounter();
  *CORE0_TIMER_IRQCNTL = CNTVIRQ_CTL;

}
