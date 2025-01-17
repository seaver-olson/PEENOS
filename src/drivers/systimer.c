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

    asm volatile ("msr cntv_ctl_el0, %0" :: "r"(1));  // Enable = 1, IMASK = 0
    asm volatile ("mrs %0, cntv_ctl_el0" : "=r"(val));

    esp_printf(putc, "CNTV_CTL_EL0: 0x%u\n", val);

    // Check if ENABLE bit is set correctly (optional)
    if (val & 0x1) {
        esp_printf(putc,"Virtual counter enabled.");
    } else {
        esp_printf(putc,"Failed to enable virtual counter.");
    }
}

static uint32_t s_tickInterval = 1000;
static uint64_t s_counterFrequency = 0;
void timer_setup(int ms_interval) {

  s_counterFrequency = readCounterFrequency();
  s_tickInterval = (s_counterFrequency * ms_interval) / s_tickInterval;

  esp_printf(putc, "Timer setup: s_counterFrequency = %u, s_tickInterval = %u\n", s_counterFrequency, s_tickInterval);
  writeVirtualCounterTval(s_tickInterval);
  enableVirtualCounter();
  *CORE0_TIMER_IRQCNTL = CNTVIRQ_CTL;

}
