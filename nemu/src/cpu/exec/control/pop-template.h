#include "cpu/exec/template-start.h"

#define instr pop


static void do_execute () {
	OPERAND_W(op_src,MEM_R(cpu.esp));
//#if DATA_BYTE == 2
	cpu.esp += DATA_BYTE;
//#elif DATA_BYTE == 4
//	cpu.esp += 4;
//#endif
	
	print_asm_template1();
}


//extern static inline uint32_t instr_fetch(swaddr_t , size_t); 
//stack address size is 32
/*
make_helper(pushr){
	uint8_t reg_num = instr_fetch(eip , 1);
	reg_num -= 0x50; 
	strcpy(op_src->str , "push"); 
	print_asm(str(instr)  "l $%s\n" , regsl[check_reg_index(reg_num)]);		// include/cpu/exec/helper.h
	//using print_asm to print instr info
	reg_l(R_ESP) -= 4; 
	swaddr_write(reg_l(R_ESP) , 4 , reg_l(reg_num));   
#ifdef MZYDEBUG
	printf("reg_num fetched is %x\n" , reg_num); 
#endif
	return 1; 
	
}
*/
make_instr_helper(r)
//make_instr_helper(m)
#include "cpu/exec/template-end.h"
