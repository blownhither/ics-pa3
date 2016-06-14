#include "cpu/exec/helper.h"

make_helper(sti) {
	eflags.eflags.IF = 1;
	print_asm("sti");
	return 1;
}
