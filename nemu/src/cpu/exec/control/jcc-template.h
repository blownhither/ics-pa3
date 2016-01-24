#include "cpu/exec/template-start.h"

#define JCC_ROUTINE DATA_TYPE_S offs = instr_fetch(cpu.eip+1,DATA_BYTE);\
                    print_asm(str(instr)" 0x%x <offs=0x%x>",cpu.eip+offs+DATA_BYTE+1,offs);

#define JCC_CONDITION(reg) JCC_ROUTINE\
                            if(reg==1)cpu.eip += offs + DATA_BYTE + 1;\
                            return 0;


//make_helper(jo_si_v); //0x0f 80
//make_helper(jno_si_v); //0x0f 81
//make_helper(jb_si_v); //0x0f 82
//make_helper(jnb_si_v); //0x0f 83
//make_helper(je_si_v); //0x0f 84
#define instr je
make_helper(concat(je_si_,SUFFIX)){
	//DATA_TYPE_S offs = instr_fetch(cpu.eip+1,DATA_BYTE);
    //JCC_ROUTINE;
 	//if(eflags.eflags.ZF)cpu.eip += offs + DATA_BYTE + 1;    
	//print_asm(str(instr)" 0x%x <offs=0x%x>\n",cpu.eip+offs+DATA_BYTE+1,offs);
	//return 0;
    JCC_CONDITION(eflags.eflags.ZF);
}
//make_helper(jne_si_v); //0x0f 85
#undef instr
#define instr jne
make_helper(concat(jne_si_,SUFFIX)){
	printf("jne %x\n",op_src->val);
    if(!eflags.eflags.ZF)cpu.eip += op_src->val + DATA_BYTE + 1;    
	//print_asm("jmp 0x%x <offset = 0x%x\n>",cpu.eip+1+DATA_BYTE,op_src->val);
return 0;
}
#undef instr

//make_helper(jna_si_v); //0x0f 86
//make_helper(ja_si_v); //0x0f 87
//make_helper(js_si_v); //0x0f 88
//make_helper(jns_si_v); //0x0f 89
//make_helper(jp_si_v); //0x0f 8a
//make_helper(jpo_si_v); //0x0f 8b
//make_helper(jl_si_v); //0x0f 8c
//make_helper(jge_si_v); //0x0f 8d
//make_helper(jle_si_v); //0x0f 8e
//make_helper(jnle_si_v); //0x0f 8f
//make_instr_helper(si);

#include "cpu/exec/template-end.h"

