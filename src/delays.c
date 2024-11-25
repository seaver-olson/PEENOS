#include "delays.h"

unsigned long get_timer_count(){
	unsigned long *timer_count_register = (unsigned long *)0x3f003004;
	return *timer_count_register;
}

void wait_msec(unsigned int ms){
	unsigned long s = get_timer_count();
	while (get_timer_count() - s < (1000 * ms)){
		
	}
}
