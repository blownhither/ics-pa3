#include "cpu/exec/template-start.h"

#define instr int

static void do_execute(){
	cpu.eip += 1;
	raise_intr(op_src->val);
	print_asm("int %x", op_src->val);
}

make_instr_helper(i)


#include "cpu/exec/template-end.h"
