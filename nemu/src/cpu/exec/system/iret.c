#include "cpu/exec/helper.h"

make_helper(iret) {
	cpu.eip = pop_stack_32();
	cpu.cs = pop_stack_32();
	eflags.eflags_l = pop_stack_32();
	print_asm("iret"); 
	return 1;
}
