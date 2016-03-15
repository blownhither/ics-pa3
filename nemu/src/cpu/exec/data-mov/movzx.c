#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
//#include "cpu/exec/template-start.h"
/*
#define DATA_BYTE 1
#include "movzx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movzx-template.h"
#undef DATA_BYTE
*/

#define DATA_BYTE 4
#include "movzx-template.h"
#undef DATA_BYTE

make_helper(movzxl) {
/*
	ModR_M m;
	m.val = instr_fetch(eip+1, 1);
	Log("ModR/M %x\n",m.val);
*/
/*
	if(ops_decoded.is_data_size_16){	//r/m8 -> r16
		panic("a");
	}
	else {	
*/							//r/m8 -> r32
	int len = 0;
	if(((instr_fetch(eip+1,1)>>6)&3) == 3)op_src->size = 1;
	else op_src->size = 4, len++;
	op_dest->size = 4;
	Log("eip=%x",eip+1);
	len += read_ModR_M(eip+1, op_src, op_dest);
	
	op_dest->val = reg_b(op_dest->reg);
	Log("reg=0x%x,val=0x%x",op_dest->reg,op_dest->val);
	print_asm_template2();
	return len;
}


