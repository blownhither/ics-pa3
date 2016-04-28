#ifndef __MMU_TOOL_H
#define __MMU_TOOL_H

//#include "nemu.h"
#include "../../lib-common/x86-inc/mmu.h"

/* Ma Ziyin: the 16bit segment selector*/
typedef union SegmentSelector {
	uint16_t val;
	struct {
		uint16_t RPL	: 2;
		uint16_t TI		: 1;
		uint16_t index	: 13;
	};
} SegSelc;

typedef struct SegmentDescriptorCache {
	uint32_t base;
	uint32_t limit;
	uint8_t  DPL;	//segment access requirement
} DescCache;


typedef union VirtualPageNo {
	uint32_t val;
	struct {
		uint32_t offset	: 12;
		uint32_t pt		: 10;
		uint32_t pi		: 10;
	};	
	struct {
		uint32_t 		: 12;
		uint32_t vpn  	: 20;
	};
} VPN;

/*
extern uint32_t segDesc_to_base(lnaddr_t desc_addr) ;
extern uint32_t segDesc_to_limit(lnaddr_t desc_addr) ;
*/
extern void load_desc_cache(uint16_t cur_sreg);
lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t cur_segr);
hwaddr_t page_translate(lnaddr_t addr);
hwaddr_t search_tlb(lnaddr_t addr);
void init_tlb();

#endif
