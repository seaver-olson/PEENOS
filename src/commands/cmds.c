#include "cmds.h"
#include "rprintf.h"

void clear_bss() {
    unsigned char *begin_bss = &__bss_start;
    unsigned char *end_bss = &__bss_end;
    if (begin_bss >= end_bss) {
        esp_printf(putc, "Invalid BSS range, aborting clear.");
        return;
    }
    esp_printf(putc, "Attempting to clear BSS section from %u to %u...\n", begin_bss, end_bss);
    while (begin_bss < end_bss) {*begin_bss++ = 0;}
    esp_printf(putc, "finished clearing bss\n");	
}


void hexdump(unsigned char *buffer, unsigned int length) {
    for (int i = 0; i < length; i += 16) {
        esp_printf(putc, "(%08x) : ", buffer + i);
        for (int j = 0; j < 16; j++) {
            if (j + i >= length) {
                esp_printf(putc, "   ");
            } else {
                esp_printf(putc, "%02x ", buffer[i + j]);
            }
        }
        esp_printf(putc, "\n");
    }
    esp_printf(putc, "\n");
}

unsigned int getEL() {
    unsigned int el;
    asm volatile("mrs %0, CurrentEL" : "=r"(el));  // Read CurrentEL register
    return (el >> 2) & 0x3;  // Extract EL from bits [3:2]
}