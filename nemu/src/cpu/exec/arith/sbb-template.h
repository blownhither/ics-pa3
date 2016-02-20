#include "cpu/exec/template-start.h"

#define instr sbb


static void do_execute(){
	DATA_TYPE a = op_src->val + eflags.eflags.CF?1:0;
	DATA_TYPE b = op_dest->val; 
/*
#if DATA_BYTE == 4
	a = (DATA_TYPE)op_src->val & 0xffffffff; 
#elif DATA_BYTE == 2
	a = (DATA_TYPE)op_src->val & 0xffff; 
#else
	a = op_src->val; 
#endif
*/
	DATA_TYPE result = b-a;
#define MZYDEBUG
#ifdef MZYDEBUG
	printf("%x - %x = %x",b,a,result);
#endif
	eflags.eflags.CF = (a > b);
	extern bool parity_check(uint32_t ); 
	eflags.eflags.PF = parity_check(result&0xffff );
	//eflags.eflags.AF = 
	eflags.eflags.SF = (result >> (DATA_BYTE - 1))&1;
	eflags.eflags.ZF = (a == b);
	eflags.eflags.OF = (op_src->val<0 && op_dest->val>0 && result<0) || (op_src->val>0 && op_dest->val<0 && result>0);

	OPERAND_W(op_dest , result); 
	print_asm_template2(); 
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)


#include "cpu/exec/template-end.h"
