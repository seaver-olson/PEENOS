#include "rprintf.h"
#include "interrupt.h"

extern void Vector_table_el1();

void interrupt_setup(){
	asm("msr vbar_el1, %0"
        :
        : "r" (Vector_table_el1)
        :);
}

void handle_timer(){
	esp_printf(putc, "Timer interrupt handled, next scheduled at: %u\n", read_timer_freq());
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
