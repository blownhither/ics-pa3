#include "cpu/exec/template-start.h"

#define instr test

extern bool parity_check(uint32_t );
static void do_execute () {
	eflags.eflags.CF = eflags.eflags.OF = 0;
	eflags.eflags.ZF = !!op_src->val;
	eflags.eflags.PF = parity_check(op_src->val);
	eflags.eflags.SF = MSB(op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"
