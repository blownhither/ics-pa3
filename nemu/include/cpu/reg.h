#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../lib-common/x86-inc/cpu.h"
//cpu.h contains CR0 and CR3

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
	uint32_t CF:1; 
	uint32_t dont_care:1; 
	uint32_t PF:1; 
	uint32_t   :1; 
	uint32_t AF:1; 
	uint32_t   :1; 
	uint32_t ZF:1; 
	uint32_t SF:1; 
	uint32_t TF:1; 
	uint32_t IF:1; 
	uint32_t DF:1; 
	uint32_t OF:1; 
	uint32_t OL:1;
	uint32_t IP:1; 
	uint32_t NT:1; 
	uint32_t   :1; 
	uint32_t RF:1; 
	uint32_t VM:1;
	uint32_t   :6;
	uint32_t   :8; 
}EFLAGS_bit;
typedef union {
	EFLAGS_bit eflags;
	uint32_t eflags_l;  
}EFLAGS;
EFLAGS eflags; 


typedef struct {
	union{
		union {//each GPR includes one of the three tpyes defined below, merged together brings benefits
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];
		struct {
			uint32_t eax;
			uint32_t ecx;
			uint32_t edx;
			uint32_t ebx;
			uint32_t esp;
			uint32_t ebp;
			uint32_t esi;
			uint32_t edi;
		};
	};
	/* Do NOT change the order of the GPRs' definitions. */

	//uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
//#define eax gpr[0]._32
#define ax  gpr[0]._16
#define al  gpr[0]._8[0] 
//#define ecx gpr[1]._32
#define cl	gpr[1]._8[0]
/*
#define edx gpr[2]._32
#define ebx gpr[3]._32
#define esp gpr[4]._32
#define ebp gpr[5]._32
#define esi gpr[6]._32
#define edi gpr[7]._32
*/
	swaddr_t eip;
	struct {
		uint32_t base;
		uint16_t limit;
	} gdtr;	//Global Descriptor
	
	CR0 cr0;
	CR3 cr3;
	
	union{
		union{
			struct {
				uint16_t index	:13;
				uint8_t ti		:1;
				uint8_t rpl		:2;
			} sgr[6];
			uint16_t sgr_val[6];
		};
		struct {
			uint16_t es;	//0
			uint16_t cs;
			uint16_t fs;
			uint16_t ds;
			uint16_t ss;	//unnecessary
			uint16_t gs;
		};
	};	//segment registers

} CPU_state;
extern CPU_state cpu;
enum{EF_CF=0 , EF_PF=2 , EF_AF=4 , EF_ZF=6 , EF_SF , EF_TF , EF_IF , EF_DF , EF_OF , EF_OL , EF_IP , EF_NT , EF_RF=16 , EF_VM=17}; 


static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char* sregs[];

#endif
