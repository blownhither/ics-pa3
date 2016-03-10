#include "cpu/exec/template-start.h"

#define instr movzbl
#if DATA_BYTE==4
static void do_execute() {

	uint32_t ans = op_src->val & (uint32_t)0xff;
#ifdef MZYDEBUG
	Log("src=0x%x, ans=0x%x\n", op_src->val, ans);
#endif
	if(ops_decoded.is_data_size_16){
		ans |= (op_dest->val&0xffff0000); 
#ifdef MZYDEBUG
		Log("ans_16=0x%x\n",ans);
#endif
	}
	OPERAND_W(op_dest, ans);
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
