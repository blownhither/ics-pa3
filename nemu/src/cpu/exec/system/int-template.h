#include "cpu/exec/template-start.h"

#define instr int

static void do_execute(){
	//cpu.eip += 2;			// keep the value of eip, but in raise_intr must +2?
	print_asm("int %x", op_src->val);
	//printf("int %d\n",op_src->val);
	raise_intr(op_src->val, 2);
}

make_instr_helper(i)


#include "cpu/exec/template-end.h"
