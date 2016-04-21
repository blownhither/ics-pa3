#include "nemu.h"

static inline uint32_t segDesc_to_base(lnaddr_t desc_addr) {
	uint32_t b15 = lnaddr_read(desc_addr + 2, 2);
	uint32_t b23 = lnaddr_read(desc_addr + 4, 1);
	uint32_t b31 = lnaddr_read(desc_addr + 7, 1);
	return b15 + (b23 << 16) + (b31 << 24);
}

static inline uint32_t segDesc_to_limit(lnaddr_t desc_addr) {
	uint32_t b15 = lnaddr_read(desc_addr, 2);
	uint32_t b19 = lnaddr_read(desc_addr + 6, 1) & 0x0f;	//4 bit
	return b15 + (b19 << 16);
}
