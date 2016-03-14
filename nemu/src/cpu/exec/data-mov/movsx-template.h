#include "cpu/exec/template-start.h"
/*
#define instr movsx

static void do_execute() {
	uint32_t ans = (uint32_t)(DATA_TYPE_S)op_src->val;
	OPERAND_W(op_dest, ans);
	print_asm_template2();
}
*/

//make_instr_helper(rm_b2r)
//make_instr_helper(rm_w2r)

#define instr movsbl
#if DATA_BYTE==4
static void do_execute() {
	uint32_t ans = (op_src->val & (uint32_t)0xff);
	ans |= ((op_src->val >> 7)&(uint32_t)1) ? 0xffffff00 : 0;
	//printf("s%d, d%d\n",(int)op_src->size, (int)op_dest->size);
	//TODO: check necessity
	if(ops_decoded.is_data_size_16)
		ans = (op_dest->val&0xffff0000) | (ans&0xffff);	//upper bits remain
#ifdef MZYDEBUG
	printf("src:%x, key:%x, ans:%x, dest:%x\n",op_src->val,(op_src->val>>7)&(uint32_t)1,ans,op_dest->val);
#endif
	op_src->size = 1;
	OPERAND_W(op_dest, ans);
	print_asm_template2();
}
make_instr_helper(rm2r)
#endif
#undef instr

#define instr movswl
#if DATA_BYTE==4
static void do_execute() {
	uint32_t ans = (op_src->val & (uint32_t)0xffff);
	ans |= ((op_src->val >> 15)&(uint32_t)1) ? 0xffff0000 : 0;
//	printf("src:%x, ans:%x, dest:%x\n",op_src->val,ans,op_dest->val);
	op_src->size = 2;
	OPERAND_W(op_dest, ans);
	print_asm_template2();
}
make_instr_helper(rm2r)
#endif 
#undef instr

#include "cpu/exec/template-end.h"
