#include "cpu/exec/template-start.h"

#define instr setcc

static void do_execute () {
	
	uint8_t condition = instr_fetch(cpu.eip-1,1);
	printf("%x\n",condition);
	/* There is no need to update EFLAGS, since no other instructions 
	 * in PA will test the flags updated by this instruction.
	 */

//	print_asm_template2();
}

make_instr_helper(rm)
#include "cpu/exec/template-end.h"
