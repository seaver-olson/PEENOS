

#define CORE0_IRQ_SOURCE ((uint32_t*)0x40000060)
#define CORE1_IRQ_SOURCE ((uint32_t*)0x40000064)
#define CORE2_IRQ_SOURCE ((uint32_t*)0x40000068)
#define CORE3_IRQ_SOURCE ((uint32_t*)0x4000006C)


void interrupt_setup();
