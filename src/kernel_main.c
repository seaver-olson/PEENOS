#include "rprintf.h"
#include "page.h"
#include "mmu.h"
#include "delays.h"
#include "fat.h"
#include "systimer.h"
#include "interrupt.h"
#include "cmds.h"
#include "sd.h"

#define CMD_BUFFER_SIZE 512




void executeCommand(char *command) {
    if (strcmp(command, "help") == 0) {
        esp_printf(putc, "Available commands:\n");
        esp_printf(putc, "  help - Show this help message\n");
        esp_printf(putc, "  logo - Display the logo\n");
        esp_printf(putc, "  clear - Clear the screen\n");
        esp_printf(putc, "  meminfo - Display memory information\n");
    } else if (strcmp(command, "clear") == 0) {
        clearScreen();
    } else if (strcmp(command, "logo") == 0) {
        logo();
    } else if (strcmp(command, "meminfo") == 0) {
        meminfo();
    } else {
        esp_printf(putc, "Unknown command: %s\n", command);
    }
}

void kernel_main() {
    char buffer[CMD_BUFFER_SIZE] = {0};
    int buf_index = 0;
    auxInit();
    logo();
    clear_bss();
    esp_printf(putc, "Current EL: %d\n", getEL());
    //unsigned char writeBuffer[512];
    //unsigned char readBuffer[512];
    // Initialize timer and enable IRQ
    //timer_setup(1);
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
    esp_printf(putc, "PEENOS 8==> ");
    while (1) {
        char c = getc();
        if (c == '\n') {
	    	buffer[buf_index] = '\0';
                executeCommand(buffer);
		buf_index=0;//go back to start of line
		esp_printf(putc, "\nPEENOS 8==> ");
        	
	} else if (c == '\0') continue;
        else {
		if (buf_index < CMD_BUFFER_SIZE - 1) {
                	buffer[buf_index++] = c; // Add character to buffer
               		putc(c); // Echo back
		} else{
			esp_printf(putc, "\nBuffer overflow! Resetting...\n");
                	buf_index = 0; // Reset buffer
                	esp_printf(putc, "PEENOS 8==> "); 
		}
	}
    }
    esp_printf(putc,"terminating");
}
