#include "cpu/exec/template-start.h"

#define instr int

static void do_execute(){
	//cpu.eip += 1;	// no need to +1 in template
	print_asm("int %x", op_src->val);
	printf("int %d",op_src->val);
	raise_intr(op_src->val);
}

make_instr_helper(i)


#include "cpu/exec/template-end.h"
