#include "monitor/monitor.h"
#include "cpu/helper.h"
#include <setjmp.h>

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the ``si'' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10

int nemu_state = STOP;
uint64_t L1_cache_miss , L1_cache_access, L2_cache_miss , L2_cache_access, L1_cache_miss, L1_cache_access;

int exec(swaddr_t);
extern uint64_t L1_get_cache_cost ();
extern uint64_t L2_get_cache_cost ();
extern bool check_watchpoints(); 
extern void i8259_ack_intr();
extern uint8_t i8259_query_intr();

char assembly[80];
char asm_buf[128];

/* Used with exception handling. */
jmp_buf jbuf;

void print_bin_instr(swaddr_t eip, int len) {
	int i;
	int l = sprintf(asm_buf, "%8x:   ", eip);
	for(i = 0; i < len; i ++) {
		l += sprintf(asm_buf + l, "%02x ", instr_fetch(eip + i, 1));
	}
	sprintf(asm_buf + l, "%*.s", 50 - (12 + 3 * len), "");
}

/* This function will be called when an `int3' instruction is being executed. */
void do_int3() {
	printf("\nHit breakpoint at eip = 0x%08x\n", cpu.eip);
	nemu_state = STOP;
}

/* Simulate how the CPU works. */

uint64_t si_count = 0;

void cpu_exec(volatile uint32_t n) {
	if(nemu_state == END) {
		printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
		return;
	}
	nemu_state = RUNNING;

#ifdef DEBUG
	volatile uint32_t n_temp = n;
#endif

	setjmp(jbuf);

	for(; n > 0; n --) {
#ifdef DEBUG
		swaddr_t eip_temp = cpu.eip;
		if((n & 0xffff) == 0) {
			/* Output some dots while executing the program. */
			fputc('.', stderr);
		}
#endif
		/* Execute one instruction, including instruction fetch,
		 *  instruction decode, and the actual execution. */
		int instr_len = exec(cpu.eip);
		//printf("instr_len is %x in cpu-exec.c\n",instr_len);
		cpu.eip += instr_len;
		si_count++;

#ifdef DEBUG
		
#endif

		print_bin_instr(eip_temp, instr_len);
		strcat(asm_buf, assembly);
		Log_write("%s\n", asm_buf);
		if(n_temp < MAX_INSTR_TO_PRINT) {
			printf("%s\n", asm_buf);
		} 

		

		/* TODO: check watchpoints here. */
		if(check_watchpoints()){
			n=1;
			nemu_state = STOP; 
		} 

		if(nemu_state != RUNNING) { 
			printf("Cache cost = %llu, L1 miss rate = %f, L2 miss rate = %f\n",(unsigned long long)(L2_get_cache_cost()+L1_get_cache_cost()), (double)L1_cache_miss/(double)L1_cache_access,(double)L2_cache_miss/(double)L2_cache_access);
			printf("Instruction executed: %llu\n",(long long unsigned)si_count);
			return; 
		}
		//printf("cpu.eip is %x in rear cpu-exec.c\n",cpu.eip);
		
		//PA 4.4
		if(cpu.INTR & eflags.eflags.IF) {
			//printf("cpu.INTR triggered\n");
			uint32_t intr_no = i8259_query_intr();
			i8259_ack_intr();
			int len = get_instr_len();
			cpu.eip --;	//TODO: ?!
			raise_intr(intr_no, len);
		}
	}

	if(nemu_state == RUNNING) { nemu_state = STOP; }
}
