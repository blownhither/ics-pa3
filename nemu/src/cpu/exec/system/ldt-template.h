#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute(){
/*
	printf("op_src->addr=%x\teax=%x\top_dest->val=%x\n",op_src->addr,cpu.eax,op_dest->val);
	printf("eip=%x\n",cpu.eip);
*/
	cpu.gdtr.limit = swaddr_read(op_src->addr, 2);
	cpu.gdtr.base = swaddr_read(op_src->addr + 2, 4);
	if(ops_decoded.is_data_size_16){
		cpu.gdtr.base &= 0xffffff;
	}
	print_asm("lgdt %x", op_src->addr);
	
}

make_instr_helper(rm)

#undef instr
#define instr lidt

static void do_execute(){
	cpu.idtr.limit = swaddr_read(op_src->addr, 2);
	cpu.idtr.base = swaddr_read(op_src->addr + 2, 4);
	if(ops_decoded.is_data_size_16){
		cpu.idtr.base &= 0xffffff;
	}
	print_asm("lidt %x", op_src->addr);
} 

make_instr_helper(rm)

#undef instr
#include "cpu/exec/template-end.h"
