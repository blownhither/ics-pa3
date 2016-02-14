#ifdef DATA_BYTE
#undef DATA_BYTE
#endif
#define DATA_BYTE 4
#include "cpu/exec/template-start.h"

make_helper(stc) {
	eflags.eflags.CF = 1;
	return 1;
}
make_helper(clc) {
	eflags.eflags.CF = 0;
	return 1;
}
make_helper(cmc) {
	eflags.eflags.CF = !eflags.eflags.CF;
	return 1;
}
make_helper(cld) {
	eflags.eflags.DF = 0;
	return 1;
}
make_helper(std) {
	eflags.eflags.DF = 1;
	return 1;
}
#include "cpu/exec/template-end.h"
/*
(Set Carry Flag)
(Clear Carry Flag)
(Complement Carry Flag)
(Clear Direction Flag)
(Set Direction Flag)
*/
