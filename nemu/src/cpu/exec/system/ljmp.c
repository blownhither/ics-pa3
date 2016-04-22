#include "cpu/exec/helper.h"
#include "monitor/monitor.h"
make_helper(ljmp){	//0xEA
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
		load_desc_cache(CS_NUM);
		printf("CS base:0x%llx, limit:0x%x\n",(long long)cpu.desc_cache[CS_NUM].base,cpu.desc_cache[CS_NUM].limit);
		print_asm("ljmp $0x%x, $0x%x",cpu.cs, cpu.eip);
	}
	//TODO: check
	return 0;
}
