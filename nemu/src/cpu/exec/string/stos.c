#include "cpu/exec/helper.h"

make_helper(exec);

make_helper(stos_b) {
	const int ES = 0;	//TODO: check ES
	swaddr_write(ES + cpu.edi, 1, cpu.al);
	cpu.edi += eflags.eflags.DF? -1:1;
	return 1;
}

make_helper(stos_v) {
	const int ES = 0;	//TODO: check ES
	if(ops_decoded.is_data_size_16){
		printf("edi=0x%x, ax=0x%x",cpu.edi, cpu.ax);
		swaddr_write(ES + cpu.edi, 2, cpu.ax);
		cpu.edi += eflags.eflags.DF? -2:2;
	}
	else{
		swaddr_write(ES + cpu.edi, 4, cpu.eax);
		cpu.edi += eflags.eflags.DF? -4:4;
	}
	return 1;
}
