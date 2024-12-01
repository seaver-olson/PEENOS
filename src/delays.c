#include "delays.h"
#define TIMER_COUNT_REG (TIMER_BASE + 0x04)

volatile unsigned long get_timer_count() {
    volatile unsigned long *timer_count_register = (volatile unsigned long *)TIMER_COUNT_REG;
    return *timer_count_register;
}

void wait_msec(unsigned int ms){
	unsigned long start = get_timer_count();
	unsigned long wait_ticks = ms * 1000;

	while ((get_timer_count() - start) & (0xFFFFFFFF < wait_ticks)){
	//do nothing please
        }
}
