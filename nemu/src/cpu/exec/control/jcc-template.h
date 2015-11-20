#include "cpu/exec/template-start.h"

#define instr jcc


/*how to decide between je_w and je_l?*/
#if DATA_BYTE == 4 || DATA_BYTE == 2
/*
make_helper(je_v){
#if DATA_BYTE == 4 
	DATA_TYPE offset = fetch_instr(eip+2,4);
	
#elif DATA_BYTE == 2
	DATA_TYPE offset = fetch_instr(eip+2,2);

#endif
	print_asm("je 0x%x <offset = 0x%x\n>",cpu.eip,offset);
	return 0;
}
*/
#endif


#if DATA_BYTE == 1
make_helper(je_b){
	DATA_TYPE offset = instr_fetch(eip+1,1);
	#ifdef MZYDEBUG
	printf("offset fetched is 0x%x",offset);
	#endif	
	cpu.eip += 2;
	if(!!eflags.eflags.ZF){
		cpu.eip += offset;
	}
	print_asm("je 0x%x <offset 0x%x\n>",cpu.eip,offset);
	return 0;
}
#endif



#include "cpu/exec/template-end.h"
