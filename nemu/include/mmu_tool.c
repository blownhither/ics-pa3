#include "nemu.h"
#include "../../lib-common/x86-inc/mmu.h"

uint32_t segDesc_to_base(lnaddr_t desc_addr) {
	uint8_t buf[8];
	int i;
	for(i=0;i<8;++i){
		buf[i] = lnaddr_read(desc_addr + i, 1);
	}
	SegDesc *p = (SegDesc *)buf;
	return p->base_15_0 + (p->base_23_16<<16) + (p->base_31_24<<24);
}

uint32_t segDesc_to_limit(lnaddr_t desc_addr) {
	uint8_t buf[8];
	int i;
	for(i=0;i<8;++i){
		buf[i] = lnaddr_read(desc_addr + i, 1);
	}
	SegDesc *p = (SegDesc *)buf;
	return p->limit_15_0 + (p->limit_19_16<<16);
}
