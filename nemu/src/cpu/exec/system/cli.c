#include "cpu/exec/helper.h"

make_helper(cli) {
	eflags.eflags.IF = 0;
	print_asm("cli");
	return 1;
}
