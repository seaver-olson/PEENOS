#include "rprintf.h"
#include "page.h"
#include "mmu.h"
#include "delays.h"
#include "fat.h"
#include "systimer.h"

extern unsigned char __bss_end;
extern unsigned char __bss_start;

void test_timer() {
    esp_printf(putc, "Starting timer test...\n");
    unsigned long timer_value;

    // Continuously print the timer value every 500 milliseconds
    for (int i = 0; i < 10; i++) { // Print 10 readings
        timer_value = get_timer_count();
        esp_printf(putc, "Timer value: %lu\n", timer_value);
        wait_msec(500); // Wait 500 ms between readings
    }

    esp_printf(putc, "Timer test complete.\n");
}

void clear_bss() {
    unsigned char *begin_bss = &__bss_start;
    unsigned char *end_bss = &__bss_end;
    while (begin_bss < end_bss) {
        *begin_bss = 0;
        begin_bss++;
    }
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
    asm("mrs %0, CurrentEL"
        : "=r"(el)
        :
        :);
    return el;
}

void kernel_main() {
    unsigned char writeBuffer[512];
    unsigned char readBuffer[512];
    // Initialize timer and enable IRQ
    timer_setup(1);
    success("TIMER SETUP");
    asm("msr DAIFClr, #2");
    success("IRQ SETUP");
    
    init_pfa_list();
    struct ppage *allocd_list = allocate_physical_pages(10);
    free_physical_pages(allocd_list);
    success("Page list set up");
    /* Set up the page table
    if (setupIdentityMap()!=0){
	    fail("[ERROR] MMU INIT FAILED");
	    return;
    }
    success("MMU INITIALIZED\n");

    // Initialize FAT filesystem

    if (fatInit() != 0) {
        fail("[ERROR] FAT INIT FAILED");
        return;
    }
    success("FAT SYSTEM INITIALIZED\n");
    
    // Test SD card write and read
    success("starting");
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
	char c = getc_NB();
	if (c != 0){
		esp_printf(putc, "%c\n", c);
        } else {
		esp_printf(putc, "gun to my head");
	}
        wait_msec(1000000);
        // readLine(buffer); // Uncomment if used.
    }
    esp_printf(putc,"terminating");
}
