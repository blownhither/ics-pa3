#include "cpu/exec/helper.h"

make_helper(exec);

make_helper(movs_b) {
	const int ES = 0;	//TODO: check ES
	swaddr_write(ES + cpu.edi, 1, swaddr_read(cpu.esi,1));
	cpu.edi += eflags.eflags.DF? -1:1;
	cpu.esi += eflags.eflags.DF? -1:1;
	return 1;
}

make_helper(movs_v) {
	const int ES = 0;	//TODO: check ES
	if(ops_decoded.is_data_size_16){
		swaddr_write(ES + cpu.edi, 2, swaddr_read(cpu.esi,2));
		cpu.edi += eflags.eflags.DF? -2:2;
		cpu.esi += eflags.eflags.DF? -2:2;
	}
	else{
		swaddr_write(ES + cpu.edi, 4, swaddr_read(cpu.esi,4));
		cpu.edi += eflags.eflags.DF? -4:4;
		cpu.esi += eflags.eflags.DF? -4:4;
	}
	return 1;
}

//NOTE: does double word prefix still means data_size_16 before movs?
