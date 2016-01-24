#include "cpu/exec/template-start.h"

#define JCC_ROUTINE DATA_TYPE_S offs = instr_fetch(cpu.eip+1,DATA_BYTE);\
                    print_asm(str(instr)" 0x%x <offs=0x%x>",cpu.eip+offs+DATA_BYTE+1,offs);

#define JCC_CONDITION(reg) JCC_ROUTINE\
                            if((reg)==1)cpu.eip += offs;\
                            return DATA_BYTE + 1;

#define JCC_MAKER(reg1) make_helper(concat3(instr,_si_,SUFFIX)){\
                           JCC_CONDITION(reg1);\
                          }

/*example of JCC_MAKER on je*/
/*make_helper(concat(je_si_,SUFFIX)){
	//DATA_TYPE_S offs = instr_fetch(cpu.eip+1,DATA_BYTE);
 	//if(eflags.eflags.ZF)cpu.eip += offs + DATA_BYTE + 1;    
	//print_asm(str(instr)" 0x%x <offs=0x%x>\n",cpu.eip+offs+DATA_BYTE+1,offs);
	//return 0;
    JCC_CONDITION(eflags.eflags.ZF);
}*/

#ifdef instr 
#undef instr
#endif
#define instr jo  //0x0f 80
JCC_MAKER(eflags.eflags.OF)
#undef instr

#define instr jno //0x0f 81
JCC_MAKER(!eflags.eflags.OF)
#undef instr

#define instr jb  //0x0f 82
JCC_MAKER(eflags.eflags.CF)
#undef instr

#define instr jnb //0x0f 83
JCC_MAKER(!eflags.eflags.CF)
#undef instr

#define instr je //0x0f 84
JCC_MAKER(eflags.eflags.ZF)
#undef instr

#define instr jne //0x0f 85
JCC_MAKER(!eflags.eflags.ZF)
#undef instr

#define instr jna //0x0f 86
//CF || ZF !!
JCC_MAKER(eflags.eflags.CF || eflags.eflags.ZF)
#undef instr

#define instr ja //0x0f 87
JCC_MAKER(!eflags.eflags.CF && !eflags.eflags.ZF)
#undef instr

#define instr js //0x0f 88
JCC_MAKER(eflags.eflags.SF)
#undef instr

#define instr jns //0x0f 89
JCC_MAKER(!eflags.eflags.SF)
#undef instr

#define instr jp //0x0f 8a
JCC_MAKER(eflags.eflags.PF)
#undef instr

#define instr jpo //0x0f 8b
JCC_MAKER(!eflags.eflags.PF)
#undef instr

#define instr jl //0x0f 8c
JCC_MAKER(eflags.eflags.SF != eflags.eflags.OF)
#undef instr

#define instr jge //0x0f 8d
JCC_MAKER(eflags.eflags.SF == eflags.eflags.OF)
#undef instr

#define instr jle //0x0f 8e
//ZF || SF!=OF
JCC_MAKER(eflags.eflags.ZF || (eflags.eflags.SF!=eflags.eflags.OF))
#undef instr

#define instr jg //0x0f 8f
JCC_MAKER(!eflags.eflags.ZF && (eflags.eflags.SF==eflags.eflags.OF))
#undef instr
//make_instr_helper(si);

#include "cpu/exec/template-end.h"

