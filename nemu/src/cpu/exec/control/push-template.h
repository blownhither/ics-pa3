//#include "cpu/exec/template-start.h"

#define instr push

/*
static void do_execute () {
	printf("op_src->val is %x" , opsrc->val); 
}
*/

//extern static inline uint32_t instr_fetch(swaddr_t , size_t); 
//stack address size is 32
make_helper(pushr){
	uint8_t reg_num = instr_fetch(eip , 1);
	reg_num -= 0x50; 
	strcpy(op_src->str , "push"); 
	print_asm(str(instr) str(SUFFIX)"%s\n" , op_src->str);		// include/cpu/exec/helper.h
	reg_l(R_ESP) -= 4; 
	swaddr_write(reg_l(R_ESP) , 4 , reg_l(reg_num));   
#ifdef MZYDEBUG
	printf("reg_num fetched is %x\n" , reg_num); 
#endif
	return 1; 
	
}

//#include "cpu/exec/template-end.h"
