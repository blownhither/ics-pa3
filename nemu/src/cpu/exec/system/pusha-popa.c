#include "cpu/exec/helper.h"

make_helper(pusha) {	//PUSHAD
	uint32_t temp = cpu.esp;
	uint32_t list[] = {cpu.eax, cpu.ecx, cpu.edx, cpu.ebx, temp, cpu.ebp, cpu.esi, cpu.edi};
	int i;
	const int n = sizeof(list) / sizeof(uint32_t);
	for(i=0; i<n; i++){
		push_stack_32(list[i]);
	}
	return 1;
}

make_helper(popa) {		//POPAD
	cpu.edi = pop_stack_32();
	cpu.esi = pop_stack_32();
	cpu.ebp = pop_stack_32();
	pop_stack_32();
	cpu.ebx = pop_stack_32();
	cpu.edx = pop_stack_32();
	cpu.ecx = pop_stack_32();
	cpu.eax = pop_stack_32();
	return 1;
}
