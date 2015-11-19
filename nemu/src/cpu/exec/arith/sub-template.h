#include "cpu/exec/template-start.h"

#define instr sub

//0x83 is to sub imm8 from longer r/m
//others are to sub a length from same length
//do_execute is do_instr_SUFFIX

//#define op_src (&ops_decoded.src)
//DATA_BYTE ,  DATA_TYPE is refering to latter operand(SRC)?
//and SUFFIX is referring to former opearand

//if a is byte and b is word/dword
//then b=b-(signextend)a
//else b = b - a
static void do_execute(){
	DATA_TYPE a; 
	DATA_TYPE b = op_dest->val; 
#if DATA_BYTE == 4
	a = (DATA_TYPE)op_src->val & 0xffffffff; 
#elif DATA_BYTE == 2
	a = (DATA_TYPE)op_src->val & 0xffff; 
#else
	a = op_src->val; 
#endif
	DATA_TYPE result = b-a;
	eflags.eflags.CF = (a > b);
	/*
	uint32_t pf_temp = (result >> 16) ^ result; 
	pf_temp = (result >> 8) ^ result; 
	pf_temp = (result >> 4) ^ result; 
	pf_temp = (result >> 2) ^ result;
	pf_temp = ((result >> 1) ^ result) & 1;
	*/ 
	extern bool parity_check(uint32_t ); 
	eflags.eflags.PF = parity_check(result&0xffff );
	//TODO :eflags.eflags.AF = ;
	eflags.eflags.SF = (result >> (DATA_BYTE - 1))&1;
	eflags.eflags.ZF = (a == b);
	eflags.eflags.OF = (op_src->val<0 && op_dest->val>0 && result<0) || (op_src->val>0 && op_dest->val<0 && result>0);

	OPERAND_W(op_dest , result); 
	print_asm_template2(); 
}
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)


#include "cpu/exec/template-end.h"
