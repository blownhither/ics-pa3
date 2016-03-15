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
	uint8_t op = instr_fetch(eip+1, 1);
	Log("ModR/M %x\n",op);

	//TODO: m.mod == 3?

	if(ops_decoded.is_data_size_16){	//r/m8 -> r16
		
	}
	else {								//r/m7 -> r32
		op_src->type = OP_TYPE_REG;
		//op_src->reg = 
	}
	return 0;
}

