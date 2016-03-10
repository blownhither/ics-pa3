#include "cpu/exec/template-start.h"

//relative jump
#define instr jmpr	
static void do_execute(){
    cpu.eip += op_src->val;    
	print_asm("jmp 0x%x <offset = 0x%x\n>",cpu.eip+1+DATA_BYTE,op_src->val);
    return ;
}

make_instr_helper(si);
#undef instr

/*
//direct jump
#define instr jmp	
static void do_execute(){
    cpu.eip = op_src->val - op_src->size - 1 - (ops_decoded.is_data_size_16?1:0);	//TODO: check 2    

	Log("in jmp_rm, eip=0x%x\n",cpu.eip);

	print_asm_template1();
    return ;
}
make_instr_helper(rm);
#undef instr
*/

make_helper(concat(jmp_rm_, SUFFIX)) {
	Log("eip=0x%x to ",eip);
	concat(decode_rm_, SUFFIX)(eip);
	cpu.eip = op_src->val;
	Log("0x%x\n",cpu.eip);
	return 0;
}
/*
make_helper(jmp_rm_w) {
	Log("eip=0x%x to ",eip);
	decode_rm_w(eip);
	cpu.eip = op_src->val;
	Log("0x%x\n",cpu.eip);
	return 0;
}
*/

/*
#define make_instr_helper(type) \
	make_helper(concat5(instr, _, type, _, SUFFIX)) {
		return idex(eip, concat4(decode_, type, _, SUFFIX), do_execute);\
	}

#define do_execute concat4(do_, instr, _, SUFFIX)

idex: exec and return decoded_len+1
*/

#include "cpu/exec/template-end.h"

