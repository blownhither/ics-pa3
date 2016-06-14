#include "cpu/exec/helper.h"

uint32_t pio_read(ioaddr_t, size_t);

/*
#define DATA_BYTE 1
#include "in-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "in-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "in-template.h"
#undef DATA_BYTE


make_helper_v(in_i2a)
*/

//0xEC or 0xED
make_helper(in_b) {
	cpu.al = pio_read(cpu.dx, 1);
	print_asm("in %%al, %%dx");
	return 1;
}

make_helper(in_v) {
	if (ops_decoded.is_data_size_16) {
		cpu.ax = pio_read(cpu.dx, 2);
		print_asm("in %%ax, %%dx");
	}
	else { 
		cpu.eax = pio_read(cpu.dx, 4);
		print_asm("in %%eax, %%dx")	;
	}
	return 1;
}
