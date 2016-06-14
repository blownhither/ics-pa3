#include "cpu/exec/helper.h"

make_helper(exec);

//ESI-->EDI
extern bool parity_check(uint32_t ); 
make_helper(cmps_b) {
	//const int ES = 0;	//TODO: check ES
	int a,b;
	uint8_t ans = (a=swaddr_read(cpu.esi,1)) - (b=swaddr_read(cpu.edi,1));
	cpu.edi += eflags.eflags.DF? -1:1;
	cpu.esi += eflags.eflags.DF? -1:1;
	eflags.eflags.CF = (a > b);
	eflags.eflags.PF = parity_check(ans);
	//eflags.eflags.AF = 
	eflags.eflags.SF = (ans>>7)&1;
	eflags.eflags.ZF = (a == b);
	eflags.eflags.OF = (a<0 && b>0 && ans<0) || (a>0 && b<0 && ans>0);
	return 1;
}

make_helper(cmps_v) {
	//const int ES = 0;	//TODO: check ES
	int a,b;
	if(ops_decoded.is_data_size_16){
		uint16_t ans = (a=swaddr_read(cpu.esi,2)) - (b=swaddr_read(cpu.edi,2));
		cpu.edi += eflags.eflags.DF? -2:2;
		cpu.esi += eflags.eflags.DF? -2:2;
		eflags.eflags.CF = (a > b);
		eflags.eflags.PF = parity_check(ans);
		//eflags.eflags.AF = 
		eflags.eflags.SF = (ans>>15)&1;
		eflags.eflags.ZF = (a == b);
		eflags.eflags.OF = (a<0 && b>0 && ans<0) || (a>0 && b<0 && ans>0);
	}
	else{
		uint32_t ans = (a=swaddr_read(cpu.esi,4)) - (b=swaddr_read(cpu.edi,4));
		cpu.edi += eflags.eflags.DF? -4:4;
		cpu.esi += eflags.eflags.DF? -4:4;
		eflags.eflags.CF = (a > b);
		eflags.eflags.PF = parity_check(ans);
		//eflags.eflags.AF = 
		eflags.eflags.SF = (ans>>31)&1;
		eflags.eflags.ZF = (a == b);
		eflags.eflags.OF = (a<0 && b>0 && ans<0) || (a>0 && b<0 && ans>0);
	}
	return 1;
}
