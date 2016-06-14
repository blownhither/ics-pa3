#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)

/* Use the function to get the start address of user page directory. */
inline PDE* get_updir();

static PTE vptable[NR_PTE] align_to_page;

void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	//panic("please implement me");
	
	PDE *pdir = get_updir();//(PDE *)va_to_pa(kpdir);
	PTE *ptable = (vptable + (VMEM_ADDR >> 12));//(PTE *)va_to_pa(kptable);
	pdir[0].val = make_pde(va_to_pa(ptable));
	int i;
	for(i = VMEM_ADDR; i < VMEM_ADDR + SCR_SIZE + PAGE_SIZE; i += PAGE_SIZE) {
		ptable->val = make_pte(i);
		ptable ++;
	}
	
}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	//Log("tested\n");
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

