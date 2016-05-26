#include "cpu/exec/helper.h"
 
#define DATA_BYTE 1
#include "int-template.h"
#undef DATA_BYTE
/*
#define DATA_BYTE 2
#include "int-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "int-template.h"
#undef DATA_BYTE
*/

/*Finding int3 under misc instructions*/
/*
make_helper(int3){
	cpu.eip += 1;
	raise_intr(3);
	return 0;
}
*/
make_helper(into){
	cpu.eip += 1;
	raise_intr(4);
	return 0;
}
