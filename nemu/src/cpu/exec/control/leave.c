#include "cpu/exec/helper.h"

#ifdef DATA_BYTE
#undef DATA_BYTE
#endif
#define DATA_BYTE 4

#include "cpu/exec/template-start.h"
make_helper(leave){
	cpu.esp = cpu.ebp;
	cpu.ebp = MEM_R(cpu.esp);	//same as swaddr_write
	cpu.esp += 4;				//return address
	print_asm("leave");
	return 1;

}
#include "cpu/exec/template-end.h"
