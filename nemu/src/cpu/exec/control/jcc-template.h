#include "cpu/exec/template-start.h"

#define instr je


#if DATA_BYTE == 4 || DATA_BYTE == 2
make_helper(concat(je_i_ , SUFFIX)){
#if DATA_BYTE == 4 
	DATA_TYPE offset = instr_fetch(eip+2,4);
	
#elif DATA_BYTE == 2
	DATA_TYPE offset = instr_fetch(eip+2,2);

#endif
	if(!!eflags.eflags.ZF){
		cpu.eip += offset; 
	}
	print_asm("je 0x%x <offset = 0x%x\n>",cpu.eip,offset);
	return DATA_BYTE + 1;
}
#endif


#if DATA_BYTE == 1
make_helper(je_i_b){
	DATA_TYPE_S offset = instr_fetch(eip+1,1);
	#ifdef MZYDEBUG
	printf("offset fetched is 0x%x",offset);
	#endif	
	if(!!eflags.eflags.ZF){
		cpu.eip += offset;
	 }
	print_asm("je 0x%x <offset 0x%x\n>",cpu.eip,offset);
	return 2;
}
#endif

#undef instr

#include "cpu/exec/template-end.h"
