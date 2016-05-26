#include "cpu/exec/template-start.h"

#define instr lidt

static void do_execute(){
	printf("op_src->val=%x\teax=%x\top_dest->val=%x\n",op_src->val,cpu.eax,op_dest->val);
	printf("eip=%x\n",cpu.eip);
	
	cpu.gdtr.base = op_dest->val;
	if(ops_decoded.is_data_size_16){
		cpu.gdtr.base &= 0xffffff;
	}
	print_asm("lgdt %x",op_dest->val);
}

make_instr_helper(rm_imm)

#undef instr
#define instr lgdt

static void do_execute(){
	printf("op_src->val=%x\teax=%x\top_dest->val=%x\n",op_src->val,cpu.eax,op_dest->val);
	printf("eip=%x\n",cpu.eip);
}

make_instr_helper(rm_imm)

#undef instr
#include "cpu/exec/template-end.h"
