#include "cpu/exec/helper.h"

make_helper(exec);
lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t cur_segr);

make_helper(stos_b) {
	lnaddr_t addr = seg_translate(cpu.edi, 1, ES_NUM);
	lnaddr_write(addr, 1, cpu.al);
	cpu.edi += eflags.eflags.DF? -1:1;
	print_asm("%%ax,%%es:(%%edi)");
	return 1;
}

make_helper(stos_v) {
	lnaddr_t addr;
	if(ops_decoded.is_data_size_16){
		addr = seg_translate(cpu.edi, 2, ES_NUM);
		lnaddr_write(addr, 2, cpu.ax);
		cpu.edi += eflags.eflags.DF? -2:2;
	}
	else{
		addr = seg_translate(cpu.edi, 4, ES_NUM);
		//assert(cpu.edi == addr);
		lnaddr_write(addr, 4, cpu.eax);
		cpu.edi += eflags.eflags.DF? -4:4;
		printf("stos: addr=0x%x, eax=0x%x\n", addr, cpu.eax);
	}
	
	print_asm("%%eax,%%es:(%%edi)");
	return 1;
}
