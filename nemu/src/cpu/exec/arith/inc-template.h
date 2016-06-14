#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	eflags.eflags.ZF = !result;
	eflags.eflags.SF = MSB(result);
	eflags.eflags.OF = (op_src->val > result);
	extern bool parity_check(uint32_t ); 
	eflags.eflags.PF = parity_check(result);
	eflags.eflags.CF = (result==0);
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
