//#include "cpu/exec/template-start.h"

//`#define instr push

/*
static void do_execute () {
	printf("op_src->val is %x" , opsrc->val); 
}
*/
#define MZYDEBUG
make_helper(pushr){
#ifdef MZYDEBUG
	printf("op_src->val is %x" , op_src->type);
	return 1; 
	
#endif
}

//#include "cpu/exec/template-end.h"
