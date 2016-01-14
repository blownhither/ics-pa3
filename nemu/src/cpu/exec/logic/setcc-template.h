#include "cpu/exec/template-start.h"

#define instr setcc

static void do_execute () {
	
	uint8_t condition = instr_fetch(cpu.eip+1,1), ans;
	printf("%x",condition);
	switch(condition){
		case 0x90: ans = eflags.eflags.OF;break;
		case 0x91: ans = !eflags.eflags.OF;break;
		case 0x92: ans = eflags.eflags.CF;break;
		case 0x93: ans = !eflags.eflags.CF;break;
		case 0x94: ans = eflags.eflags.ZF;break;
		case 0x95: ans = !eflags.eflags.ZF;break;
		case 0x96: ans = (eflags.eflags.CF || eflags.eflags.ZF);break;
		case 0x97: ans = !(eflags.eflags.CF || eflags.eflags.ZF);break;
		case 0x98: ans = eflags.eflags.SF;break;
		case 0x99: ans = !eflags.eflags.SF;break;
		case 0x9a: ans = eflags.eflags.PF;break;
		case 0x9b: ans = !eflags.eflags.PF;break;
		case 0x9c: ans = (eflags.eflags.OF != eflags.eflags.SF);break;
		case 0x9d: ans = (eflags.eflags.OF == eflags.eflags.SF);break;
		case 0x9e: case 0x9f:ans = (eflags.eflags.ZF || eflags.eflags.SF!=eflags.eflags.OF);break;
		 //ans = !eflags.eflags.OF;break;
		default: panic("exception for setcc!\n");
		
	}
	OPERAND_W(op_dest,ans);
	/* There is no need to update EFLAGS, since no other instructions 
	 * in PA will test the flags updated by this instruction.
	 */

	print_asm_template2();
}

make_instr_helper(rm)
#include "cpu/exec/template-end.h"
