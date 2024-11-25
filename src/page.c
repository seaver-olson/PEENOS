
#include "page.h"
#define NULL    ((void*)0)//some how I just found out NULL doesn't come with C, I feel very stupid

struct ppage physical_page_array[128];
struct ppage *physical_frame_allocation = &physical_page_array[0];

void init_pfa_list(void) {
	physical_frame_allocation->prev = NULL;
	struct ppage *current_physical_page;
	for (int i = 0; i < 127;i++){
		current_physical_page = &physical_page_array[i+1]; //swapped the initizaitions  of curr_p_p and free_p_p, fixed seg fault
		physical_frame_allocation->next = current_physical_page;
		current_physical_page->prev = physical_frame_allocation;
		physical_frame_allocation->physical_addr  = (void *) &physical_page_array[i];
		physical_frame_allocation = current_physical_page;
	}
	physical_frame_allocation->next = NULL;//end of free physical pages
	physical_frame_allocation->physical_addr  = (void *)  &physical_page_array[127];
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
		if (i+1 == npages){
			
			physical_frame_allocation = current_physical_page;
			if (physical_frame_allocation != NULL){
				physical_frame_allocation->prev = NULL;
			}
			current_physical_page->next = NULL;
		}
	}
	return  allocd_list;
}

void free_physical_pages(struct ppage *ppage_list){
	if (ppage_list == NULL){
		return;
	}
	struct ppage *current_physical_page;
	current_physical_page = ppage_list;
	//get to end of ppage_list
	while (current_physical_page->next != NULL){
		current_physical_page = current_physical_page->next;
	}

	if (physical_frame_allocation != NULL){
		physical_frame_allocation->prev = current_physical_page;
	}

	current_physical_page->next = physical_frame_allocation;
	physical_frame_allocation = ppage_list;
}


int main(){
	init_pfa_list();
	struct ppage *allocd_list;
	allocd_list = allocate_physical_pages(2);
	free_physical_pages(allocd_list);
	return 0;
}

