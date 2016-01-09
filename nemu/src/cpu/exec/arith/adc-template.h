#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute(){
	DATA_TYPE ans = op_dest->val + op_src->val + eflags.eflags.CF;
	OPERAND_W(op_dest,ans);
	eflags.eflags.ZF = !ans;
	eflags.eflags.SF = MSB(ans);
	eflags.eflags.OF = (MSB(ans)!=MSB(op_dest->val)) && (MSB(ans)!=MSB(op_src->val));
	extern bool parity_check(uint32_t ); 
	eflags.eflags.PF = parity_check(ans);
	eflags.eflags.CF = (unsigned)op_src->val > (unsigned)ans;
	print_asm_template2(); 
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"
