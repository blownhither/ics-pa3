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
	printf("op_src->val is %x , op_dest->val is %x\n" , op_src->val , op_dest->val); 
#if DATA_BYTE == 4
	cpu.eip = cpu.eip + op_src->simm; 
#endif
	print_asm_template1(); 
	return; 
}

make_instr_helper(si); 
#include "cpu/exec/template-end.h"
