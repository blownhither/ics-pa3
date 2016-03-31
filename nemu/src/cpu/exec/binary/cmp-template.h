#include "cpu/exec/template-start.h"

#define instr cmp

extern bool parity_check(uint32_t );
static void do_execute () {
	DATA_TYPE_S result;
	if(instr_fetch(cpu.eip,1)==0x83){
		//result =op_dest->val - (DATA_TYPE_S)((op_src->simm&0x80)?(0xffffff00|op_src->simm):(0x00ff&op_src->simm));
		result = op_dest->val - op_src->simm;
	}
	else
		result = op_dest->val - op_src->val;
	//!!!note that
	//when "sign-extend is required", please use (DATA_TYPE_S)op_src->imm
	eflags.eflags.ZF = !result;
	eflags.eflags.CF = op_dest->val < op_src->val;
	eflags.eflags.OF = (MSB(op_dest->val) ^ MSB(op_src->val)) 
				&& (MSB(op_dest->val) ^ MSB(result));
	eflags.eflags.SF = MSB(result);
	//eflags.eflags.AF = 
	eflags.eflags.PF = parity_check(result);
	print_asm_template2();
}


#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(i2a)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"
