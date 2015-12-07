#include "cpu/exec/template-start.h"

#define instr je


//#if DATA_BYTE == 4 || DATA_BYTE == 2
make_helper(concat(je_i_ , SUFFIX)){

	DATA_TYPE_S offset = instr_fetch(eip+1 , DATA_BYTE); 
	//printf("offset fetched is %x\n",offset);
	//printf("eip now is %x\n",cpu.eip);	
	if(eflags.eflags.ZF){
		cpu.eip += offset; 
	}
	//printf("eip now is %x\n",cpu.eip);
	print_asm("je 0x%x <offset = 0x%x\n>",cpu.eip+DATA_BYTE+1,offset);
	//printf("eip now is %x\n",cpu.eip);
	//printf("DATA_BYTE is %x",DATA_BYTE);
	return DATA_BYTE + 1;	//?!
}
/*
#elif DATA_BYTE == 1
make_helper(je_i_b){
	DATA_TYPE_S offset = instr_fetch(eip+1 , DATA_BYTE); 
	//printf("in _b offset fetched is %x\n",offset);
	//printf("eip now is %x\n",cpu.eip);	
	if(eflags.eflags.ZF){
		cpu.eip += offset; 
	}
	//printf("eip now is %x\n",cpu.eip);
	print_asm("je 0x%x <offset = 0x%x\n>",cpu.eip+DATA_BYTE+1,offset);
	//printf("eip now is %x\n",cpu.eip);
	//printf("DATA_BYTE is %x\n",DATA_BYTE);
	return 2;	//?!
}

#endif
*/


#undef instr
#include "cpu/exec/template-end.h"
