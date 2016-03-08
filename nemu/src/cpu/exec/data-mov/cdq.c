#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
#define instr cdq
make_helper(cdq){
	cpu.edx = (uint32_t) (0 - (MSB(cpu.eax)==1));

	//TODO: unchecked  ops_decoded.is_data_size_16

	print_asm("cdq/cltd");
	return 1;
}

#include "cpu/exec/template-end.h"
