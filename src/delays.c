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

volatile unsigned long get_timer_count() {
    unsigned int h, l;
    h = *TIMER_HIGH;
    l = *TIMER_LOW;
    // Ensure that the high register hasn't changed between the reads.
    if (h != *TIMER_HIGH) {
        h = *TIMER_HIGH;
        l = *TIMER_LOW;
	warning("[Warning] high register changed between timer readings\n");
    }
    return ((unsigned long)h << 32) | l;
}
void wait_msec(unsigned int ms) {
    unsigned long start_time = get_timer_count();
    unsigned long target_time = start_time + ms;
    esp_printf(putc, "start: %u", start_time);
    esp_printf(putc, "end: %u", target_time);
    while(start_time < target_time){
	//esp_printf(putc, "%u : %u",start_time, target_time);
	start_time=get_timer_count();
   	esp_printf(putc, "start now: %u", start_time); 
   }
   esp_printf(putc, "%u : %u",start_time, target_time);

}



