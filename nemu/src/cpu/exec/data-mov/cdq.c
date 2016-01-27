

#define instr cdq
#define MSB_32(x) ((x>>31)&1)
#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
make_helper(cdq){
	cpu.edx = 0 - (MSB_32(cpu.eax)==1);
	printf("%x",cpu.edx);	
	print_asm("cdq/cltq");
	return 1;
}

#include "cpu/exec/template-end.h"
