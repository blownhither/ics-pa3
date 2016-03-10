#include "cpu/exec/template-start.h"

#define instr test

extern bool parity_check(uint32_t );
static void do_execute () {
	eflags.eflags.CF = eflags.eflags.OF = 0;
	DATA_TYPE ans = op_src->val & op_dest->val;
	eflags.eflags.ZF = !ans;
	eflags.eflags.PF = parity_check(ans);
	eflags.eflags.SF = MSB(ans);
	//Log("test:	0x%x & 0x%x = 0x%x\n",op_src->val, op_dest->val, ans);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"
