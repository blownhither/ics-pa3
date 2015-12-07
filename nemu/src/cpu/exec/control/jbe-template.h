#include "cpu/exec/template-start.h"
#define instr jbe
make_helper(concat(jbe_si_ , SUFFIX)){

	DATA_TYPE_S offset = instr_fetch(eip+1 , DATA_BYTE); 
	if(eflags.eflags.CF==1 || eflags.eflags.ZF==1){

		//if(offset<0)
		//cpu.eip += (offset | 0xffffff00);
		if(offset<0)
		cpu.eip += offset;
		else
		cpu.eip -= ~offset + 1;
 
	}
	print_asm("jbe ""0x%x <offset = 0x%x\n>",cpu.eip+DATA_BYTE+1,offset);
	return DATA_BYTE + 1;
}
#undef instr
#include "cpu/exec/template-end.h"
