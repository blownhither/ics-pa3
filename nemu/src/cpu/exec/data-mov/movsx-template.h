#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	uint32_t ans = (uint32_t)(DATA_TYPE_S)op_src->val;
	OPERAND_W(op_dest, ans);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
