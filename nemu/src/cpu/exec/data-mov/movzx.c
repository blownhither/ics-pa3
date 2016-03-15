#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
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
	ModR_M m;
	m.val = instr_fetch(eip, 1);
	Log("ModR/M %x\n",m.val);

	//TODO: m.mod == 3?

	if(ops_decoded.is_data_size_16){	//r/m8 -> r16
		panic("a");
	}
	else {								//r/m8 -> r32
		op_src->type = OP_TYPE_REG;
		op_src->reg = m.reg;
		if(m.mod == 3) {
			op_src->reg = m.R_M;
			op_src->val = reg_b(m.R_M);
		}
		else {
			panic("a");
		}
	}
	return 1;
}

