#include "cpu/exec/template-start.h"

//relative call, call rel16/32
#define instr callr
//0xe8
static void do_execute(){
	cpu.esp -= 4;
	swaddr_write(cpu.esp , 4 , cpu.eip+DATA_BYTE+1); 
#ifdef MZYDEBUG
	printf("callr %x,\t%x\n",cpu.esp,cpu.eip+DATA_BYTE+1);
#endif
#if DATA_BYTE == 2
	cpu.eip = (cpu.eip + op_src->simm) & 0xffff; 
#elif DATA_BYTE == 4
	cpu.eip = cpu.eip + op_src->simm;
#endif
	print_asm_template1(); 
	return; 
}
make_instr_helper(si); 
#undef instr

//direct call direct, call rm
#define instr call
//0xff /2
static void do_execute(){
	cpu.esp -= 4;
	swaddr_write(reg_l(R_ESP), 4, cpu.eip + 2);
#if DATA_BYTE == 2
	cpu.eip = op_src->val & 0xffff; 
#elif DATA_BYTE == 4
	cpu.eip = op_src->val - 2;	//TODO: check -2
#endif
	print_asm_template1(); 
	return; 
}
make_instr_helper(rm); 
#undef instr


#include "cpu/exec/template-end.h"





