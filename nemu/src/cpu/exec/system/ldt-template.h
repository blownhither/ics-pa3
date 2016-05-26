#include "cpu/exec/template-start.h"

#define instr lidt

static void do_execute(){
	printf("op_src->val=%x\teax=%x\top_dest->val=%x\n",op_src->val,cpu.eax,op_dest->val);
	printf("eip=%x\n",cpu.eip);
}

make_instr_helper(rm)

#undef instr
#define instr lgdt

static void do_execute(){
	printf("op_src->val=%x\teax=%x\top_dest->val=%x\n",op_src->val,cpu.eax,op_dest->val);
	printf("eip=%x\n",cpu.eip);
}

make_instr_helper(rm)

#undef instr
#include "cpu/exec/template-end.h"
