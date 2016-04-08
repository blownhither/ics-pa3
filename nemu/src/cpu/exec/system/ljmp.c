#include "cpu/exec/helper.h"
#include "monitor/monitor.h"
make_helper(ljmp){	//0xEA
	//assert(cpu.cr0.PE == 1); // && eflags.eflags.VM == 0
	// #GP(0) or #SS(0)
	if(cpu.cr0.PE == 0){	//real mod
		panic("please implement ljmp");
	}
	else {	//protected
		//TODO:	#GP(0) 
		
		if(ops_decoded.is_data_size_16){
			cpu.cs = instr_fetch(cpu.eip+3, 2);
			cpu.eip = instr_fetch(cpu.eip+1, 2);
		}
		else {
			cpu.cs = instr_fetch(cpu.eip+5, 2);
			cpu.eip = instr_fetch(cpu.eip+1, 4);
		}
		print_asm("ljmp %x, %x",cpu.cs, cpu.eip);
	}
	//TODO: check
	return 0;
}
