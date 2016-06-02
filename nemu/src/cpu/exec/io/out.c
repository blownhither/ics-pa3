#include "cpu/exec/helper.h"

void pio_write(ioaddr_t, size_t, uint32_t);

make_helper(out_b) {
	pio_write(cpu.dx, 1, cpu.al);
	return 1;
}

make_helper(out_v) {
	if (ops_decoded.is_data_size_16) 
		pio_write(cpu.dx, 2, cpu.ax);
	else 
		pio_write(cpu.dx, 2, cpu.eax);
	return 1;
}
