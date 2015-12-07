#include "cpu/exec/template-start.h"

#define instr je


//#if DATA_BYTE == 4 || DATA_BYTE == 2
make_helper(concat(je_i_ , SUFFIX)){

	DATA_TYPE offset = instr_fetch(eip+1 , DATA_BYTE); 
	if(!!eflags.eflags.ZF){
		cpu.eip += offset; 
	}
	print_asm("je 0x%x <offset = 0x%x\n>",cpu.eip+DATA_BYTE+1,offset);
	return DATA_BYTE;
}

#undef instr
#include "cpu/exec/template-end.h"
