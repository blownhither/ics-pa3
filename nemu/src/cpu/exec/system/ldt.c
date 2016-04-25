#include "cpu/exec/helper.h"
#include "monitor/monitor.h"
make_helper(lgdt){	//0F 01 /2
	lnaddr_t addr = instr_fetch(cpu.eip+3, 4);
	cpu.gdtr.limit = lnaddr_read(addr, 2);
	cpu.gdtr.base = instr_fetch(addr + 2, 4);
	if(ops_decoded.is_data_size_16){
		cpu.gdtr.base &= 0xffffff;
	}
	print_asm("lgdt %x",cpu.gdtr.base);
	return 6;	//TODO: always 6?
}


