#include "cpu/exec/template-start.h"

#define instr setcc

static void do_execute () {
	
	printf("%x\n%x\n",op_src->reg,op_dest->val);

	/* There is no need to update EFLAGS, since no other instructions 
	 * in PA will test the flags updated by this instruction.
	 */

//	print_asm_template2();
}

make_instr_helper(r2rm)
#include "cpu/exec/template-end.h"
