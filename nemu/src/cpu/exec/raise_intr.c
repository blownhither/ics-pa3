#include "../../lib-common/x86-inc/mmu.h"
#include "nemu.h"
#include <setjmp.h>
extern jmp_buf jbuf;
extern uint32_t current_sreg;
void load_desc_cache(uint16_t cur_sreg);

void raise_intr(uint8_t NO) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 * That is, use ``NO'' to index the IDT.
	 */
	current_sreg = SS_NUM;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, eflags.eflags_l);
	current_sreg = SS_NUM;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.cs);
	current_sreg = SS_NUM;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip);
	
	printf("raise_intr:0x%x\n",NO);
	uint8_t buf[8];
	int i;
	for(i=0; i<8; i++)
		buf[i] = lnaddr_read(cpu.idtr.base + (NO<<3) + i, 1);
	GateDesc *gd;
	gd = (GateDesc *)(void *)buf;
	cpu.cs = gd->segment;
	load_desc_cache(CS_NUM);
	cpu.eip = gd->offset_15_0 + (gd->offset_31_16<<16);
	
	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}

/*
mmu.h
typedef struct GateDescriptor {
	uint32_t offset_15_0      : 16;
	uint32_t segment          : 16;
	uint32_t pad0             : 8;
	uint32_t type             : 4;
	uint32_t system           : 1;
	uint32_t privilege_level  : 2;
	uint32_t present          : 1;
	uint32_t offset_31_16     : 16;
} GateDesc;
*/
