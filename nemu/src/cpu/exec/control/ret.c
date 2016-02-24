#include "cpu/exec/helper.h"

#ifdef DATA_BYTE
#undef DATA_BYTE
#endif
#define DATA_BYTE 4

#include "cpu/exec/template-start.h"
make_helper(ret){
	cpu.eip = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
#ifdef MZYDEBUG
	printf("ret: eip=0x%x(esp=0x%x)\n",cpu.eip,cpu.esp);
#endif
	print_asm("ret");
	return 0;
}
make_helper(retf) {
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp += 4;
	uint16_t CS = MEM_R(cpu.esp) & 0xffff;
	cpu.esp += 4;
	print_asm("ret far (CS offs:%x)",CS);
	return 0;
}
make_helper(ret_i) {
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp += 4;
	int16_t imm = instr_fetch(eip,2);
	cpu.esp += imm;
	print_asm("ret offs:%x",imm);
	return 0;
}
make_helper(retf_i) {
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp += 4;
	uint16_t CS = MEM_R(cpu.esp) & 0xffff;
	cpu.esp += 4;
	int16_t imm = instr_fetch(eip,2);
	cpu.esp += imm;
	print_asm("ret far offs:%x(CS offs:%x)",imm,CS);
	return 0;
}
#include "cpu/exec/template-end.h"
