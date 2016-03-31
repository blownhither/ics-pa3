#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
#define instr cbw
make_helper(cbw){
	if(ops_decoded.is_data_size_16){	//cbw, byte to word
		cpu.ax = (cpu.al&0x80)? (0xff00|cpu.al) : (0x00ff&cpu.al);
	}
	else{
		cpu.eax = (cpu.ax&0x8000)? (0xffff0000|cpu.ax) : (0x0000ffff&cpu.ax);
	}
	print_asm("cbd/cwd");
	return 1;
}

#include "cpu/exec/template-end.h"
