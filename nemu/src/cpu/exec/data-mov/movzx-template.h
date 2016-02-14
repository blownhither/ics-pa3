#include "cpu/exec/template-start.h"

#define instr movzbl
#if DATA_BYTE==4
static void do_execute() {
	//uint32_t ans = (op_src & (uint32_t)0xff);
	OPERAND_W(op_dest, op_src->val & 0xff);
	print_asm_template2();
}
make_instr_helper(rm2r)
#endif
#undef instr

#define instr movzwl
#if DATA_BYTE==4
static void do_execute() {
	OPERAND_W(op_dest, op_src->val & 0xffff);
	print_asm_template2();
}
make_instr_helper(rm2r)
#endif 
#undef instr

#include "cpu/exec/template-end.h"
