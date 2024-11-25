#include "rprintf.h"
#include "page.h"
#include "mmu.h"
#include "delays.h"
#include "fat.h"
#include "systimer.h"

extern unsigned char __bss_end;
extern unsigned char __bss_start;

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
    char buffer[512];  // Currently unused; consider removing or utilizing.

    // Initialize timer and enable IRQ
    if (timer_init(1) != 0) fail("[ERROR] TIMER INIT FAILED"); return;
    asm("msr DAIFClr, #2");
    success("IRQ SETUP");
    
    // Set up the page table
    if (setupIdentityMap()!=0){
	fail("[ERROR] MMU INIT FAILED");
	return;
    }
    success("MMU INITIALIZED\n");
    // Initialize the SD card
    if (sd_init() != 0) {
        fail("[ERROR] SD INIT FAILED");
        return;
    }
    success("SD CARD INITIALIZED\n");

    // Initialize FAT filesystem
    if (fatInit() != 0) {
        fail("[ERROR] FAT INIT FAILED");
        return;
    }
    success("FAT SYSTEM INITIALIZED\n");

    // Test SD card write and read
    unsigned char writeBuffer[512] = {0};
    memcpy(writeBuffer, "Test data for SD card write", strlen("Test data for SD card write"));
    unsigned char readBuffer[512] = {0};

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

    hexdump(readBuffer, 512);

    while (1) {
        wait_msec(1000);
        // readLine(buffer); // Uncomment if used.
    }
}
