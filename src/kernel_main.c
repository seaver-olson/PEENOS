#include "rprintf.h"
#include "page.h"
#include "mmu.h"
#include "delays.h"
#include "fat.h"
#include "systimer.h"
#include "interrupt.h"

extern unsigned char __bss_end;
extern unsigned char __bss_start;

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

void kernel_main() {
    logo();
    clear_bss();
    esp_printf(putc, "Current EL: %d\n", getEL());
    //unsigned char writeBuffer[512];
    //unsigned char readBuffer[512];
    // Initialize timer and enable IRQ
    timer_setup(1);
    success("TIMER SETUP");
    interrupt_setup();
    asm("msr DAIFClr, #2");
    success("IRQ SETUP");
    esp_printf(putc,"Freq: %u\n",read_timer_freq());
    init_pfa_list();
    struct ppage *allocd_list = allocate_physical_pages(10);
    free_physical_pages(allocd_list);
    success("Page list set up");
    if (setupIdentityMap()!=0){
	    fail("[ERROR] MMU INIT FAILED");
	    return;
    }
    success("MMU INITIALIZED\n");

    /* Initialize FAT filesystem

    if (fatInit() != 0) {
        fail("[ERROR] FAT INIT FAILED");
        return;
    }
    success("FAT SYSTEM INITIALIZED\n");
    
    // Test SD card write and read
    memcpy(writeBuffer, "Test data for SD card write", strlen("Test data for SD card write"));
    success("prepping for write...");
    if (sd_writeblock(writeBuffer, 0, 1) != 0) {
        fail("[ERROR] SD WRITE FAILED");
        return;
    }
    success("SD CARD WRITE SUCCESSFUL\n");

    if (sd_readblock(0, readBuffer, 1) != 0) {
        fail("[ERROR] SD READ FAILED");
        return;
    }
    success("SD CARD READ SUCCESSFUL\n");
    */

    while (1) {
        esp_printf(putc, "PEENOS 8==> ");
	wait_cycles(10000);
        //esp_printf(putc, "%c\n", getc());
        // readLine(buffer); // Uncomment if used.
    }
    esp_printf(putc,"terminating");
}
