#include "cpu/exec/helper.h"

#ifdef DATA_BYTE
#undef DATA_BYTE
#endif
#define DATA_BYTE 4

#include "cpu/exec/template-start.h"

//only 0xc3 and 0xc2 !
make_helper(ret){
	
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp += 4;
	cpu.eip &= 0x0000ffff;
	print_asm("ret");
	return 1;

}
/*
make_helper(ret_i_w){
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp += 4;			//return address
	cpu.esp += op_src->imm;
	print_asm("ret");
	return 1;
}*/
#include "cpu/exec/template-end.h"
