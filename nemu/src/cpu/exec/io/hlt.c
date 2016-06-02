#include "cpu/exec/helper.h"

make_helper(hlt) {
	if (!eflags.eflags.IF) {
		panic("eflags.IF disabled when execute HLT!");
	}
	while( !cpu.INTR ) ;
	print_asm("hlt");
	return 1;
}
