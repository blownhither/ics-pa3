#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
#define instr cdq
make_helper(cdq){
	cpu.edx = 0 - (MSB(cpu.eax)==1);
	//printf("%x",cpu.edx);	
	print_asm("cdq/cltd");
	return 1;
}

#include "cpu/exec/template-end.h"
