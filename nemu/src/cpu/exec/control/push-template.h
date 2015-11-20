//#include "cpu/exec/template-start.h"

//`#define instr push

/*
static void do_execute () {
	printf("op_src->val is %x" , opsrc->val); 
}
*/
#define MZYDEBUG

//extern static inline uint32_t instr_fetch(swaddr_t , size_t); 
//stack address size is 32
make_helper(pushr){
	uint8_t reg_num = instr_fetch(eip , 1); 
#ifdef MZYDEBUGi
	reg_num -= 0x80; 
	printf("reg_num fetched is %x\n" , reg_num);
#endif
	printf("%x:\t\tpush %s" , eip , regsl[check_reg_index(reg_num)]); 
	reg_l(R_ESP) += 4; //TODO += ?!?
	swaddr_write(reg_l(R_ESP) , 4 , reg_l(reg_num));   
	return 1; 
	
}

//#include "cpu/exec/template-end.h"
