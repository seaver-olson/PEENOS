
#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

#define MILLISECONDS_IN_SECONDS 1000
#define CORE0_TIMER_IRQCNTL ((volatile uint32_t*)0x40000040)
#define CNTVIRQ_CTL (1 << 3)


void timer_setup(int ms_interval);

void writeVirtualCounterTval(uint32_t val);    // Writes to the virtual timer's target value register
uint32_t readCounterFrequency(void);            // Reads the frequency of the timer
void enableVirtualCounter(void); 
#endif
