#include "cpu/exec/helper.h"
#include "monitor/monitor.h"
make_helper(mov_cr2r){	//0F 20
	uint8_t op = instr_fetch(cpu.eip+2, 1);
	//op = 0x11 + SPE_REG + REG
	uint8_t reg = op & 7;
	uint8_t spe_reg = (op>>3) & 7;
	assert(spe_reg == 0);
	cpu.cr0.val = cpu.gpr[reg]._32;
	print_asm("mov cr0, %s", regsl[reg]);
	return 2;
}
make_helper(mov_r2cr){	//0F 22
	uint8_t op = instr_fetch(cpu.eip+2, 1);
	//op = 0x11 + SPE_REG + REG
	uint8_t reg = op & 7;
	uint8_t spe_reg = (op>>3) & 7;
	assert(spe_reg == 0);
	cpu.gpr[reg]._32 = cpu.cr0.val;
	print_asm("mov %s, cr0", regsl[reg]);
	return 2;
}
