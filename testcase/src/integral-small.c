#include "trap.h"
#include "FLOAT.h"
FLOAT a;
int b;
int main(){
	a = f2F(2.0);
	nemu_assert(0x2ffff==a);
	b = F2int(a);
	nemu_assert(2==b);
	HIT_GOOD_TRAP;
	return 0;
}
