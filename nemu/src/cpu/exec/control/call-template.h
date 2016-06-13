#include "cpu/exec/template-start.h"

/*

//relative call, call rel16/32
#define instr callr
//0xe8
static void do_execute(){
	cpu.esp -= 4;
	swaddr_write(cpu.esp , 4 , cpu.eip + DATA_BYTE + 1);//cpu.eip+DATA_BYTE+1); 
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
	cpu.eip = op_src->val - get_instr_len() - 1;
	// get_instr_len vary from 1 to 2, defined in helper.h
#endif
	print_asm_template1(); 
	return; 
}
make_instr_helper(rm)
#undef instr

*/
#define instr call

static void do_execute() {
	int len = get_instr_len();
	cpu.esp -= 4;
	current_sreg = SS_NUM;
	swaddr_write(cpu.esp, 4, cpu.eip + len);
	if(op_src->type == OP_TYPE_IMM) {
		cpu.eip += op_src->val;
		print_asm(op_src->str, OP_STR_SIZE, "$0x%x", cpu.eip + len + 1);
	}else {
		cpu.eip = op_src->val - len - 1;
	}
	print_asm_template1();
}

make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
