#include "cpu/exec/helper.h"
#include "monitor/monitor.h"
make_helper(lidt){	//0F 01 /2
	assert(instr_fetch(cpu.eip,1)==0x18);
	lnaddr_t addr = cpu.eax;
	//printf("lgdt: addr=0x%x\n",addr);
	cpu.idtr.limit = lnaddr_read(addr, 2);
	cpu.idtr.base = lnaddr_read(addr + 2, 4);
	if(ops_decoded.is_data_size_16){
		cpu.idtr.base &= 0xffffff;
	}
	print_asm("lidt %x",addr);
	return 6;	//TODO: always 6?
}


