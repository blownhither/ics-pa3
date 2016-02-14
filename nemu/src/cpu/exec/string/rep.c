#include "cpu/exec/helper.h"

make_helper(exec);

make_helper(rep) {
	int len;
	int count = 0;
	bool repne_flag = (instr_fetch(eip,1)==0xf2);
	if(instr_fetch(eip + 1, 1) == 0xc3) {
		/* repz ret */
		exec(eip + 1);
		len = 0;
	}
	else {
		while(cpu.ecx) {
			exec(eip + 1);
			count ++;
			cpu.ecx --;
	/*		assert(ops_decoded.opcode == 0xa4	// movsb
				|| ops_decoded.opcode == 0xa5	// movsw
				|| ops_decoded.opcode == 0xaa	// stosb
				|| ops_decoded.opcode == 0xab	// stosw
				|| ops_decoded.opcode == 0xa6	// cmpsb
				|| ops_decoded.opcode == 0xa7	// cmpsw
				|| ops_decoded.opcode == 0xae	// scasb
				|| ops_decoded.opcode == 0xaf	// scasw
				);*/

			/* TODO: Jump out of the while loop if necessary. */
			
			//REPE
			if(!repne_flag && !eflags.eflags.ZF && 
				(  ops_decoded.opcode == 0xa6 
				|| ops_decoded.opcode == 0xa7 
				|| ops_decoded.opcode == 0xae 
				|| ops_decoded.opcode == 0xaf )
			) break;
				
			//REPNE
			if(repne_flag && eflags.eflags.ZF && 
				(  ops_decoded.opcode == 0xa6 
				|| ops_decoded.opcode == 0xa7 
				|| ops_decoded.opcode == 0xae 
				|| ops_decoded.opcode == 0xaf )
			) break;
		}
		len = 1;
	}

#ifdef DEBUG
	char temp[80];
	sprintf(temp, "rep %s", assembly);
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif
	
	return len + 1;
}
