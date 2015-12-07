#include "cpu/exec/template-start.h"

#define instr lea

static void do_execute() {
	//ModR_M temp;
	printf("src is %x,dest is%x, op_src->str is%s",op_src->val,op_dest->val,op_src->str);
	//temp.val = instr_fetch(eip+1,1);
	//reg_l(temp.reg) = op_src->addr;
	//printf_asm("leal%s,%%%s",op_src->str,regsl[temp.reg] );
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
