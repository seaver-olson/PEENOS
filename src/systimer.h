#include "rprintf.h"

#define MPS 1000
#define CORE0_TIMER_IRQCNTL ((uint32_t*)(0x40000040))
#define CNTVIRQ_CTL (1 << 3)

void timer_init(int ms_interval);
