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
int is_mmio(hwaddr_t);
uint32_t mmio_read(hwaddr_t, size_t, int);
void mmio_write(hwaddr_t, size_t, uint32_t, int);

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {

#ifdef HAS_DEVICE
	int mm = is_mmio(addr);
	if (mm != -1){
		//printf("mmio triggered at 0x%x", addr);
		return mmio_read(addr, len, mm) & (~0u >> ((4 - len) << 3));	// already masked
	}
#endif

#ifdef EFFICIENCY
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
#endif

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

#ifdef HAS_DEVICE
	int mm = is_mmio(addr);
	if (mm != -1){
		mmio_write(addr, len, data, mm);
		//assert(mmio_read(addr, len, mm) == data);
		return;
	}
#endif

#ifdef EFFICIENCY
	dram_write(addr, len, data);
	return ;
#endif

	L1_cache_write(addr, len, data);
#ifdef MZYDEBUG
	uint32_t result = hwaddr_read(addr, len);
	
	if(result != data) {
		printf("written:0x%x at 0x%x\tshould be 0x%x\n",result , addr, data);
		printf("Found inconsistancy at eip=0x%x\n",cpu.eip);
	}
#endif
}

#define limit 0x1000

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	//assert(len==1 || len==2 || len==4);
	hwaddr_t hwaddr;
	if(!(/*cpu.cr0.PE && */cpu.cr0.PG))
		return hwaddr_read(addr, len);
	if ( (addr & 0xfff) + len > limit ) { /*data cross the page boundary*/
		/* this is a special case, you can handle it later. */
		uint32_t off = addr & 0xfff;
		//TODO:
		hwaddr_t hwaddr2;
		hwaddr = page_translate(addr);
		hwaddr2 = page_translate(addr + limit - off);
		return hwaddr_read(hwaddr, limit - off) + 
			(hwaddr_read(hwaddr2, len - limit + off) << ((limit - off) * 8));
	}
	else {
		//printf("do page_translate!");
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(hwaddr, len);
	}
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	if(!(/*cpu.cr0.PE && */cpu.cr0.PG))
		return hwaddr_write(addr, len, data);
	if ( 0 ) { /*data cross the page boundary*/
		/* this is a special case, you can handle it later. */
		assert(0);
	}
	else {
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
