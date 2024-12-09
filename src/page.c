
#include "page.h"
#define NULL    ((void*)0)//some how I just found out NULL doesn't come with C, I feel very stupid

#define TOTAL_PAGES 128

struct ppage physical_page_array[TOTAL_PAGES];
struct ppage *physical_frame_allocation = NULL;
unsigned int total_physical_pages = TOTAL_PAGES;

void init_pfa_list(void) {
	physical_frame_allocation = &physical_page_array[0];
	struct ppage *current_physical_page = physical_frame_allocation;
	for (int i = 0; i < TOTAL_PAGES - 1; i++) {
        current_physical_page->next = &physical_page_array[i + 1];
        current_physical_page->physical_addr = (void*)(i * 0x1000); // Assuming 4 KB pages
        current_physical_page->prev = (i > 0) ? &physical_page_array[i - 1] : NULL;
        current_physical_page = current_physical_page->next;
    }
	current_physical_page->next = NULL;
    current_physical_page->prev = &physical_page_array[TOTAL_PAGES - 2];
	current_physical_page->physical_addr = (void*)((TOTAL_PAGES - 1) * 0x1000);
}

struct ppage *allocate_physical_pages(unsigned int npages){
	//if no free frames return NULL to prevent SegFault
	if (physical_frame_allocation == NULL){
		return NULL;
	}
	struct ppage *allocd_list = physical_frame_allocation;
  	struct ppage *current_physical_page = allocd_list;
	for (int i = 0; i < npages; i++){
		if (current_physical_page == NULL){
			return NULL;//prevent segFault from asking for more memory than exists
		}
		current_physical_page = current_physical_page->next;
	}
	struct ppage *new_head = current_physical_page;
    if (new_head != NULL) {
        new_head->prev = NULL;
    }
    physical_frame_allocation = new_head;
	
    if (current_physical_page != NULL) {
        current_physical_page->prev->next = NULL;
    }
	return  allocd_list;
}

void free_physical_pages(struct ppage *ppage_list){
	if (ppage_list == NULL){
		return;
	}
	struct ppage *current_physical_page;
	current_physical_page = ppage_list;
	while (current_physical_page->next != NULL){
		current_physical_page = current_physical_page->next;
	}

	if (physical_frame_allocation != NULL){
		physical_frame_allocation->prev = current_physical_page;
	}

	current_physical_page->next = physical_frame_allocation;
	physical_frame_allocation = ppage_list;
}

unsigned int get_total_pages() {
    return total_physical_pages; 
}

unsigned int get_used_pages() {
    unsigned int free_page_count = 0;
    struct ppage *current = free_page_list;  // Assume this is your free page list head
    while (current != NULL) {
        free_page_count++;
        current = current->next;
    }
    return total_physical_pages - free_page_count;
}

unsigned int get_free_pages() {
    unsigned int free_page_count = 0;
    struct ppage *current = physical_frame_allocation;

    while (current != NULL) {
        free_page_count++;
        current = current->next;
    }

    return free_page_count;
}

