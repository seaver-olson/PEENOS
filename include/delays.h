#include "gpio.h"
#include "rprintf.h"

volatile unsigned long get_timer_count();
void wait_msec(unsigned int ms);
void wait_cycles(unsigned int ms);
uint32_t read_timer_freq();
