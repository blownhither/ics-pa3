#include "trap.h"
#include "FLOAT.h"
FLOAT a,c;
int b;
int main(){
	for(c=-100;c<=0x10000;++c){
		nemu_assert(int2F(c) == (c<<16));
	}
	set_bp();
	a = f2F(2.0);
	nemu_assert(0x2ffff==a);
	set_bp();
	b = F2int(a);
	nemu_assert(2==b);

	HIT_GOOD_TRAP;
	return 0;
}
