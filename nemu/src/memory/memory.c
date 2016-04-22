#include "common.h"
#include "mmu_tool.h"
#include "cpu/reg.h"
#include "../../lib-common/x86-inc/mmu.h"

/* Memory accessing interfaces */
 void L1_cache_write ( hwaddr_t _addr, size_t len, uint32_t data );
 uint32_t L1_cache_read(hwaddr_t addr, size_t len);
 uint32_t dram_read(hwaddr_t addr, size_t len);
 void dram_write(hwaddr_t addr, size_t len, uint32_t data);
 
uint32_t current_sreg;	//segment register in use
 
uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	uint32_t ret2 = L1_cache_read(addr, len) & (~0u >> ((4 - len) << 3));
#ifdef MZYDEBUG
	uint32_t ret = dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	if(ret != ret2) {
		printf("dram:0x%x,cache:0x%x\n",ret,ret2);
		printf("eip=0x%x,addr=0x%x\n",cpu.eip, (uint32_t)addr);
	}
#endif
	//return ret;
	return ret2;
}
//#define MZYDEBUG
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	//dram_write(addr, len, data);
	L1_cache_write(addr, len, data);
#ifdef MZYDEBUG
	uint32_t result = hwaddr_read(addr, len);
	
	if(result != data) {
		printf("written:0x%x at 0x%x\tshould be 0x%x\n",result , addr, data);
		printf("Found inconsistancy at eip=0x%x\n",cpu.eip);
	}
#endif
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}
#define MZYDEBUG
lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t cur_segr) {
	assert(!cpu.segr[cur_segr].ti);		//global descriptor
	lnaddr_t desc_addr = cpu.gdtr.base + (cpu.segr[cur_segr].index << 3);
	lnaddr_t base = segDesc_to_base(desc_addr);
	lnaddr_t limit =segDesc_to_limit(desc_addr);
#ifdef MZYDEBUG
	printf("base:0x%x limit:0x%x addr:0x%x\ndesc_addr:0x%x\n",base,limit,addr,desc_addr);
#endif
	assert(addr < limit);
	return base + addr;
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr;
	if(cpu.cr0.PE){
		lnaddr = seg_translate(addr, len, current_sreg);	
	}
	else 
		lnaddr = addr;
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr;
	if(cpu.cr0.PE)
		lnaddr = seg_translate(addr, len, current_sreg);
	else
		lnaddr = addr;
	lnaddr_write(lnaddr, len, data);
}

