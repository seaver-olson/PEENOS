#include "rprintf.h"
#include "interrupt.h"

extern void Vector_table_el1();

void interrupt_setup(){
	asm("msr vbar_el1, %0"
        :
        : "r" (Vector_table_el1)
        :); //writes address of VTel1 to the VBAR_EL1
}

void handle_timer(){
	*TIMER_IRQ_CLEAR = 0x2;
	//set next timer interrupt
        uint32_t next_interrupt = *TIMER__LOW + 1000000;
	*TIMER_COMPARE1 = next_interrupt;
	esp_printf(putc, "Timer interrupt handled, next scheduled at: %u\n", next_interrupt);
}
void handle_irq(){
	switch(*CORE0_IRQ_SOURCE){
		case 8:
			handle_timer();
			break;
		case 16:
			//mailbox interrupt
			break;
		default:
			esp_printf(putc,"UNhandled IRQ source: %d\n", *CORE0_IRQ_SOURCE);
			break;
	}
}
