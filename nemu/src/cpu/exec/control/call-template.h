#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	reg_l(4) += 4; //esp
	/*TODO ?! why +=
	//DATA_TYPE temp = reg_w(4);
	MEM_W(reg_l(4) , cpu.eip); //= swaddr_write	
	#if DATA_BYTE == 2
	cpu.eip = (cpu.eip + op_src->val) & 0xffff; 
*///TODO*/
#if DATA_BYTE == 4
	cpu.eip = cpu.eip + op_src->val; 
#endif
	print_asm_template1(); 
	return; 
}

make_instr_helper(si); 
#include "cpu/exec/template-end.h"
