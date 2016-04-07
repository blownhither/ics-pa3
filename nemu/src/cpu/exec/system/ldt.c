#include "cpu/exec/helper.h"
#include "monitor/monitor.h"
make_helper(lgdt){	//0F 01 /2
	cpu.gdtr.limit = instr_fetch(cpu.eip-2, 2);
	cpu.gdtr.base = instr_fetch(cpu.eip, 4);
	if(ops_decoded.is_data_size_16){
		cpu.gdtr.base &= 0xffffff;
	}
	return 4;
	print_asm("lgdt %x",cpu.gdtr.base);
}


