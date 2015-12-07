#include "cpu/exec/template-start.h"

#define instr je


//#if DATA_BYTE == 4 || DATA_BYTE == 2
make_helper(concat(je_i_ , SUFFIX)){

	DATA_TYPE_S offset = instr_fetch(eip+1 , DATA_BYTE); 
	printf("offset fetched is %x\n",offset);
	printf("eip now is %x\n",cpu.eip);	
	if(eflags.eflags.ZF){
		cpu.eip += offset; 
	}
	printf("eip now is %x\n",cpu.eip);
	print_asm("je 0x%x <offset = 0x%x\n>",cpu.eip+DATA_BYTE+1,offset);
	printf("eip now is %x\n",cpu.eip);

	return DATA_BYTE + 1;	//?!
}

#undef instr
#include "cpu/exec/template-end.h"
