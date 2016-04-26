#include "nemu.h"
#include "../../lib-common/x86-inc/mmu.h"

/*
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
*/

#define MZYDEBUG
lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t cur_segr) {

#ifdef MZYDEBUG
	Assert(!cpu.segr[cur_segr].ti, "Segment fault: Local Descriptor table required.\n");		//global descriptor
	//access DescCache 
	if((cpu.desc_cache[cur_segr].limit<<12) <= addr+len-1)
		printf("limit=0x%x, addr=0x%x\n", cpu.desc_cache[cur_segr].limit<<12, addr);
	Assert((cpu.desc_cache[cur_segr].limit<<12) > addr+len-1, "Segment fault: access out of limit at 0x%x.\n", cpu.desc_cache[cur_segr].limit<<12);
	Assert(cpu.desc_cache[cur_segr].DPL >= cpu.segr[cur_segr].RPL, "Segment fault: TSS DPL must be ≥ TSS selector RPL.\n");
	//DPL >= CPL (current process)
#endif

	return cpu.desc_cache[cur_segr].base + addr;
}

void load_desc_cache(uint16_t cur_sreg) {
	uint8_t buf[8];
	int i;
	lnaddr_t desc_addr = cpu.gdtr.base + (cpu.segr[cur_sreg].index );//<< 3);
	for(i=0;i<8;++i){
		buf[i] = lnaddr_read(desc_addr + i, 1);
	}
	SegDesc *p = (SegDesc *)buf;
	cpu.desc_cache[cur_sreg].limit = p->limit_15_0 + (p->limit_19_16<<16);
	cpu.desc_cache[cur_sreg].base = 
		p->base_15_0 + (p->base_23_16<<16) + (p->base_31_24<<24);
}

hwaddr_t page_translate(lnaddr_t addr) {
	VPN vpn;
	PDE pde;
	PTE pte;
	vpn.val = addr;
	pde.val = hwaddr_read(cpu.cr3.base + (vpn.pi << 3), 4);
	pte.val = hwaddr_read((pde.page_frame + (vpn.pt << 3)) << 12, 4);
	return (pte.page_frame << 12) + vpn.offset;
}
