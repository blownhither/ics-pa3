#include "cpu/exec/helper.h"

#define ef eflags.eflags
inline bool check_cmovcc_condition(uint8_t op) {
	Log("%x",op);	
	switch(op) {
		case 0x44: return (ef.ZF);
		default: Log("cmovcc encounter unexpected opcode");return 0;
	}
}
#undef ef

/*
#define DATA_BYTE 1
#include "cmovcc-template.h"
#undef DATA_BYTE
*/
#define DATA_BYTE 2
#include "cmovcc-template.h"
#undef DATA_BYTE


#define DATA_BYTE 4
#include "cmovcc-template.h"
#undef DATA_BYTE

make_helper_v(cmovcc_rm2r)

