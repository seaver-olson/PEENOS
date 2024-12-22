#include "delays.h"


void wait_cycles(unsigned int cycles){
	if (cycles){
		while (cycles--){
			asm volatile("nop");//nothing i guess(???)
		}
	}
}

uint32_t read_timer_freq() {
    uint32_t freq;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));
    return freq;
}

unsigned long get_timer_count() {
	unsigned long *timer_count_register = (unsigned long *)0x3f003004;
  	return *timer_count_register;
}
void wait_msec(unsigned int ms) {
   unsigned long start_time = get_timer_count();
   unsigned long target_time = start_time + ms;
   esp_printf(putc, "start: %lu", start_time);
   esp_printf(putc, "end: %lu", target_time);
   while ((unsigned long)(get_timer_count() - start_time) < ms){
	esp_printf(putc, "Current: %lu / Target: %lu\n", get_timer_count(), target_time);
   }
   esp_printf(putc, "%u : %u",start_time, target_time);

}



