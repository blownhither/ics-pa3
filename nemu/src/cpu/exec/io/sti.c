#include "cpu/exec/helper.h"

make_helper(sti) {
	eflags.eflags.IF = 1;
	return 1;
}
