#include "TLB.h"
#include "mmu_tool.h"
#include <stdlib.h>

static int count_tlb = 0;
static TLB tlb[TLB_SIZE];

int load_tlb(lnaddr_t addr){
	int i;
	if(count_tlb < TLB_SIZE) {
		count_tlb ++;
		for(i=0; i<TLB_SIZE; ++i) 
			if(!tlb[i].valid) break;
#ifdef MZYDEBUG
		assert(i < TLB_SIZE)
#endif
	}
	else {
		i = rand()%TLB_SIZE;
	}
	tlb[i].valid = true;
	VPN v;
	PDE pde;
	PTE pte;
	v.val = addr;
	tlb[i].tag = v.vpn;
	pde.val = hwaddr_read((cpu.cr3.base << 12) + (v.pi * 4), 4);
#ifdef MZYDEBUG
	printf("addr=0x%x 0x%x->pde=0x%x \n",addr,(cpu.cr3.base << 12) + (v.pi * 4),pde.val);
#endif
	Assert(pde.present,"PDE not present(0x%x) at 0x%x when accessing lnaddr 0x%x\n",pde.val,(cpu.cr3.base << 12) + (v.pi * 4),addr);
	pte.val = hwaddr_read((pde.page_frame << 12) + ((uint32_t)v.pt * 4), 4);
#ifdef MZYDEBUG
	printf("\t0x%x->pte=0x%x ans=0x%x\n",(pde.page_frame << 12) + (v.pt * 4),pte.val, (pte.page_frame << 12) + v.offset);
#endif
	Assert(pte.present,"PTE not present(0x%x) at 0x%x when accessing lnaddr 0x%x\n\teip==0x%x",pte.val,(pde.page_frame << 12) + (v.pt * 4),addr,cpu.eip);
	tlb[i].ppn = pte.page_frame;
	return i;
}

hwaddr_t search_tlb(lnaddr_t addr){
	int i;
	VPN v;
	v.val = addr;
	for(i=0; i<TLB_SIZE; ++i){
		if(!tlb[i].valid || tlb[i].tag != v.vpn) continue;
		//found
		return tlb[i].ppn;
	}
	//not found
	i = load_tlb(addr);
	return tlb[i].ppn;
}

void init_tlb(){
	count_tlb = 0;
	memset(tlb, 0, sizeof(tlb));
}
