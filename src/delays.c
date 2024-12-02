#include "delays.h"


void wait_cycles(unsigned int cycles){
	if (cycles){
		while (cycles--){
			asm volatile("nop");//nothing i guess(???)
		}
	}
}

volatile unsigned long get_timer_count() {
    unsigned int h=-1,l;//high and low registers
	h=*TIMER_HIGH;
	l=*TIMER_LOW;
	if (h!=*TIMER_HIGH){
		h=*TIMER_HIGH;
		l=*TIMER_LOW;
	}
	return ((unsigned long)h<<32)|l;
}



void wait_msec(unsigned int ms){
	unsigned long t= get_timer_count();
	if (t){
		while (get_timer_count() -t<ms){
			asm volatile("nop");
		}
	}
}
