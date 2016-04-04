#include "cpu/exec/template-start.h"

#define instr shld

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute () {
	DATA_TYPE in = op_dest->val;
	DATA_TYPE out = op_src2->val;
	uint8_t count = op_src->val;
	count &= 0x1f;
	while(count != 0) {
		out <<= 1;
		out |= (in>>((DATA_BYTE<<3)-1)) & 1;
		in <<= 1;
		count --;
	}

	OPERAND_W(op_src2, out);

	print_asm("shld" str(SUFFIX) " %s,%s,%s", op_src->str, op_dest->str, op_src2->str);
}

//make_instr_helper(si_rm2r)


make_helper(concat(shldi_, SUFFIX)) {	//0F AC SHRD r/m_v,r_v,imm8
	int len = concat(decode_si_rm2r_, SUFFIX) (eip + 1);  /* use decode_si_rm2r to read 1 byte immediate */
	op_dest->val = REG(op_dest->reg);
	do_execute();
	return len + 1;
}

make_helper(concat(shldc_, SUFFIX)) {	//0F AD SHRD r/m_v,r3_v,CL
	int len = concat(decode_si_rm2r_, SUFFIX) (eip + 1);  /* use decode_si_rm2r to read 1 byte immediate */
	op_dest->val = REG(op_dest->reg);
	op_src->val = cpu.cl;
	do_execute();
	return len;
}

#endif
#include "cpu/exec/template-end.h"
