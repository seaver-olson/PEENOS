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
    } else if (strcmp(command, "logo") == 0) {
        logo();
    } else {
        esp_printf(putc, "Unknown command: %s\n", command);
    }
}

void readLine(char *buffer){
    char c;
    int i=0;
    while(1){
        c=getc();
        if(c=='\n'){
            buffer[i]='\0';
            return;
        }
        buffer[i]=c;
        i++;
	if (i >= CMD_BUFFER_SIZE - 1) { 
		buffer[i] = '\0';
        	esp_printf(putc, "Input too long. Truncated to: %s\n", buffer);
                return;
	}
    }
}

void pShell(){
    char buffer[CMD_BUFFER_SIZE];
    while(1){
        esp_printf(putc, "PEENOS 8==> ");
        readLine(buffer);
	if (strlen(buffer) == 0){
		warning("Recieved empty command");
	}
	esp_printf(putc, "&c", getc());
	executeCommand(buffer);
    }
}

void kernel_main() {
    auxInit();
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
    pShell();
    while (1) {
    }
    esp_printf(putc,"terminating");
}
