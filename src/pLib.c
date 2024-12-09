#include "pLib.h"
#include "rprintf.h"
#include "mmu.h"
#include "delays.h"
#include "fat.h"
#include "systimer.h"
#include "interrupt.h"
#include "cmds.h"
#include "sd.h"

void clearScreen(){
    esp_printf(putc, "\033[2J\033[H");
}

void meminfo() {
    // Assuming you have a function to get the total number of physical pages
    unsigned int total_pages = get_total_pages();
    unsigned int used_pages = get_used_pages();
    unsigned int free_pages = total_pages - used_pages;

    // Assuming each page is 4 KB (adjust as necessary)
    unsigned int page_size_kb = 4;

    esp_printf(putc, "Memory Information:\n");
    esp_printf(putc, "  Total Memory: %d KB\n", total_pages * page_size_kb);
    esp_printf(putc, "  Used Memory: %d KB\n", used_pages * page_size_kb);
    esp_printf(putc, "  Free Memory: %d KB\n", free_pages * page_size_kb);
}
