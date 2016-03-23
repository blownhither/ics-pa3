#include "common.h"
#include "cpu/reg.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t cache_read(hwaddr_t addr, size_t len);
void cache_write ( hwaddr_t _addr, size_t len, uint32_t data );
/* Memory accessing interfaces */

#define MZYDEBUG

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	uint32_t ret2 = cache_read(addr, len) & (~0u >> ((4 - len) << 3));
#ifdef MZYDEBUG
	uint32_t ret = dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	if(ret != ret2) {
		printf("dram:0x%x,cache:0x%x\n",ret,ret2);
		printf("eip=0x%x,addr=0x%x",cpu.eip, (uint32_t)addr);
		assert(0);
	}
#endif
	//return ret;
	return ret2;
}
//#define MZYDEBUG
#undef MZYDEBUG
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	dram_write(addr, len, data);
	//cache_write(addr, len, data);
#ifdef MZYDEBUG
	uint32_t result = hwaddr_read(addr, len);
	printf("written:0x%x at 0x%x\tshould be 0x%x\n",result , addr, data);
	if(result != data) {
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

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

