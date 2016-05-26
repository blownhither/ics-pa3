/*

#include "cpu/exec/helper.h"
#include "monitor/monitor.h"
make_helper(lgdt){	//0F 01 /2
	lnaddr_t addr = instr_fetch(cpu.eip+3, 4);
	//printf("lgdt: addr=0x%x\n",addr);
	cpu.gdtr.limit = lnaddr_read(addr, 2);
	cpu.gdtr.base = lnaddr_read(addr + 2, 4);
	if(ops_decoded.is_data_size_16){
		cpu.gdtr.base &= 0xffffff;
	}
	print_asm("lgdt %x",addr);
	return 6;	//TODO: always 6?
}

*/


#include "cpu/exec/helper.h"
 /*
#define DATA_BYTE 1
#include "ldt-template.h"
#undef DATA_BYTE
*/
#define DATA_BYTE 2
#include "ldt-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "ldt-template.h"
#undef DATA_BYTE

make_helper_v(lidt_rm_imm)
make_helper_v(lgdt_rm_imm)
