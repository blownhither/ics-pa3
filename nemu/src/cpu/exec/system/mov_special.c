#include "cpu/exec/helper.h"
#include "monitor/monitor.h"

make_helper(mov_cr2r){	//0F 20
	assert(cpu.cr0.PE == 0);// || eflags.VM == 1);
	uint8_t op = instr_fetch(cpu.eip+2, 1);
	//op = 0x11 + SPE_REG + REG
	uint8_t reg = op & 7;
	uint8_t spe_reg = (op>>3) & 7;
	assert(spe_reg == 0);
	cpu.gpr[reg]._32 = cpu.cr0.val;
	print_asm("mov %%cr0, %s", regsl[reg]);
	return 2;
}

make_helper(mov_r2cr){	//0F 22
	assert(cpu.cr0.PE == 0);// || eflags.VM == 1);
	uint8_t op = instr_fetch(cpu.eip+2, 1);
	//op = 0x11 + SPE_REG + REG
	uint8_t reg = op & 7;
	uint8_t spe_reg = (op>>3) & 7;
	assert(spe_reg == 0);
	cpu.cr0.val = cpu.gpr[reg]._32;
	print_asm("mov %s, %%cr0", regsl[reg]);
	return 2;
}

//The segment register bit assignments are ES=0,
//CS=1, SS=2, DS=3, FS=4, and GS=5.
make_helper(mov_sr2r){		//8C
	return 2;
}

make_helper(mov_r2sr){		//8E
	//decode_r_l(cpu.eip + 1);
	uint8_t op = instr_fetch(cpu.eip + 1, 1);
	uint8_t sr = (op >> 3) & 7;
	uint8_t r = op & 7;
	printf("%d %d\n",sr,r);
	cpu.sgr_val[sr] = cpu.gpr[r]._32;
	print_asm("mov %%%s, %%%s", regsl[r], sregs[sr]);
	return 2;
}
