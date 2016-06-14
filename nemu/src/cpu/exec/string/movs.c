#include "cpu/exec/helper.h"
//make_helper(exec);
lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t cur_segr);

make_helper(movs_b) {
	lnaddr_t es_edi = seg_translate(cpu.edi, 1, ES_NUM);
	lnaddr_t ds_esi = seg_translate(cpu.esi, 1, DS_NUM);
	lnaddr_write(es_edi, 1, lnaddr_read(ds_esi,1));
	//printf("cpu.edi = 0x%x, cpu.esi = 0x%x, data=0x%x\n", cpu.edi, cpu.esi, swaddr_read(cpu.esi,1));
	cpu.edi += eflags.eflags.DF? -1:1;
	cpu.esi += eflags.eflags.DF? -1:1;

	return 1;
}

make_helper(movs_v) {
	lnaddr_t es_edi;
	lnaddr_t ds_esi;
	if(ops_decoded.is_data_size_16){
		es_edi = seg_translate(cpu.edi, 2, ES_NUM);
		ds_esi = seg_translate(cpu.esi, 2, DS_NUM);
		lnaddr_write(es_edi, 2, lnaddr_read(ds_esi,2));
		cpu.edi += eflags.eflags.DF? -2:2;
		cpu.esi += eflags.eflags.DF? -2:2;
	}
	else{
		es_edi = seg_translate(cpu.edi, 4, ES_NUM);
		ds_esi = seg_translate(cpu.esi, 4, DS_NUM);
		lnaddr_write(es_edi, 4, lnaddr_read(ds_esi,4));
		cpu.edi += eflags.eflags.DF? -4:4;
		cpu.esi += eflags.eflags.DF? -4:4;
	}
	return 1;
}

//NOTE: does double word prefix still means data_size_16 before movs?
