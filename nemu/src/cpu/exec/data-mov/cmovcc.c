#include "cpu/exec/helper.h"

#define ef eflags.eflags
inline bool check_cmovcc_condition(uint8_t op) {
	//Log("%x",op);	
	switch(op) {
		case 0x40:	return (ef.OF);
		case 0x41:	return (!ef.OF);
		case 0x42:	return (ef.CF);
		case 0x43:	return (!ef.CF);
		case 0x44: 	return (ef.ZF);
		case 0x45:	return (!ef.ZF);
		case 0x46:	return (ef.CF || ef.ZF);
		case 0x47:	return (!ef.CF && !ef.ZF);
		case 0x48:	return (ef.SF);
		case 0x49:	return (!ef.SF);
		case 0x4a:	return (ef.PF);
		case 0x4b:	return (!ef.PF);
		case 0x4c:	return (ef.SF != ef.OF);
		case 0x4d:	return (ef.SF == ef.OF);
		case 0x4e:	return (ef.ZF || ef.SF!=ef.OF);
		case 0x4f:	return (!ef.ZF && ef.SF==ef.OF);
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

