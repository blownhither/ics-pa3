#include "cpu/exec/template-start.h"

#define instr sub

/*
//TODO:make_instr_helper(???)
make_instr_helper(i2rm)
//0x83 is to sub imm8 from longer r/m
make_instr_helper(i2r)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
//others are to sub a length from same length
*/
//do_execute is do_instr_SUFFIX
//#define op_src (&ops_decoded.src)
//DATA_BYTE ,  DATA_TYPE is refering to latter operand(SRC)?
//and SUFFIX is referring to former opearand


static void do_execute(){
	DATA_TYPE a; 
	DATA_TYPE b = op_dest->val; 
//if a is byte and b is word/dword
//then b=b-(signextend)a
//else b = b - a
#if (DATA_BYTE == 4) && SUFFIX == b
	a = (DATA_TYPE)op_src->val | 0xffffff00; 
#elif (DATA_BYTE == 2) && SUFFIX == b
	a = (DATA_TYPE)op_src->val | 0xff00; 
#else
	a = op_src->val; 
#endif
	DATA_TYPE result = b-a;
	OPERAND_W(op_dest , result); 
	print_asm_template2(); 
	//return DATA_BYTE; //TODO
}
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif


#include "cpu/exec/template-end.h"
