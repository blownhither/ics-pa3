#include "common.h"
#include "mmu_tool.h"
#include "cpu/reg.h"
#include "../../lib-common/x86-inc/mmu.h"

/* Memory accessing interfaces */
 void L1_cache_write ( hwaddr_t _addr, size_t len, uint32_t data );
 uint32_t L1_cache_read(hwaddr_t addr, size_t len);
 uint32_t dram_read(hwaddr_t addr, size_t len);
 void dram_write(hwaddr_t addr, size_t len, uint32_t data);
 lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t cur_segr);
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
	//assert(len==1 || len==2 || len==4);
	if(!(cpu.cr0.PE && cpu.cr0.PG))
		return hwaddr_read(addr, len);
	if ( 0 ) { /*data cross the page boundary*/
		/* this is a special case, you can handle it later. */
		assert(0);
	}
	else {
		printf("do page_translate!");
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(hwaddr, len);
	}
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	if(!(cpu.cr0.PE && cpu.cr0.PG))
		return hwaddr_write(addr, len, data);
	if ( 0 ) { /*data cross the page boundary*/
		/* this is a special case, you can handle it later. */
		assert(0);
	}
	else {
		printf("do page_translate!\n");
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_write(hwaddr, len, data);
	}
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
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

