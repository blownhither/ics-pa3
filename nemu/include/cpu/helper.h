#ifndef __HELPER_H__
#define __HELPER_H__

#include "nemu.h"
#include "cpu/decode/operand.h"

/* All function defined with 'make_helper' return the length of the operation. */
#define make_helper(name) int name(swaddr_t eip)

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t cur_segr);

void raise_intr(uint8_t NO);

static inline uint32_t instr_fetch(swaddr_t addr, size_t len) {
	lnaddr_t lnaddr;
	if(cpu.cr0.PE){
		lnaddr = seg_translate(addr, len, CS_NUM);
	}
	else 
		lnaddr = addr;
	return lnaddr_read(lnaddr, len);
	//return swaddr_read(addr, len);
}

static int len;

/* Instruction Decode and EXecute */
static inline int idex(swaddr_t eip, int (*decode)(swaddr_t), void (*execute) (void)) {
	/* eip is pointing to the opcode */
	//int len = decode(eip + 1);
	len = decode(eip + 1);
	execute();
	return len + 1;	// "1" for opcode
}

/* Thanks to akuxcw for inspiratoin!*/
static inline int get_instr_len(){
	return len;
}

static inline void push_stack_32(uint32_t val){
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, val);
}

static inline uint32_t pop_stack_32(){
	cpu.esp += 4;
	return swaddr_read(cpu.esp-4, 4);
}

/* shared by all helper function */
extern Operands ops_decoded;

#define op_src (&ops_decoded.src)
#define op_src2 (&ops_decoded.src2)
#define op_dest (&ops_decoded.dest)


#endif
